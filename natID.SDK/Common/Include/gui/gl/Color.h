// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Color.h
    @brief Provides utility functions to convert color IDs into OpenGL-compatible float color vectors. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/natGL.h>
#include <gui/gl/GL.h>
#include <td/ColorID.h>

namespace gui
{

namespace gl
{

/// @brief Utility class for converting named color identifiers to GLM color vectors.
class NATGL_API Color
{
public:
	/// @brief Fills a vec3 with the RGB components corresponding to the given color ID.
	/// @param colorID The named color identifier to look up.
	/// @param color Output vec3 receiving the (R, G, B) components in [0, 1] range.
	static void get(td::ColorID colorID, glm::vec3& color);
    /// @brief Fills a vec4 with the RGBA components corresponding to the given color ID.
    /// @param colorID The named color identifier to look up.
    /// @param color Output vec4 receiving the (R, G, B, A) components in [0, 1] range.
    static void get(td::ColorID colorID, glm::vec4& color);
};

} //namespace gl

} //namespace gui
