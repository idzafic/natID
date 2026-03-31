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
#include <td/Types.h>
#include <gui/gl/GL.h>
#include <cnt/PushBackVector.h>
#include <tuple>

namespace gui
{
namespace gl
{

//Generates triangulated symmetric polygons in form of TRIANGL_FAN structure (2d, xy plane)
NATGL_API void generateSymPolygon(const gui::gl::Circle& c, size_t nNodes, cnt::PushBackVector<gui::gl::Point2d>& vertices, float firstPtAngle = 0, cnt::PushBackVector<gui::gl::Point2d>* pTextureCoords = nullptr);

} //namespace gl
} //namespace gui

