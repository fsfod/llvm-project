//===- StructurizeCFG.h ---------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_SCALAR_STRUCTURIZECFG_H
#define LLVM_TRANSFORMS_SCALAR_STRUCTURIZECFG_H

#include "llvm/IR/PassManager.h"
#include "llvm/Support/Compiler.h"

namespace llvm {
struct LLVM_ABI StructurizeCFGPass : PassInfoMixin<StructurizeCFGPass> {
private:
  bool SkipUniformRegions;

public:
  StructurizeCFGPass(bool SkipUniformRegions = false);

  void printPipeline(raw_ostream &OS,
                     function_ref<StringRef(StringRef)> MapClassName2PassName);

  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};
} // namespace llvm

#endif // LLVM_TRANSFORMS_SCALAR_STRUCTURIZECFG_H
