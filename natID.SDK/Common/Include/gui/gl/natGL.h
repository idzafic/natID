// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file natGL.h
    @brief Platform-specific export/import macros for the natGL shared library. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <compiler/Definitions.h>

#ifdef MU_WINDOWS
	#ifdef NATGL_EXPORTS
	#define NATGL_API __declspec(dllexport) ///< Exports symbols when building the natGL DLL on Windows.
	#else
	#define NATGL_API __declspec(dllimport) ///< Imports symbols when consuming the natGL DLL on Windows.
	#endif
#else
	#ifdef NATGL_EXPORTS
	#  define NATGL_API __attribute__((visibility("default"))) ///< Exports symbols when building the natGL shared library on non-Windows platforms.
	#else
    #  define NATGL_API ///< Empty definition when consuming the natGL library on non-Windows platforms.
	#endif
#endif
