// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ToolBarButton.h
    @brief Drawable button used to render a single toolbar item. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "DrawableButton.h"

namespace gui
{

class ToolBar;
class ToolBarItem;

/// @brief A button control that renders and interacts with a ToolBarItem on a non-native toolbar.
class NATGUI_API ToolBarButton : public DrawableButton
{
private:
    ToolBarItem* _pTBI = nullptr; ///< Pointer to the associated toolbar item descriptor.
//    int _width = 0;
protected:
    /// @brief Measures the preferred size of the toolbar button.
    /// @param ci CellInfo to populate.
    void measure(CellInfo&) override;

    /// @brief Re-measures the toolbar button after a size change.
    /// @param ci CellInfo to update.
    void reMeasure(CellInfo&) override;

    /// @brief Draws the toolbar button within the given rectangle.
    /// @param rect Rectangle in which to render the button.
    void onDraw(const gui::Rect& rect) override;

    ToolBarButton(const ToolBarButton&) = delete;
    ToolBarButton& operator =(const ToolBarButton&) = delete;
public:
    /// @brief Constructs a toolbar button linked to the given toolbar item.
    /// @param pTBI Pointer to the ToolBarItem this button represents.
    ToolBarButton(ToolBarItem* pTBI);

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::ToolBarButton.
    gui::ObjType getObjType() const override { return ObjType::ToolBarButton;}

    /// @brief Returns a pointer to the associated toolbar item.
    /// @return Pointer to the ToolBarItem.
    ToolBarItem* getToolBarItem();

    /// @brief Returns a pointer to the parent toolbar.
    /// @return Pointer to the ToolBar.
    ToolBar* getToolBar();

    /// @brief Returns the icon size in device pixels as determined by the parent toolbar.
    /// @return Icon size in pixels.
    int getIconSizeInPixels() const;
};

}
