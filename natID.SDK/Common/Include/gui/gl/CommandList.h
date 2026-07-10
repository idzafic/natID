// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file CommandList.h
 * @brief Manages an ordered list of encoded GPU commands for deferred execution.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/natGL.h>
#include <td/Types.h>
#include <gui/gl/GL.h>
#include <cnt/PushBackVector.h>
#include <td/ColorID.h>
//#include <gui/gl/Program.h>
#include <gui/gl/Command.h>

namespace gui
{
namespace gl
{

/// @brief Stores and executes an ordered sequence of Command objects, with optional matrix allocation.
class NATGL_API CommandList
{
private:
    void* _pMtxAllocator = nullptr; ///< Pointer to the internal matrix memory allocator.
    cnt::PushBackVector<gui::gl::Command> _commands; ///< Storage for encoded commands.

    CommandList(const CommandList&) = delete;
    CommandList& operator =(const CommandList&) = delete;
public:
    /// @brief Default constructor; creates an empty command list.
    CommandList();

    /// @brief Constructor that pre-allocates storage for a known number of commands.
    /// @param nToReserve Number of command slots to reserve.
    CommandList(size_t nToReserve);

    /// @brief Reserves storage for additional commands.
    /// @param nToReserve Number of command slots to reserve.
    void reserve(size_t nToReserve);

    /// @brief Allocates and returns a new, unencoded Command slot at the end of the list.
    /// @return Pointer to the newly allocated Command, or nullptr on failure.
    gui::gl::Command* get();

    /// @brief Finalizes encoding for a previously obtained command and adds it to the list.
    /// @param pComm Pointer to the Command returned by get().
    /// @return true if encoding succeeded.
    bool encode(gui::gl::Command* pComm);

    /// @brief Removes the specified command from the list if it is the last entry.
    /// @param pComm Pointer to the Command to remove.
    /// @return true if the command was successfully removed.
    bool remove(gui::gl::Command* pComm);

    /// @brief Allocates a glm::mat4x4 from the internal matrix pool.
    /// @return Pointer to the allocated matrix, or nullptr on allocation failure.
    glm::mat4x4* allocMatrix();

    /// @brief Executes all encoded commands in order.
    void execute();

    /// @brief Executes a sub-range of encoded commands.
    /// @param startPos Zero-based index of the first command to execute.
    /// @param count Number of commands to execute.
    void execute(size_t startPos, size_t count);

    /// @brief Returns the number of encoded commands currently in the list.
    /// @return Command count.
    size_t size() const;

    /// @brief Resets command state without releasing allocated memory.
    void reset();

    /// @brief Releases all memory held by the command list and resets its state.
    void release();
};


} //namespace gl

} //namespace gui
