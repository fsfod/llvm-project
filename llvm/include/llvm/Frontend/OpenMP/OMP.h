//===-- OMP.h - Core OpenMP definitions and declarations ---------- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the core set of OpenMP definitions and declarations.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_FRONTEND_OPENMP_OMP_H
#define LLVM_FRONTEND_OPENMP_OMP_H

#include "llvm/Frontend/OpenMP/OMP.h.inc"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Compiler.h"

namespace llvm::omp {
LLVM_ABI ArrayRef<Directive> getLeafConstructs(Directive D);
LLVM_ABI ArrayRef<Directive> getLeafConstructsOrSelf(Directive D);

LLVM_ABI ArrayRef<Directive>
getLeafOrCompositeConstructs(Directive D, SmallVectorImpl<Directive> &Output);

LLVM_ABI Directive getCompoundConstruct(ArrayRef<Directive> Parts);

LLVM_ABI bool isLeafConstruct(Directive D);
LLVM_ABI bool isCompositeConstruct(Directive D);
LLVM_ABI bool isCombinedConstruct(Directive D);

/// Create a nicer version of a function name for humans to look at.
LLVM_ABI std::string prettifyFunctionName(StringRef FunctionName);

/// Deconstruct an OpenMP kernel name into the parent function name and the line
/// number.
LLVM_ABI std::string deconstructOpenMPKernelName(StringRef KernelName, unsigned &LineNo);

} // namespace llvm::omp

#endif // LLVM_FRONTEND_OPENMP_OMP_H
