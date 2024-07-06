//===-- llvm/Support/Compiler.h - Compiler abstraction support --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file defines several macros, based on the current compiler.  This allows
// use of compiler-specific features in a way that remains portable. This header
// can be included from either C or C++.
//
//===----------------------------------------------------------------------===//

#ifndef CLANG_SUPPORT_COMPILER_H
#define CLANG_SUPPORT_COMPILER_H

#include "llvm/Support/Compiler.h"

/// CLANG_ABI is the main export/visibility macro to mark something as explicitly
/// exported when llvm is built as a shared library with everything else that is 
/// unannotated will have internal visibility.
/// 
/// CLANG_EXPORT_TEMPLATE is used on explicit template instantiations in source
/// files that were declared extern in a header. This macro is only set as a 
/// compiler export attribute on windows, on other platforms it does nothing.
/// 
/// CLANG_TEMPLATE_ABI is for annotating extern template declarations in headers 
/// for both functions and classes. On windows its turned in to dllimport for 
/// library consumers, for other platforms its a default visibility attribute.
/// 
/// CLANG_C_ABI is used to annotated functions and data that need to be exported 
/// for the libllvm-c API. This used both for the llvm-c headers and for the 
/// functions declared in the different Target's c++ source files that don't include
/// the header forward declaring them.
#ifndef CLANG_ABI_GENERATING_ANNOTATIONS
// Marker to add to classes or functions in public headers that should not have
// export macros added to them by the clang tool
#define CLANG_ABI_NOT_EXPORTED
#if defined(LLVM_BUILD_LLVM_DYLIB) || defined(LLVM_BUILD_SHARED_LIBS) ||       \
    defined(CLANG_ENABLE_PLUGINS)
#if defined(_WIN32)
#if defined(CLANG_BUILD_STATIC)
#  define CLANG_ABI
#  define CLANG_TEMPLATE_ABI
#  define CLANG_EXPORT_TEMPLATE
#else
# if defined(CLANG_EXPORTS)
#  define CLANG_ABI __declspec(dllexport)
#  define CLANG_TEMPLATE_ABI
#  define CLANG_EXPORT_TEMPLATE CLANG_ABI
# else
#  define CLANG_ABI __declspec(dllimport)
#  define CLANG_TEMPLATE_ABI __declspec(dllimport)
#  define CLANG_EXPORT_TEMPLATE
# endif
#endif
#define CLANG_ABI_EXPORT __declspec(dllexport)
#elif defined(__ELF__)
#  define CLANG_ABI LLVM_ATTRIBUTE_VISIBILITY_DEFAULT
#  define CLANG_TEMPLATE_ABI LLVM_ATTRIBUTE_VISIBILITY_DEFAULT
#  define LLVM_EXPORT_TEMPLATE
#  define CLANG_ABI_EXPORT LLVM_ATTRIBUTE_VISIBILITY_DEFAULT
#elif defined(__MACH__) || defined(__WASM__)
#  define CLANG_ABI LLVM_ATTRIBUTE_VISIBILITY_DEFAULT
#  define CLANG_TEMPLATE_ABI
#  define CLANG_EXPORT_TEMPLATE
#  define CLANG_ABI_EXPORT LLVM_ATTRIBUTE_VISIBILITY_DEFAULT
#endif
#define CLANG_C_ABI CLANG_ABI
#define CLANG_CLASS_ABI CLANG_ABI
#else
#  define CLANG_C_ABI 
#  define CLANG_ABI
#  define CLANG_ABI_EXPORT
#  define CLANG_CLASS_ABI
#  define CLANG_TEMPLATE_ABI
#  define CLANG_EXPORT_TEMPLATE
#endif
#endif

#endif