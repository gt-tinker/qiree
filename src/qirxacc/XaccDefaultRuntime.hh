//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirxacc/XaccDefaultRuntime.hh
//---------------------------------------------------------------------------//
#pragma once

#include "qiree/RuntimeInterface.hh"
#include "qirxacc/MemManager.hh"
#include "qirxacc/XaccQuantum.hh"

namespace qiree
{
class XaccQuantum;

//---------------------------------------------------------------------------//
/*!
 * Print per-qubit measurement statistics.
 *
 * (Compare with \ref XaccTupleRuntime.)
 *
 * Example:
 * \code
 * tuple ret length 2
 * qubit 0 experiment <null>: {0: 509, 1: 515}
 * qubit 1 experiment <null>: {0: 509, 1: 515}
 * \endcode
 */
class XaccDefaultRuntime final : virtual public RuntimeInterface
{
  public:
    // Construct with XACC quantum runtime and options
    inline XaccDefaultRuntime(std::ostream& output,
                              XaccQuantum& xacc,
                              bool print_accelbuf);

    //!@{
    //! \name Runtime interface
    // Initialize the execution environment, resetting qubits
    void initialize(OptionalCString env) override;

    //! Mark the following N results as being part of an array named tag
    void array_record_output(size_type, OptionalCString tag) final;

    //! Mark the following N results as being part of a tuple named tag
    void tuple_record_output(size_type, OptionalCString) final;

    // Save one result
    void result_record_output(Result result, OptionalCString tag) final;
    //!@}

    // Memory management

    Array array_create_1d(uint32_t elem_size, uint64_t length) override
    {
        return MemManager::array_create_1d(elem_size, length);
    }
    void array_update_reference_count(Array array, int32_t delta) override
    {
        return MemManager::array_update_reference_count(array, delta);
    }
    void* array_get_element_ptr_1d(Array array, uint64_t index) override
    {
        return MemManager::array_get_element_ptr_1d(array, index);
    }
    uint64_t array_get_size_1d(Array array) override
    {
        return MemManager::array_get_size_1d(array);
    }
    Tuple tuple_create(uint64_t num_bytes) override
    {
        return MemManager::tuple_create(num_bytes);
    }
    void tuple_update_reference_count(Tuple tuple, int32_t delta) override
    {
        return MemManager::tuple_update_reference_count(tuple, delta);
    }

  private:
    std::ostream& output_;
    XaccQuantum& xacc_;
    bool const print_accelbuf_;

    void execute_if_needed();
};

//---------------------------------------------------------------------------//
/*!
 * Construct an \c XaccDefaultRuntime.
 *
 * The \c print_accelbuf argument determines whether the XACC \c
 * AcceleratorBuffer is dumped after execution.
 */
XaccDefaultRuntime::XaccDefaultRuntime(std::ostream& output,
                                       XaccQuantum& xacc,
                                       bool print_accelbuf = true)
    : output_(output), xacc_(xacc), print_accelbuf_(print_accelbuf)
{
}

//---------------------------------------------------------------------------//
}  // namespace qiree
