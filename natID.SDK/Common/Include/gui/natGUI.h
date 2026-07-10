// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file natGUI.h
    @brief Platform-specific export/import macros for the natGUI shared library. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once

#include <compiler/Definitions.h>

#ifdef MU_WINDOWS
	#ifdef NATGUI_EXPORTS
	#define NATGUI_API __declspec(dllexport) ///< Exports symbols when building the natGUI DLL on Windows.
	#else
	#define NATGUI_API __declspec(dllimport) ///< Imports symbols when consuming the natGUI DLL on Windows.
	#endif
#else
	#ifdef NATGUI_EXPORTS
    #define NATGUI_API __attribute__((visibility("default"))) ///< Exports symbols when building the natGUI shared library on non-Windows platforms.
	#else
    #define NATGUI_API ///< Empty definition when consuming the natGUI library on non-Windows platforms.
	#endif
#endif


