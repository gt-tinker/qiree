//----------------------------------*-C++-*----------------------------------//
// Copyright 2024 UT-Battelle, LLC, and other QIR-EE developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//---------------------------------------------------------------------------//
//! \file qirxacc/XaccTupleRuntime.hh
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
 * Print per-tuple (or per-array) measurement statistics.
 *
 * (Compare with \ref XaccDefaultRuntime.)
 *
 * Example:
 * \code
 * tuple ret length 2 distinct results 2
 * tuple ret result 00 count 512
 * tuple ret result 11 count 512
 * \endcode
 */
class XaccTupleRuntime final : virtual public RuntimeInterface
{
  public:
    // Construct with XACC quantum runtime and options
    XaccTupleRuntime(std::ostream& output,
                     XaccQuantum& xacc,
                     bool print_accelbuf);

    //!@{
    //! \name Runtime interface
    // Initialize the execution environment, resetting qubits
    void initialize(OptionalCString env) override;

    // Execute circuit and mark the following N results as being part of an
    // array named tag
    void array_record_output(size_type, OptionalCString tag) final;

    // Execute circuit and mark the following N results as being part of a
    // tuple named tag
    void tuple_record_output(size_type, OptionalCString) final;

    // Execute circuit and report a single measurement result
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
    enum class GroupingType
    {
        tuple,
        array,
    };

    std::ostream& output_;
    XaccQuantum& xacc_;
    bool const print_accelbuf_;
    bool valid_;
    GroupingType type_;
    std::string tag_;
    size_type num_results_;
    std::vector<Qubit> qubits_;

    void execute_if_needed();
    void
    start_tracking(GroupingType type, std::string tag, size_type num_results);
    void push_result(Qubit q);
    void print_header(size_type num_distinct);
    void finish_tuple();

    static char const* to_cstring(GroupingType);
};

//---------------------------------------------------------------------------//
}  // namespace qiree
