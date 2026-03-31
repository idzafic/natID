// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include "natGLX.h"
#include <glx/Types.h>
#include <functional>

namespace MTL
{
class Drawable;
class MetalLayer;
} //namespace MTL

namespace glx
{

class Drawable;
class Texture;
class CommandBuffer;


class NATGLX_API Drawable
{
    friend class CommandBuffer;
    friend class MTL::MetalLayer;

private:
    MTL::Drawable* _pDrawable;
    
    MTL::Drawable* getNatPtr()
    {
        return _pDrawable;
    }
    
    const MTL::Drawable* getNatPtr() const
    {
        return _pDrawable;
    }
public:
    Drawable();
    Drawable(MTL::Drawable* pDrawable);
    Drawable(const Drawable& drawable);
    void operator = (const Drawable& drawable);
    using PresentedHandlerFunction = std::function<void(Drawable*)>;

	friend class MTL::Drawable;
public:
    
    Texture texture();

    void addPresentedHandler(const Drawable::PresentedHandlerFunction& function);

    bool isOk() const;

    void present();

    void presentAtTime(TimeInterval presentationTime);

    void presentAfterMinimumDuration(TimeInterval duration);

    //void           addPresentedHandler(const MTL::DrawablePresentedHandler block);

    TimeInterval presentedTime() const;

    size_t   drawableID() const;

    
    void retain();
    void release();
};


} //namespace glx

