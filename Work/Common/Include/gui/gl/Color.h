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
#include <td/ColorID.h>

namespace gui
{

namespace gl
{

class NATGL_API Color
{
public:
	static void get(td::ColorID colorID, glm::vec3& color);
    static void get(td::ColorID colorID, glm::vec4& color);
};

} //namespace gl

} //namespace gui

