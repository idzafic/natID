// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Format.h
 * @brief Numeric formatting options for GUI controls, including floating-point display settings.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/natGUI.h>
#include <td/Types.h>

namespace gui
{

/// @brief Controls whether and how the thousands separator is rendered for numeric values.
enum class ThSep : unsigned char {AppDefault=0, DoNotShowThSep, ShowThSep};

/// @brief Describes the floating-point display format applied to numeric GUI controls.
class NATGUI_API NumFormat
{
public:
    td::FormatFloat floatFmt = td::FormatFloat::Decimal; ///< The floating-point notation style (e.g., Decimal, Scientific).
    char nDecForReal = 6;                                 ///< Number of decimal places shown for real (floating-point) values.

    /// @brief Constructs a NumFormat with the given decimal count and optional notation style.
    /// @param nDec Number of decimal places to display.
    /// @param floatingNoFmt Floating-point notation style; defaults to Decimal.
    NumFormat(int nDec, td::FormatFloat floatingNoFmt = td::FormatFloat::Decimal);
    /// @brief Copies format settings from another NumFormat instance.
    /// @param nf The source NumFormat to copy from.
    void operator = (const NumFormat& nf);
};

} //namespace gui
