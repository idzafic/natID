#pragma once
#include <glx/View.h>
#include "Renderer.h"
#include <gui/Canvas.h>


class ViewGLX : public glx::View
{
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


};
