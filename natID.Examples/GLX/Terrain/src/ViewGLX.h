#pragma once
#include <glx/View.h>
#include "Renderer.h"
#include <gui/Canvas.h>


class ViewGLX : public glx::View
{
protected:
    Renderer* _pRenderer = nullptr;  // Keep reference to our renderer
    
    glx::IRenderer* createRenderer() override
    {
        _pRenderer = new Renderer(this);
        return _pRenderer;
    }

public:
    ViewGLX()
    {
       
    }


};
