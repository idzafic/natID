// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Types.h>
#include <cnt/SafeFullVector.h>

namespace gui
{

class NATGUI_API Display
{
    static float _backingScaleFactor;
    static float _scaleResolution;
    static double _logigalPixelToMmHRatio;
    static double _logigalPixelToMmVRatio;
    static double _dpi;
public:
    enum class ColorMode : td::BYTE {Dark=0, Light, Any};
    
    typedef struct _DI
    {
        gui::Size logicalSize; //logical pixel (this is not device pixel)
        gui::Size physicalSize; //mm
    }Info;
    static void getDefaultLogicalSize(gui::Size& sz);
    static void getDefaultPhysicalSize(gui::Size& sz);
    static void getDefaultMaxResolution(size_t& maxW, size_t& maxH);
    static void getDefaultInfo(Display::Info& info);
    static float getDefaultBackingScaleFactor();
    static float getDefaultScreenScaleFactor();
    static float getDefaultScaleFactorAndMaxResolution(size_t& maxW, size_t& maxH);
    
    static size_t getNoOfDisplays();
    static void getInfoForAll(cnt::SafeFullVector<Display::Info>& allDisplays);
    static double getDefaultLogicalPixelToMmHRatio();
    static double getDefaultLogicalPixelToMmVRatio();
    static double getDefaultDPI();
};

} //namespace gui
