// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Layout.h
    @brief Base class for all layout managers that arrange controls within a view. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Control.h"

namespace gui
{

class View;
class BaseView;
class ViewHelper;

/// @brief Abstract base class for layout managers that arrange and size child controls within a view.
class NATGUI_API Layout : public Control
{
    friend class View;
    friend class BaseView;
    friend class ViewHelper;
protected:
    /// @brief Default constructor for use by derived layout classes.
    Layout();
    Layout(const Layout&) = delete;
    Layout& operator =(const Layout&) = delete;

    /// @brief Adds a child control to the specified view.
    /// @param pCtrl Pointer to the control to add.
    /// @param pView Pointer to the view that will host the control.
    void addCtrlToView(Control* pCtrl, View* pView) const;

    /// @brief Performs an initial measurement pass for a cell, called before the layout is frozen.
    /// @param cellInfo Cell information to populate with initial size data.
    virtual void initialMeasure(CellInfo&);
public:
    /// @brief Populates the given view with the layout's controls using cell and margin information.
    /// @param pView Pointer to the view to populate.
    /// @param cell Cell reference describing the available area.
    /// @param szMargins Margin sizes to apply around the content.
    virtual void populateView(View* pView, const Cell& cell, const Size& szMargins);

    /// @brief Shows or hides the entire layout and all its child controls.
    /// @param bHide True to hide the layout, false to show it.
    void hide(bool bHide);

    /// @brief Returns whether the layout is currently hidden.
    /// @return True if the layout is hidden.
    virtual bool isHidden() const override;
};

} //namespace gui
