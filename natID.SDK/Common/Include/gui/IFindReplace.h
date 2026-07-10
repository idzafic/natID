// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IFindReplace.h
    @brief Interface for find-and-replace dialog controllers. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <td/Types.h>

namespace gui
{
class TextEdit;

/// @brief Abstract interface for a find-and-replace UI component.
class IFindReplace
{
public:
    /// @brief Specifies the behaviour of the dialog when there is no active text edit.
    enum class InactiveMode : td::BYTE
    {
        Hide=0, ///< Hide the find-replace dialog when inactive.
        Freeze  ///< Keep the dialog visible but frozen when inactive.
    };

    /// @brief Associates a TextEdit with this find-replace controller.
    /// @param pTE Pointer to the TextEdit to search/replace in. Pass null to hide or disable the dialog.
    virtual void setTextEdit(TextEdit* pTE) = 0; //pTE can be null (hide/disable the dialog)
};

}  //namespace gui
