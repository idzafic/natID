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
//    void setPreferredFrameRateRange(float minFPS, float maxFPS);
    
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
