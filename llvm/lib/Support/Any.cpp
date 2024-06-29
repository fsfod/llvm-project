//===----- lib/Support/Any.cpp - Any object holder class -------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Any.h"
#include <string>

using namespace llvm;

// Create explicit instantiations for tests 
template void* Any::anchor<char>();
template void* Any::anchor<unsigned char>();
template void* Any::anchor<int>();
template void* Any::anchor<int*>();
template void* Any::anchor<double>();
template void* Any::anchor<float>();
template void* Any::anchor<char*>();
template void* Any::anchor<char const *>();
template void* Any::anchor<std::string>();