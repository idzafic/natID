// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DialogView.h
 * @brief View subclass that serves as the root content view inside a Dialog window.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/HorizontalLayout.h>
#include <gui/VerticalLayout.h>
#include <gui/View.h>
#include <cnt/SafeFullVector.h>
#include <gui/Button.h>

namespace gui
{

class Dialog;

/// @brief Root view of a Dialog that routes button click events back to the owning Dialog.
class NATGUI_API DialogView : public View
{
    friend class Dialog;
protected:
    gui::Dialog* _pDialog; ///< Pointer to the Dialog that owns this view.
protected:
    //void onInitialAppearance() override final;
private:
    /// @brief Intercepts button clicks and forwards them to the owning Dialog.
    /// @param pButton Pointer to the button that was clicked.
    /// @return True if the click was handled, false otherwise.
    bool onClick(gui::Button* pButton) override;
    //bool estimateDesiredInitialSize(const gui::Size& initialSize, gui::Size& desiredSize) const override;
public:
    /// @brief Constructs a DialogView linked to the given Dialog.
    /// @param pDlg Pointer to the owning Dialog.
    DialogView(gui::Dialog* pDlg);
    //~DialogView();
};

} //namespace gui
