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
#include <gui/gl/natGL.h>
#include <gui/gl/GL.h>

namespace gui
{
namespace gl
{

class ICameraLimiter
{
public:
    enum class Move : td::BYTE {No=0, Limited, Desired};
    virtual Move makeMove(const glm::vec3& initialPos, const glm::vec3& desiredPosition, glm::vec3& finalePosition) = 0;
};

} //namespace gl
} //namespace gui

