// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PropertyEditorTitle.h
 * @brief Title bar view for a property editor, containing a label with Apply and Restore buttons.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include "DrawableButtonIntern.h"
#include <gui/HorizontalLayout.h>

namespace gui
{

/// @brief Displays a title label alongside Apply and Restore action buttons for a property editor panel.
class NATGUI_API PropertyEditorTitle : public View
{

protected:
    Label _label;                   ///< Label displaying the title text of the property editor.
    DrawableButtonIntern _apply;    ///< Button that commits pending property changes.
    DrawableButtonIntern _restore;  ///< Button that reverts property changes to their last saved state.
    HorizontalLayout _layout;       ///< Horizontal layout manager arranging the label and buttons.
    //td::BYTE _autoUpdate;

protected:
    /// @brief Measures the preferred size of the title view.
    /// @param cell Cell info structure to populate with size constraints.
    void measure(CellInfo& cell) override;

    /// @brief Re-measures the title view after layout changes.
    /// @param cell Cell info structure to update.
    void reMeasure(CellInfo& cell) override;

    /// @brief Default constructor. Constructs an uninitialised property editor title.
    PropertyEditorTitle();
public:
    /// @brief Constructs a property editor title with the given label text and optional settings.
    /// @param label The title string to display.
    /// @param autoUpdate True if property values should update automatically without explicit Apply.
    /// @param hAlign Horizontal alignment of the label text; defaults to Center.
    PropertyEditorTitle(const td::String& label, bool autoUpdate = false, td::HAlignment hAlign = td::HAlignment::Center);
    /// @brief Destructor.
    ~PropertyEditorTitle();

    //void setAlignment(td::HAlignment hAlign);
};

} //namespace gui
