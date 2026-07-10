#pragma once
#include <glx/View.h>
#include "Renderer.h"
#include <gui/Canvas.h>
#include <gui/FileDialog.h>
#include <gui/Texture.h>
#include <gui/Alert.h>
#include <mu/DebugConsoleLog.h>

class ViewGLX : public glx::View
{
protected:
    gui::Texture _frameTexture; //for frame export

protected:
    glx::IRenderer* createRenderer() override
    {
        _pRenderer = new Renderer(this);
        return _pRenderer;
    }
    
    //Helper methods (IRenderer -> My Renderer)
    inline Renderer* getRenderer()
    {
        return static_cast<Renderer*>(_pRenderer);
    }
    
    inline const Renderer* getRenderer() const
    {
        return static_cast<const Renderer*>(_pRenderer);
    }
public:
    ViewGLX()
    {
    }
    
    void updateTextScale(float val)
    {
        if (_pRenderer)
        {
            // Update the texture scale in renderer and redraw
            getRenderer()->setTextureScale(val);
            reDraw();
        }
    }
    
    void switchTexture()
    {
        if (_pRenderer)
        {
            getRenderer()->switchTexture();
            reDraw();
        }
    }
    
    void exportFrame()
    {
        mu::DebugConsoleLog::info() << "Export frame functionality not yet implemented for this example";
    }
};
