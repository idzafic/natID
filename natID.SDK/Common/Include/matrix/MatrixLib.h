// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file MatrixLib.h
    @brief Defines platform-specific export/import macros for the Matrix shared library. */
#pragma once
#include <td/Types.h>
#include <compiler/Definitions.h>

#ifdef MU_WINDOWS
    #ifdef MATRIX_EXPORTS
        #define MATRIX_API __declspec(dllexport)
        #define MATRIX_EXTERN
    #else
        #define MATRIX_API __declspec(dllimport)
        #define MATRIX_EXTERN extern
    #endif
#else
    #ifdef MATRIX_EXPORTS
        #define MATRIX_API __attribute__((visibility("default")))
        #define MATRIX_EXTERN
    #else
        #define MATRIX_API
        #define MATRIX_EXTERN extern
    #endif
#endif
