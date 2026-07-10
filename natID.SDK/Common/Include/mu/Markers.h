// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Markers.h
    @brief Exports a shared zero-buffer constant used as a marker sentinel in the mu library. */
#pragma once
#include <td/Types.h>

namespace mu
{
	extern MAINUTILS_API const td::UTF8 __zeroBuffer[8]; ///< Shared zero-initialized buffer used as a sentinel marker.
};
