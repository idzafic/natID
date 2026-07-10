// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Alignment.h
    @brief Defines compile-time alignment and trailing-element constants for SIMD memory layouts. */
#pragma once

/// @brief Required memory alignment in bytes for dense SIMD data buffers.
#ifndef _SP_SIMD_DENSE_ALIGNMENT
#define _SP_SIMD_DENSE_ALIGNMENT    16
#endif

/// @brief Required memory alignment in bytes for sparse SIMD data buffers.
#ifndef _SP_SIMD_ALIGNMENT
#define _SP_SIMD_ALIGNMENT    16
#endif

/// @brief Number of trailing (padding) elements appended to SIMD-aligned buffers to prevent overread.
#ifndef _SP_SIMD_TRAILING
#define _SP_SIMD_TRAILING 3
#endif
