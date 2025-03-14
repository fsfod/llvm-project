//=--- CommonBugCategories.cpp - Provides common issue categories -*- C++ -*-=//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "clang/StaticAnalyzer/Core/BugReporter/CommonBugCategories.h"
#include "clang/Support/Compiler.h"

// Common strings used for the "category" of many static analyzer issues.
namespace clang {
namespace ento {
namespace categories {

CLANG_ABI const char *const AppleAPIMisuse = "API Misuse (Apple)";
CLANG_ABI const char *const CoreFoundationObjectiveC = "Core Foundation/Objective-C";
CLANG_ABI const char *const LogicError = "Logic error";
CLANG_ABI const char *const MemoryRefCount =
    "Memory (Core Foundation/Objective-C/OSObject)";
CLANG_ABI const char *const MemoryError = "Memory error";
CLANG_ABI const char *const UnixAPI = "Unix API";
CLANG_ABI const char *const CXXObjectLifecycle = "C++ object lifecycle";
CLANG_ABI const char *const CXXMoveSemantics = "C++ move semantics";
CLANG_ABI const char *const SecurityError = "Security error";
CLANG_ABI const char *const UnusedCode = "Unused code";
CLANG_ABI const char *const TaintedData = "Tainted data used";
} // namespace categories
} // namespace ento
} // namespace clang
