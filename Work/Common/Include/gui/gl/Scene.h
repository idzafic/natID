// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

class NATGL_API Scene
{
    friend class Program;
    typedef struct _part
    {
        gui::gl::Buffer* gpuBuffer;
        gui::gl::Program* program;
    }Part;
protected:
    
private:
    cnt::PushBackVector<Part> _parts;
protected:

public:
    Scene();
    ~Scene(); 
    //transfer data to GPU
    void execute();
    
    //release from GPU and reset internal pointers
    void reset();
    //release all memory allocated by the Buffer
    void release();
};

} //namespace gl
} //namespace gui

