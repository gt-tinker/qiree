//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirxacc/MemManager.hh
//---------------------------------------------------------------------------//
#pragma once

#include "qiree/Types.hh"

namespace qiree
{

//---------------------------------------------------------------------------//
/*!
 * Shared logic for memory management
 */
class MemManager
{
  public:
    // From RuntimeInterface
    static Array array_create_1d(uint32_t elem_size, uint64_t length);
    static void array_update_reference_count(Array array, int32_t delta);
    static void* array_get_element_ptr_1d(Array array, uint64_t index);
    static uint64_t array_get_size_1d(Array array);
    static Tuple tuple_create(uint64_t num_bytes);
    static void tuple_update_reference_count(Tuple tuple, int32_t delta);

    // Useful otherwise
    static uint32_t array_get_elem_size(Array array);
};

//---------------------------------------------------------------------------//
}  // namespace qiree
