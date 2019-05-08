//
// Copyright (C) 2019 Assured Information Security, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef IMPLEMENTATION_MACROS_H
#define IMPLEMENTATION_MACROS_H

// -----------------------------------------------------------------------------
// Includes
// -----------------------------------------------------------------------------

#include <bfgsl.h>
#include <bftypes.h>
#include <bfdebug.h>
#include <bfexception.h>
#include <bfupperlower.h>

#include <intrinsics.h>

#include <any>
#include <mutex>
#include <memory>
#include <unordered_map>

// -----------------------------------------------------------------------------
// Friends
// -----------------------------------------------------------------------------

template<typename T>
class bfmanager;
class private_entry;
class private_memory_manager;

// -----------------------------------------------------------------------------
// Macros
// -----------------------------------------------------------------------------

#define COPY_MOVE_SEMANTICS(name)                                               \
    public:                                                                     \
    name(name &&) = default;                                                    \
    name &operator=(name &&) = default;                                         \
    name(const name &) = delete;                                                \
    name &operator=(const name &) = delete;                                     \

#ifdef ENABLE_BUILD_TEST
#include <hippomocks.h>
#define MOCK_FUNCTION(name, func)                                               \
    static void name::mock(MockRepository &mocks, vcpu *vcpu) func
#else
#define MOCK_FUNCTION(unused1, unused2)
#endif

#define FRIEND_DEFINITIONS(unused)                                              \
    template<typename T>                                                        \
    friend class ::bfmanager;                                                   \
    friend class ::private_entry;

#endif
