// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file natPlotLib.h
    @brief Platform-specific export/import macros for the natPlot shared library. */
#pragma once
#ifdef NATPLOT_EXPORTS
    #ifdef MU_WINDOWS
        #define NATPLOT_API __declspec(dllexport) ///< Exports symbols when building the natPlot DLL on Windows.
    #else
        #define NATPLOT_API __attribute__((visibility("default"))) ///< Exports symbols when building the natPlot shared library on non-Windows platforms.
    #endif
#else
    #ifdef MU_WINDOWS
        #define NATPLOT_API __declspec(dllimport) ///< Imports symbols when consuming the natPlot DLL on Windows.
    #else
        #define NATPLOT_API ///< Empty definition when consuming the natPlot library on non-Windows platforms.
    #endif
#endif
