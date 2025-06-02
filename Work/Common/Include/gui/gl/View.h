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
#include "natGL.h"
#include "Context.h"
#include <gui/DrawableView.h>
#include <gui/gl/GL.h>
#include <tuple>
#include <gui/Cursor.h>
#include <gui/Texture.h>
#include <functional>

namespace gui
{
namespace gl
{

class ViewHelper;

class NATGL_API View : public gui::DrawableView
{
    friend class ViewHelper;
    
private:
    std::function<void()> _onTextureRead;
protected:
    float _logicalToPhysicalPixelScale = 1.0f;
    float _logicaToPhysicalSizeScale = 1.0f;
private:
    td::UINT4 _screenFB = 0xFFFFFFFF;
    td::BYTE _contRenderMode = 0;
private:
    void _prepareNextFrame();
    
protected:
    void measure(CellInfo& cell) override;
    void reMeasure(CellInfo& cell) override;
    void reRender() const;
    
    virtual double getZoom() const; //neeeds to overloaded in upper classes
    //link GLSL program using vertex and fragment shader from resource texts
    td::UINT4 linkProgram(const char* vertexResID, const char* fragmentResID);
    
    //create programs, allocate vbos, etc in onInit
    virtual void onInit();
    
    void makeCurrentContext() const;
    
    //delete programs, vbos, ...
    virtual void cleanResources();
    
    std::tuple<int, int> getOpenGLVersion() const;
//    void releaseGL();
    virtual void onZoomChanged();
    virtual void onOriginChanged();
    
    void createContext(const gl::Context& context);
    void createContext(const gl::Context& context, const std::initializer_list<InputDevice::Event>& events, const Geometry* g = nullptr);

    void readFrameFrameBuffer(gui::Texture& texture, const std::function<void()>& fn);
    virtual bool prepareNextFrame();
    
    void registerForFocusEvents() const;
    
    void activateScreenFB();
    
    float getLogicalToPhysicalPixelScale() const
    {
        return _logicalToPhysicalPixelScale;
    }
    
    float getLogicalToPhysicalSizeScale() const
    {
        return _logicaToPhysicalSizeScale;
    }
#ifdef MU_DEBUG
    const char* const dbgGetErrorStringCode(GLenum error) const
    {
        switch (error) {
            case GL_NO_ERROR:
                return "GL_NO_ERROR: No error has been recorded.";
            case GL_INVALID_ENUM:
                return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
            case GL_INVALID_VALUE:
                return "GL_INVALID_VALUE: A numeric argument is out of range.";
            case GL_INVALID_OPERATION:
                return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
#ifdef GL_STACK_OVERFLOW
            case GL_STACK_OVERFLOW:
                return "GL_STACK_OVERFLOW: This command would cause a stack overflow. (Deprecated in modern OpenGL)";
#endif
#ifdef GL_STACK_UNDERFLOW
            case GL_STACK_UNDERFLOW:
                return "GL_STACK_UNDERFLOW: This command would cause a stack underflow. (Deprecated in modern OpenGL)";
#endif
            case GL_OUT_OF_MEMORY:
                return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
    #ifdef GL_CONTEXT_LOST
            case GL_CONTEXT_LOST:
                return "GL_CONTEXT_LOST: The OpenGL context has been lost (typically due to a graphics reset).";
    #endif
            default:
                return "Unknown OpenGL error.";
        }
    }
#endif
    
    inline void dbgCheckGLError() const
    {
#ifdef MU_DEBUG
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            const char* const pErr = dbgGetErrorStringCode(error);
            mu::dbgLog("ERROR! OpenGL error in setupVBO! Error code = %x, desc=%s", error, pErr);
            assert(false);
        }
#endif
    }
    
    View();
public:
    ~View();
    gui::ObjType getObjType() const override { return ObjType::GLView;}
    
    void setContRenderMode(bool bContinuous);
    bool isContRenderMode() const;
    void getVisiblePartInModelCoordinates(gui::Rect& r) const;
    
    void setCursor(Cursor::Type ct, bool showImmediately = false) const;
    
    void startAnimation();
    void stopAnimation();
    bool isAnimated() const;
};

} //namespace gl
} //namespace gui
