// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Display.h
    @brief Static utility class for querying display screen properties. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Types.h>
#include <cnt/SafeFullVector.h>

namespace gui
{

/// @brief Provides static helpers for querying display geometry and scale factors.
class NATGUI_API Display
{
    static float _backingScaleFactor;   ///< Cached backing scale factor of the default display.
    static float _scaleResolution;      ///< Cached resolution scale factor.
    static double _logigalPixelToMmHRatio; ///< Horizontal ratio: logical pixels per millimetre.
    static double _logigalPixelToMmVRatio; ///< Vertical ratio: logical pixels per millimetre.
    static double _dpi;                 ///< Dots-per-inch of the default display.
public:
    /// @brief Colour rendering mode of the display.
    enum class ColorMode : td::BYTE {
        Dark=0, ///< Dark (night) colour mode.
        Light,  ///< Light (day) colour mode.
        Any     ///< Mode is unspecified or mixed.
    };

    /// @brief Aggregated logical and physical dimensions for a single display.
    typedef struct _DI
    {
        gui::Size logicalSize;  ///< Screen dimensions in logical (non-device) pixels.
        gui::Size physicalSize; ///< Screen dimensions in millimetres.
    }Info;

    /// @brief Returns the logical size of the default display.
    /// @param sz Size structure to fill.
    static void getDefaultLogicalSize(gui::Size& sz);

    /// @brief Returns the physical size (in mm) of the default display.
    /// @param sz Size structure to fill.
    static void getDefaultPhysicalSize(gui::Size& sz);

    /// @brief Returns the maximum pixel resolution of the default display.
    /// @param maxW Filled with the maximum width in device pixels.
    /// @param maxH Filled with the maximum height in device pixels.
    static void getDefaultMaxResolution(size_t& maxW, size_t& maxH);

    /// @brief Returns the logical and physical information for the default display.
    /// @param info Info structure to fill.
    static void getDefaultInfo(Display::Info& info);

    /// @brief Returns the HiDPI backing scale factor of the default display.
    /// @return Scale factor (e.g. 2.0 for Retina displays).
    static float getDefaultBackingScaleFactor();

    /// @brief Returns the screen scale factor of the default display.
    /// @return Screen scale factor.
    static float getDefaultScreenScaleFactor();

    /// @brief Returns the backing scale factor and fills the maximum resolution.
    /// @param maxW Filled with the maximum device width.
    /// @param maxH Filled with the maximum device height.
    /// @return Backing scale factor.
    static float getDefaultScaleFactorAndMaxResolution(size_t& maxW, size_t& maxH);

    /// @brief Returns the number of connected displays.
    /// @return Display count.
    static size_t getNoOfDisplays();

    /// @brief Fills a vector with information about all connected displays.
    /// @param allDisplays Vector to populate with per-display Info structures.
    static void getInfoForAll(cnt::SafeFullVector<Display::Info>& allDisplays);

    /// @brief Returns the horizontal logical-pixel-to-millimetre ratio.
    /// @return Ratio value.
    static double getDefaultLogicalPixelToMmHRatio();

    /// @brief Returns the vertical logical-pixel-to-millimetre ratio.
    /// @return Ratio value.
    static double getDefaultLogicalPixelToMmVRatio();

    /// @brief Returns the dots-per-inch value for the default display.
    /// @return DPI value.
    static double getDefaultDPI();
};

} //namespace gui
