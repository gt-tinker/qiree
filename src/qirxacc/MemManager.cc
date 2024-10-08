//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirxacc/MemManager.cc
//---------------------------------------------------------------------------//

#include "MemManager.hh"

namespace
{
struct RuntimeTuple
{
    qiree::size_type refcount;
    char contents[];
};

struct RuntimeArray
{
    qiree::size_type refcount;
    qiree::size_type elem_size;
    qiree::size_type length;
    char contents[];
};
}  // namespace

namespace qiree
{
//---------------------------------------------------------------------------//
// MEMORY MANAGEMENT
//---------------------------------------------------------------------------//

Array MemManager::array_create_1d(uint32_t elem_size, uint64_t length)
{
    RuntimeArray* arr
        = (RuntimeArray*)calloc(sizeof(RuntimeArray) + elem_size * length, 1);
    arr->refcount = 1;
    arr->elem_size = elem_size;
    arr->length = length;
    return arr + 1;
}

void MemManager::array_update_reference_count(Array array, int32_t delta)
{
    RuntimeArray* arr = (RuntimeArray*)array - 1;
    arr->refcount += delta;
    if (!arr->refcount)
    {
        free(arr);
    }
}

void* MemManager::array_get_element_ptr_1d(Array array, uint64_t index)
{
    RuntimeArray* arr = (RuntimeArray*)array - 1;
    return &arr->contents[arr->elem_size * index];
}

uint64_t MemManager::array_get_size_1d(Array array)
{
    RuntimeArray* arr = (RuntimeArray*)array - 1;
    return arr->length;
}

Tuple MemManager::tuple_create(uint64_t num_bytes)
{
    RuntimeTuple* tup
        = (RuntimeTuple*)calloc(sizeof(RuntimeTuple) + num_bytes, 1);
    tup->refcount = 1;
    return tup + 1;
}

void MemManager::tuple_update_reference_count(Tuple tuple, int32_t delta)
{
    RuntimeTuple* tup = (RuntimeTuple*)tuple - 1;
    tup->refcount += delta;
    if (!tup->refcount)
    {
        free(tup);
    }
}

// Misc utils

uint32_t MemManager::array_get_elem_size(Array array)
{
    RuntimeArray* arr = (RuntimeArray*)array - 1;
    return arr->elem_size;
}
}  // namespace qiree
