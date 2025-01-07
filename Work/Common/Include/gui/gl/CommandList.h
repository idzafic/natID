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
	
class NATGL_API CommandList
{
private:
    void* _pMtxAllocator = nullptr;
    cnt::PushBackVector<gui::gl::Command> _commands;
public:
    CommandList();
    CommandList(size_t nToReserve);
    void reserve(size_t nToReserve);
    
    gui::gl::Command* get();
    bool encode(gui::gl::Command* pComm);
    bool remove(gui::gl::Command* pComm);
    
    glm::mat4x4* allocMatrix();
    void execute();
    void execute(size_t startPos, size_t count);
    
    size_t size() const;
    
    void reset();
    void release();
};


} //namespace gl

} //namespace gui

