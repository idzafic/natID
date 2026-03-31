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
    Renderer* _pRenderer = nullptr;  // Keep reference to our renderer
    gui::Texture _frameTexture; //for frame export
    
    glx::IRenderer* createRenderer() override
    {
        _pRenderer = new Renderer(this);
        return _pRenderer;
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
            _pRenderer->setTextureScale(val);
            reDraw();
        }
    }
    
    void switchTexture()
    {
        if (_pRenderer)
        {
            _pRenderer->switchTexture();
            reDraw();
        }
    }
    
    void exportFrame()
    {
        mu::DebugConsoleLog::info() << "Export frame functionality not yet implemented for this example";
    }
};
