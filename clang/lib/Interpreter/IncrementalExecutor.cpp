//===--- IncrementalExecutor.cpp - Incremental Execution --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the class which performs incremental code execution.
//
//===----------------------------------------------------------------------===//

#include "IncrementalExecutor.h"

#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Interpreter/PartialTranslationUnit.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/Debugging/DebuggerSupport.h"
#include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/ExecutionEngine/Orc/MapperJITLinkMemoryManager.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/Orc/TargetProcess/JITLoaderGDB.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/TargetSelect.h"

// Force linking some of the runtimes that helps attaching to a debugger.
LLVM_ATTRIBUTE_USED void linkComponents() {
  llvm::errs() << (void *)&llvm_orc_registerJITLoaderGDBWrapper
               << (void *)&llvm_orc_registerJITLoaderGDBAllocAction;
}

namespace clang {

IncrementalExecutor::IncrementalExecutor(
  llvm::orc::LLJITBuilder &Builder, 
  llvm::orc::ThreadSafeContext &TSC, llvm::Error &Err)
  : TSCtx(TSC) {
  llvm::ErrorAsOutParameter EAO(&Err);

  if (auto JitOrErr = Builder.create())
    Jit = std::move(*JitOrErr);
  else {
    Err = JitOrErr.takeError();
    return;
  }
}

IncrementalExecutor::~IncrementalExecutor() {}

void IncrementalExecutor::SetupJITBuilder(llvm::orc::LLJITBuilder& Builder,
  const clang::TargetInfo &TI, const std::string &OrcRuntimePath) {
  using namespace llvm::orc;

  auto JTMB = JITTargetMachineBuilder(TI.getTriple());
  JTMB.addFeatures(TI.getTargetOpts().Features);
  if (TI.getTriple().isOSWindows()) {
    JTMB.getOptions().EmulatedTLS = false;
  }
  Builder.setJITTargetMachineBuilder(JTMB);
  Builder.setPrePlatformSetup([](LLJIT &J) {
    // Try to enable debugging of JIT'd code (only works with JITLink for
    // ELF and MachO).
    consumeError(enableDebuggerSupport(J));
    return llvm::Error::success();
  });
  Builder.setObjectLinkingLayerCreator(
    [](ExecutionSession &ES, const llvm::Triple &TT)
    -> llvm::Expected<std::unique_ptr<ObjectLayer>> {
    return std::make_unique<ObjectLinkingLayer>(
      ES, ES.getExecutorProcessControl().getMemMgr());
  });
  auto memmgr = MapperJITLinkMemoryManager::CreateWithMapper<InProcessMemoryMapper>(0x10000000);
  if (!memmgr) {
    return;
  }
  auto EPC = SelfExecutorProcessControl::Create(std::make_shared<SymbolStringPool>(), nullptr, std::move(*memmgr));
  if (EPC) {
    Builder.setExecutorProcessControl(std::move(*EPC));
  }

  if (!OrcRuntimePath.empty()) {
    Builder.setPlatformSetUp(ExecutorNativePlatform(OrcRuntimePath));
  }
}

llvm::Error IncrementalExecutor::addModule(PartialTranslationUnit &PTU) {
  llvm::orc::ResourceTrackerSP RT =
      Jit->getMainJITDylib().createResourceTracker();
  ResourceTrackers[&PTU] = RT;

  return Jit->addIRModule(RT, {std::move(PTU.TheModule), TSCtx});
}

llvm::Error IncrementalExecutor::removeModule(PartialTranslationUnit &PTU) {

  llvm::orc::ResourceTrackerSP RT = std::move(ResourceTrackers[&PTU]);
  if (!RT)
    return llvm::Error::success();

  ResourceTrackers.erase(&PTU);
  if (llvm::Error Err = RT->remove())
    return Err;
  return llvm::Error::success();
}

// Clean up the JIT instance.
llvm::Error IncrementalExecutor::cleanUp() {
  // This calls the global dtors of registered modules.
  return Jit->deinitialize(Jit->getMainJITDylib());
}

llvm::Error IncrementalExecutor::runCtors() const {
  return Jit->initialize(Jit->getMainJITDylib());
}

llvm::Expected<llvm::orc::ExecutorAddr>
IncrementalExecutor::getSymbolAddress(llvm::StringRef Name,
                                      SymbolNameKind NameKind) const {
  using namespace llvm::orc;
  auto SO = makeJITDylibSearchOrder({&Jit->getMainJITDylib(),
                                     Jit->getPlatformJITDylib().get(),
                                     Jit->getProcessSymbolsJITDylib().get()});

  ExecutionSession &ES = Jit->getExecutionSession();

  auto SymOrErr =
      ES.lookup(SO, (NameKind == LinkerName) ? ES.intern(Name)
                                             : Jit->mangleAndIntern(Name));
  if (auto Err = SymOrErr.takeError())
    return std::move(Err);
  return SymOrErr->getAddress();
}

} // end namespace clang
