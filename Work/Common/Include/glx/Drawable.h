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
#include <glx/Types.h>
#include <functional>

namespace MTL
{
class Drawable;
} //namespace MTL

namespace glx
{

class Drawable;
class Texture;

class NATGLX_API Drawable
{
private:
    MTL::Drawable* _pDrawable;
public:
    Drawable();
    Drawable(MTL::Drawable* pDrawable);
    void operator = (const Drawable& drawable);
    using PresentedHandlerFunction = std::function<void(Drawable*)>;
public:
    
    Texture texture();

    void addPresentedHandler(const Drawable::PresentedHandlerFunction& function);

    void present();

    void presentAtTime(TimeInterval presentationTime);

    void presentAfterMinimumDuration(TimeInterval duration);

    //void           addPresentedHandler(const MTL::DrawablePresentedHandler block);

    TimeInterval presentedTime() const;

    size_t   drawableID() const;
    
    MTL::Drawable* getNatPtr()
    {
        return _pDrawable;
    }
    
    const MTL::Drawable* getNatPtr() const
    {
        return _pDrawable;
    }
    
};


} //namespace glx

