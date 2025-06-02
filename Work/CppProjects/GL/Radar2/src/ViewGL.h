//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/View.h>
#include <glm/glm.hpp>
#include <gui/gl/View.h>
#include <gui/gl/Buffer.h>
#include "Program.h"


class ViewGL_PixelShader : public gui::gl::View
{
    Program _program;
    gui::gl::Buffer _gpuBuffer;

    int _screenSizeUniform = 0;
    int _animStepUniform = 0;

    gui::Size _size;

    int _stepCnt = 0;
    gui::Point _rightClickPos;
    gui::Point _leftClickPos;
    
private:
    bool setupShaders()
    {
        if (!_program.link(":shV", ":shF"))
            return false;
        return true;
    }

    //setup data and send it to GPU
    void setupVBO()
    {

        gui::gl::VertexType vertices[] =
        {
            1.0f, 1.0f,
            1.0f, -1.0f,
            -1.0f, -1.0f,
            -1.0f, 1.0f
        };

        _gpuBuffer.appendVertices(&vertices[0], 4);

        if (!_gpuBuffer.commit()) //transfer to GPU
        {
            mu::dbgLog("ERROR! Cannot commit buffer to GPU");
            return;
        }
        auto cmd = _gpuBuffer.createCommand();
        cmd->createDrawArrays(gui::gl::Primitive::TriangleFan, 0, 4);
        _gpuBuffer.encode(cmd);

        _program.setBuffer(&_gpuBuffer);
        
        dbgCheckGLError();
    }
protected:

    bool prepareNextFrame() override
    {
        ++_stepCnt;
        return true;
    }

    void onInit() override
    {
        if (!setupShaders())
        {
            mu::dbgLog("ERROR! OpenGL cannnot setup shaders!");
            assert(false);
            return;
        }
        setupVBO();
        
        dbgCheckGLError();
    }

    void onDraw(const gui::Rect& rect) override
    {
        // Clear the background
        gui::gl::Context::clear(td::ColorID::Black);
        gui::gl::Context::clear(gui::gl::Context::Clear::Color);

        _program.activate();

        _program.setLeftClick(_leftClickPos);
        _program.setRightClick(_rightClickPos);
        _program.setVAnimStep(_stepCnt);
        _program.setViewPortSize(_size);

        _program.execute();
        _program.deActivate();

        dbgCheckGLError();
    }


    bool onKeyPressed(const gui::Key& key) override
    {
        if (key.isASCII())
        {
            char ch = key.getChar();
        }
        return false;
    }

    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        _leftClickPos = inputDevice.getFramePoint();
        //take into consideration logical/physical ratio for HiDPI dispalys
        _leftClickPos.y = _size.height - _leftClickPos.y * _logicalToPhysicalPixelScale;
        _leftClickPos.x *= _logicalToPhysicalPixelScale;
    }

    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        _rightClickPos = inputDevice.getFramePoint();
        //take into consideration logical/physical ratio for HiDPI dispalys
        _rightClickPos.y = _size.height - _rightClickPos.y * _logicalToPhysicalPixelScale;
        _rightClickPos.x *= _logicalToPhysicalPixelScale;
    }

    void onResize(const gui::Size& newSize) override
    {
        _size = newSize;
        //take into consideration logical/physical frame size for HiDPI dispalys
        _size *= _logicaToPhysicalSizeScale;
        
        _leftClickPos.x = _size.width / 2.;
        _leftClickPos.y = _size.height / 2.;
        _rightClickPos = _leftClickPos;
    }
public:
    ViewGL_PixelShader()
    {
        _gpuBuffer.init(1, 4, 0, { gui::gl::DataType::vec2});

        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes);
        createContext(reqContext, { gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks });
    }

    ~ViewGL_PixelShader()
    {
        makeCurrentContext();
    }
};
