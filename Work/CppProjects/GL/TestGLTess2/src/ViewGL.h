//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/View.h>
#include <gui/gl/Program.h>
#include <gui/gl/Buffer.h>

#define DEBUG_GL

class ViewGL : public gui::gl::View
{
    gui::gl::Program _prog;
    gui::gl::Program _progWithTessel;
    gui::gl::Buffer _gpuBuffer;
private:

    bool setupShaders()
    {
        if (!_prog.link(":shVert", ":shFrag"))
            return false;
        if (!_progWithTessel.link(":shVert", ":shFrag", ":shTessCntrl", ":shTessEval"))
            return false;
        
        //        gui::gl::Context::setPointSize(5.0f);
        //        glDrawArrays(GL_POINTS, 0, 4);
        gui::gl::Command* cmd = _prog.createCommand();
        cmd->createDrawArrayPoints(0, 4, 5.0f);
        cmd->encode();
        
        // cmd->createDrawArrays(gui::gl::Primitive::Patches, 0, 4);
        // glDrawArrays(GL_PATCHES, 0, 4);
        cmd = _progWithTessel.createCommand();
        cmd->createDrawArrayPatches(0, 4, 4);
        cmd->encode();

        return true;
    }
    //setup data and send it to GPU
    void setupVBO()
    {
        // Vertex data for a simple rectangle with colors
        float vertices[] =
        {
             -0.5,  0.0,
              0.1,  0.8, 
              0.5, -0.5, 
             -0.5, -0.5
        };
        
        _gpuBuffer.appendVertices(&vertices[0], 4);
        
        if (!_gpuBuffer.commit()) //transfer to GPU
        {
            mu::dbgLog("ERROR! Cannot commit buffer to GPU");
            return;
        }
        
        _prog.setBuffer(&_gpuBuffer);
        _progWithTessel.setBuffer(&_gpuBuffer);
        
#ifdef DEBUG_GL
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            mu::dbgLog("ERROR! OpenGL error in setupVBO! Error code = %x", error);
            assert(false);
        }
#endif
    }
protected:
    
    void onInit() override
    {
        auto [major, minor] = getOpenGLVersion();
        mu::dbgLog("Used opengl %d.%d", major, minor);
        // Initialize OpenGL settings
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (!setupShaders())
        {
            mu::dbgLog("ERROR! OpenGL cannnot setup shaders!");
            assert(false);
            return;
        }
        setupVBO();
        
        //create
#ifdef DEBUG_GL
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            mu::dbgLog("OpenGL error: %x", error);
        }
#endif
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        gui::gl::Context::clear(td::ColorID::Black);
        gui::gl::Context::clear(gui::gl::Context::Clear::Color);
        
        _prog.activate();
        _prog.execute();

        gui::gl::Context::setPolygonMode(gui::gl::Context::PolygonMode::Line);
        _progWithTessel.activate();
        _progWithTessel.execute();

        _progWithTessel.deActivate();

#ifdef DEBUG_GL
        //check error
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) 
        {
            mu::dbgLog("ERROR! OpenGL error: %x", error);
            assert(false);
        }
#endif
    }
    
public:
    ViewGL()
    {
        //buffer must be initialized before context creation (or can be initialized in setupVBO)
        //init with 0 commands, 4 vertex entries (rows), and 0 indices
        _gpuBuffer.init(0, 4, 0, {gui::gl::DataType::vec2});
        
        gui::gl::Context reqContext;
        createContext(reqContext, {gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks });
    }
    
    ~ViewGL()
    {
        makeCurrentContext();
    }
};
