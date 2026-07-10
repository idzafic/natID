// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ObjectGenerator.h
 *  @brief Utility functions for generating triangulated 2-D geometric primitives for OpenGL rendering. */
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
/// @brief Generates vertices for a triangulated symmetric polygon in the XY plane using a TRIANGLE_FAN layout.
/// @param c Circle (centre and radius) that the polygon is inscribed in.
/// @param nNodes Number of polygon vertices (corners).
/// @param vertices Output vector that receives the generated vertex positions.
/// @param firstPtAngle Angle in radians of the first vertex, measured from the positive X axis.
/// @param pTextureCoords Optional output vector that receives UV texture coordinates matching each vertex.
NATGL_API void generateSymPolygon(const gui::gl::Circle& c, size_t nNodes, cnt::PushBackVector<gui::gl::Point2d>& vertices, float firstPtAngle = 0, cnt::PushBackVector<gui::gl::Point2d>* pTextureCoords = nullptr);

} //namespace gl
} //namespace gui
