//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/View.h>
#include <gui/gl/Buffer.h>
#include "Program.h"

class ViewTestGL2 : public gui::gl::View
{
    Program _program;
    gui::gl::Buffer _gpuBuffer;
    
    float _zoomFactor = 1.0f;
private:
    bool setupShaders()
    {
        if (!_program.link(":shVert", ":shFrag"))
            return false;
        return true;
    }
    //setup data and send it to GPU
    void setupVBO()
    {
        // Vertex data for a simple rectangle with colors
        gui::gl::VertexType vertices[] =
        {
            //x        y     R      G     B  (A is set to 1.0 in fragment shader)
            -0.5f,   0.5f,  1.0f, 0.0f, 0.0f, // Red color (rectangle)
             0.5f,   0.5f,  0.0f, 1.0f, 0.0f, // Green color (rectangle)
             0.5f,  -0.5f,  0.0f, 0.0f, 1.0f, // Blue color (rectangle)
            -0.5f,  -0.5f,  1.0f, 1.0f, 0.0f,  // Yellow color (end of rectangle)
            -1.0f,   0.0f,  1.0f, 0.0f, 0.0f,  // Red color (horizontal axis)
             1.0f,   0.0f,  0.0f, 1.0f, 0.0f,  // Green color (horizontal axis)
             0.0f,  -1.0f,  1.0f, 0.0f, 0.0f,  // Red color (vertical axis)
             0.0f,   1.0f,  0.0f, 1.0f, 0.0f,  // Green color (vertical axis)
            -0.98f, -0.98f, 1.0f, 0.0f, 0.0f,  // Red color (Bounding box)
             0.98f, -0.98f, 1.0f, 1.0f, 0.0f,  // Yellow color (Bounding box)
             0.98f,  0.98f, 0.0f, 1.0f, 0.0f,  // Green color (Bounding box)
            -0.98f,  0.98f, 1.0f, 1.0f, 0.0f  // Yellow color (Bounding box)
        };
        
        _gpuBuffer.appendVertices(&vertices[0], 12);
        
        if (!_gpuBuffer.commit()) //transfer to GPU
        {
            mu::dbgLog("ERROR! Cannot commit buffer to GPU");
            return;
        }
        
        //create command for rect
        auto cmd = _gpuBuffer.createCommand();
        cmd->createDrawArrays(gui::gl::Primitive::TriangleFan, 0, 4);
        _gpuBuffer.encode(cmd);
        
        //create command for axis
        cmd = _gpuBuffer.createCommand();
        cmd->createDrawArrays(gui::gl::Primitive::Lines, 4, 4);
        _gpuBuffer.encode(cmd);
        
        //create command for rect
        cmd = _gpuBuffer.createCommand();
        cmd->createDrawArrays(gui::gl::Primitive::LineLoop, 8, 4);
        _gpuBuffer.encode(cmd);
    
        _program.setBuffer(&_gpuBuffer);
        
        //check error
        dbgCheckGLError();
    }
protected:
    
    void onInit() override
    {
        if (!setupShaders())
        {
            mu::dbgLog("ERROR! OpenGL cannnot setup shaders!");
            assert(false);
            return;
        }
        setupVBO();
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        gui::gl::Context::clear(td::ColorID::Black);
        gui::gl::Context::clear({ gui::gl::Context::Clear::Color, gui::gl::Context::Clear::Depth });
        
        _program.activate();
        
        _program.setZoom(_zoomFactor);
        
        _program.execute();
        
        _program.deActivate();
    }
    
    void increaseZoom()
    {
        if (_zoomFactor >= 2.0f)
            return;
        _zoomFactor += 0.1f;
        reDraw();
    }
    
    void decreaseZoom()
    {
        if (_zoomFactor <= 0.11f)
            return;
        _zoomFactor -= 0.1f;
        reDraw();
    }
    
    bool onKeyPressed(const gui::Key& key) override
    {
        if (key.isASCII())
        {
            char ch = key.getChar();
            if (ch == 'G' || ch == 'g')
            {
                increaseZoom();
                return true;
            }
            else if (ch == 'H' || ch == 'h')
            {
                decreaseZoom();
                return true;
            }
        }
        return false;
    }
    
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        increaseZoom();
        auto pt = inputDevice.getFramePoint();
        mu::dbgLog("LeftMC(%.1f, %.1f)", pt.x, pt.y);
    }
    
    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        decreaseZoom();
        auto pt = inputDevice.getFramePoint();
        mu::dbgLog("RighMC(%.1f, %.1f)", pt.x, pt.y);
    }
public:
    ViewTestGL2()
    {
        //buffer must be initialized before context creation (or can be initialized in setupVBO)
        //init buffer with max 4 commands, 20 vertex sets, 0-indices, [vec2 vec3] attribs
        _gpuBuffer.init(4, 20, 0, {gui::gl::DataType::vec2, gui::gl::DataType::vec3});
        
        gui::gl::Context reqContext;
        createContext(reqContext, {gui::InputDevice::Event::Keyboard, 
            gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks });
    }
    
    ~ViewTestGL2()
    {
        makeCurrentContext();
    }
};
