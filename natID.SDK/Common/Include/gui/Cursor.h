// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Cursor.h
    @brief Declares the Cursor class and its type enumeration for mouse cursor shapes. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/natGUI.h>

namespace gui
{

/// @brief Provides the enumeration of available mouse cursor shapes used by the GUI framework.
class NATGUI_API Cursor
{
public:
    /// @brief Enumerates the cursor shapes that can be set for GUI controls and windows.
    enum class Type : char
    {
        None = 0,          ///< No cursor (invisible or unset).
        Default,           ///< The platform default arrow cursor.
        Adder,             ///< Cross-hair / add cursor.
        Move,              ///< Four-directional move cursor.
        ResizeUp,          ///< Resize cursor pointing upward.
        ResizeDown,        ///< Resize cursor pointing downward.
        ResizeVertical,    ///< Bidirectional vertical resize cursor.
        ResizeLeft,        ///< Resize cursor pointing left.
        ResizeRight,       ///< Resize cursor pointing right.
        ResizeHorizontal,  ///< Bidirectional horizontal resize cursor.
        OpenHand,          ///< Open-hand (grab) cursor.
        ClosedHand,        ///< Closed-hand (grabbing) cursor.
        Finger,            ///< Pointing finger / link cursor.
        Invisible,         ///< Completely hidden cursor.
        Custom             ///< Application-defined custom cursor.
    };
//    static void set(Cursor::Type ct);
//    static void hide();
//    void hideUntilMouseMoves(bool bHide = true);
//    static void unhide();
};



} //namespace gui
