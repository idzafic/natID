#pragma once
#include <glx/View.h>

//ovo samo za sada, razlika je u 
#ifdef MU_WINDOWS2
#include "RendererWnd.h"
#else
#include "Renderer.h"
#endif


class ViewGLX : public glx::View
{
protected:
    glx::IRenderer* createRenderer() override
    {
#ifdef MU_WINDOWS2
        return new RendererWnd(this);
#else
        return new Renderer(this);
#endif
    }

};
