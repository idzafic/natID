// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IDrawDelegate.h
    @brief Interface for custom drawing delegates that render content for a control. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once
#include <gui/Types.h>
#include <gui/Control.h>
#include <td/String.h>

namespace gui
{

/// @brief Abstract interface for objects that perform custom drawing on behalf of a control.
class IDrawDelegate
{
public:
    /// @brief Returns the minimum size required for drawing the control's content.
    /// @param sz Output size structure to be filled with the minimum required dimensions.
    /// @param pCtrl Pointer to the control being measured.
    virtual void getMinSize(gui::Size& sz, const gui::Control* pCtrl) const = 0;

    /// @brief Performs custom drawing within the given rectangle for the specified control.
    /// @param rect The bounding rectangle in which to draw.
    /// @param pCtrl Pointer to the control requesting the drawing.
    virtual void draw(const gui::Rect& rect, gui::Control* pCtrl) = 0;
};

} //namepace gui
