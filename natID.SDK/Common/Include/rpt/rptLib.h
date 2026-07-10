// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file rptLib.h
 *  @brief Defines the NATRPT_API export/import macro for the natid reporting shared library.
 */
#pragma once

#include <compiler/Definitions.h>

#ifdef MU_WINDOWS
    #ifdef NATRPT_EXPORTS
    #define NATRPT_API __declspec(dllexport)
    #else
    #define NATRPT_API __declspec(dllimport)
    #endif
#else
    #ifdef NATRPT_EXPORTS
    #  define NATRPT_API __attribute__((visibility("default")))
    #else
    #  define NATRPT_API
    #endif
#endif
