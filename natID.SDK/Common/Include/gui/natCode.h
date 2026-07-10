// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file natCode.h
    @brief Platform-specific export/import macros for the natCode shared library. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once

#include <compiler/Definitions.h>

#ifdef MU_WINDOWS
	#ifdef NATCODE_EXPORTS
	#define NATCODE_API __declspec(dllexport) ///< Exports symbols when building the natCode DLL on Windows.
	#else
	#define NATCODE_API __declspec(dllimport) ///< Imports symbols when consuming the natCode DLL on Windows.
	#endif
#else
	#ifdef NATCODE_EXPORTS
    #define NATCODE_API __attribute__((visibility("default"))) ///< Exports symbols when building the natCode shared library on non-Windows platforms.
	#else
    #define NATCODE_API ///< Empty definition when consuming the natCode library on non-Windows platforms.
	#endif
#endif


