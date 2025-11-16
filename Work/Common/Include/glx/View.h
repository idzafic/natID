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
#include "natGLX.h"
#ifdef MU_NATGLXS
#include <glx/sa/DrawableView.h>
using DrawableView = glx::DrawableView;
#else
#include <gui/DrawableView.h>
using DrawableView = gui::DrawableView;
#endif

#include <gui/Cursor.h>
#include <glx/IRenderer.h>
#include <glx/Drawable.h>
#include <glx/Texture.h>
#include <glx/Types.h>
#include <glx/RenderPassDescriptor.h>

namespace glx
{


class ViewHelper;

class NATGLX_API View : public DrawableView
{
    friend class ViewHelper;
    
private:
    glx::IRenderer* _pRenderer = nullptr;
    glx::Device _device;
    
private:
    gui::Handle getNativeSurfaceHandle() override final;
//    void _prepareNextFrame();
    
protected:
    void measure(gui::CellInfo& cell) override final;
    void reMeasure(gui::CellInfo& cell) override final;
    
    virtual glx::IRenderer* createRenderer();
    void onResize(const gui::Size& newSize) override final;
    void onDraw(const gui::Rect& rect) override final;

    
public:
    View();
    View(const Device& device);
    ~View();
    gui::ObjType getObjType() const override { return gui::ObjType::GLXView;}
//
    void setPixelFormat(glx::PixelFormat format);
    glx::PixelFormat pixelFormat() const;

    void setDevice( const Device& device );
    const Device& device() const;

//    CA::MetalDrawable* currentDrawable() const;
    Drawable currentDrawable() const;

    void setFramebufferOnly( bool framebufferOnly );
    bool framebufferOnly() const;

    void setDepthStencilAttachmentTextureUsage(Texture::Usage textureUsage );
    Texture::Usage depthStencilAttachmentTextureUsage() const;

    void setMultisampleColorAttachmentTextureUsage(Texture::Usage textureUsage );
    Texture::Usage multisampleColorAttachmentTextureUsage() const;

    void setPresentsWithTransaction( bool presentsWithTransaction );
    bool presentsWithTransaction() const;

    void setColorPixelFormat(PixelFormat colorPixelFormat );
    PixelFormat colorPixelFormat() const;

    void setDepthStencilPixelFormat(PixelFormat colorPixelFormat );
    PixelFormat  depthStencilPixelFormat() const;

    void setSampleCount( size_t sampleCount );
    size_t sampleCount() const;

    void setClearColor( td::Color clearColor );
    td::Color clearColor() const;

    void setClearDepth( double clearDepth );
    double clearDepth() const;

    void setClearStencil( uint32_t clearStencil );
    uint32_t clearStencil() const;

    Texture depthStencilTexture() const;

    Texture multisampleColorTexture() const;

    void releaseDrawables() const;

    RenderPassDescriptor currentRenderPassDescriptor() const;

    void setPreferredFramesPerSecond(int preferredFramesPerSecond );
    int preferredFramesPerSecond() const;

    void setEnableSetNeedsDisplay( bool enableSetNeedsDisplay );
    bool enableSetNeedsDisplay() const;

    void setAutoresizeDrawable( bool autoresizeDrawable );
    bool autoresizeDrawable();

    void setDrawableSize( const glx::Size& drawableSize );
    void getDrawableSize(glx::Size& drawableSize) const;

    void getPreferredDrawableSize(glx::Size& sz) const;
    Device preferredDevice() const;

    void setPaused( bool paused );
    bool isPaused() const;

//    void                        setColorSpace( CGColorSpaceRef colorSpace );
//    CGColorSpaceRef                colorSpace() const;

    void draw();
    
//    void setContRenderMode(bool bContinuous);
//    bool isContRenderMode() const;
//    void getVisiblePartInModelCoordinates(gui::Rect& r) const;
//    
//    void setCursor(gui::Cursor::Type ct, bool showImmediately = false) const;
//    
//    void startAnimation();
//    void stopAnimation();
//    bool isAnimated() const;
};

} //namespace glx

