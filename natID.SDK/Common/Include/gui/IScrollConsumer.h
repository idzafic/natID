// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IScrollConsumer.h
    @brief Interface for objects that react to scroll events from a ViewScroller. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once

#include <td/Types.h>
#include <gui/ViewScroller.h>

namespace gui
{

/// @brief Abstract interface for objects that are notified when a ViewScroller changes its origin.
class IScrollConsumer
{
public:
    /// @brief Called when the scroller's origin (scroll position) changes.
    /// @param pScroller Pointer to the ViewScroller that generated the event.
    /// @param newOrigin The new scroll origin point after the scroll event.
    virtual void onScroller(gui::ViewScroller* pScroller, const gui::Point& newOrigin) = 0;
};

} //namespace gui
