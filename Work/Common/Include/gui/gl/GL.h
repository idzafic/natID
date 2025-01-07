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
#include <compiler/Definitions.h>

#ifdef MU_WINDOWS
#include <epoxy/gl.h>
#else //other OSs

#ifdef MU_MACOS
#ifdef GTK_BACKEND
#include <epoxy/gl.h>
#else
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif
#include <OpenGL/gl3.h>
#endif //GTK_BACKEND
#else //Not MU_MACOS
//Linux with GTK
#include <epoxy/gl.h>
#endif
#endif

#include <gui/gl/Types.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>




