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
	#ifdef NATGL_EXPORTS
	#define NATGL_API __declspec(dllexport)
	#else
	#define NATGL_API __declspec(dllimport)
	#endif
#else
	#ifdef NATGL_EXPORTS
	#  define NATGL_API __attribute__((visibility("default")))
	#else
    #  define NATGL_API
	#endif
#endif

