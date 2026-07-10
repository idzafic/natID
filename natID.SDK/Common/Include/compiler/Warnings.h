// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Warnings.h
    @brief Suppresses common MSVC warnings that are informational or intentionally unused in this codebase. */
#pragma once
#include "Definitions.h"

#ifdef MU_VS_COMPILER
#pragma warning( disable : 4100 4189 )
#endif
