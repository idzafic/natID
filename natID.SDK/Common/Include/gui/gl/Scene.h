// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Scene.h
    @brief Declares the gui::gl::Scene class for managing a collection of GPU buffer/program pairs. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Buffer.h"
#include "Program.h"

namespace gui
{
namespace gl
{

/// @brief Manages a collection of GPU buffer and GLSL program pairs that form a renderable scene.
class NATGL_API Scene
{
    friend class Program;

    /// @brief Internal structure pairing a GPU buffer with its associated GLSL program.
    typedef struct _part
    {
        gui::gl::Buffer* gpuBuffer; ///< Pointer to the GPU buffer holding vertex data.
        gui::gl::Program* program;  ///< Pointer to the GLSL program used to render the buffer.
    }Part;
protected:

private:
    cnt::PushBackVector<Part> _parts; ///< Collection of all buffer/program pairs in the scene.
protected:
    /// @brief Deleted copy constructor; scenes are non-copyable.
    Scene(const Scene&) = delete;

    /// @brief Deleted copy-assignment operator; scenes are non-copyable.
    Scene& operator =(const Scene&) = delete;
public:
    /// @brief Default constructor; creates an empty scene.
    Scene();

    /// @brief Destructor; releases all GPU resources held by the scene.
    ~Scene();

    //transfer data to GPU
    /// @brief Transfers all pending buffer data to the GPU.
    void execute();

    //release from GPU and reset internal pointers
    /// @brief Releases GPU resources and resets all internal buffer/program pointers.
    void reset();

    //release all memory allocated by the Buffer
    /// @brief Releases all CPU and GPU memory allocated by the scene's buffers.
    void release();
};

} //namespace gl
} //namespace gui
