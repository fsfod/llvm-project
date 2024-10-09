//===- MachineSizeOpts.cpp - code size optimization related code ----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains some shared machine IR code size optimization related
// code.
//
//===----------------------------------------------------------------------===//

#include "llvm/CodeGen/MachineSizeOpts.h"
#include "llvm/Analysis/ProfileSummaryInfo.h"
#include "llvm/CodeGen/MBFIWrapper.h"
#include "llvm/CodeGen/MachineBlockFrequencyInfo.h"
#include "llvm/Support/Compiler.h"

using namespace llvm;

LLVM_ABI extern cl::opt<bool> EnablePGSO;
LLVM_ABI extern cl::opt<bool> PGSOLargeWorkingSetSizeOnly;
LLVM_ABI extern cl::opt<bool> ForcePGSO;
LLVM_ABI extern cl::opt<int> PgsoCutoffInstrProf;
LLVM_ABI extern cl::opt<int> PgsoCutoffSampleProf;

bool llvm::shouldOptimizeForSize(const MachineFunction *MF,
                                 ProfileSummaryInfo *PSI,
                                 const MachineBlockFrequencyInfo *MBFI,
                                 PGSOQueryType QueryType) {
  return shouldFuncOptimizeForSizeImpl(MF, PSI, MBFI, QueryType);
}

bool llvm::shouldOptimizeForSize(const MachineBasicBlock *MBB,
                                 ProfileSummaryInfo *PSI,
                                 const MachineBlockFrequencyInfo *MBFI,
                                 PGSOQueryType QueryType) {
  assert(MBB);
  return shouldOptimizeForSizeImpl(MBB, PSI, MBFI, QueryType);
}

bool llvm::shouldOptimizeForSize(const MachineBasicBlock *MBB,
                                 ProfileSummaryInfo *PSI,
                                 MBFIWrapper *MBFIW,
                                 PGSOQueryType QueryType) {
  assert(MBB);
  if (!PSI || !MBFIW)
    return false;
  BlockFrequency BlockFreq = MBFIW->getBlockFreq(MBB);
  return shouldOptimizeForSizeImpl(BlockFreq, PSI, &MBFIW->getMBFI(),
                                   QueryType);
}
