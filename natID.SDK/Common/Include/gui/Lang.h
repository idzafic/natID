// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Lang.h
    @brief Represents a supported language with a file extension and a human-readable description. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "natGUI.h"
#include <td/String.h>
#include "Types.h"

namespace gui
{

/// @brief Stores information about a single supported language, identified by its file extension.
class NATGUI_API Lang
{
private:
    td::String _extension; ///< File extension associated with the language (e.g. "en", "de").
    td::String _description; ///< Human-readable description of the language (e.g. "English").
public:
    /// @brief Default constructor; creates an empty Lang object.
    Lang();

    /// @brief Sets the extension and description of this language.
    /// @param ext File extension string identifying the language.
    /// @param desc Human-readable language description.
    void set(const td::StringExt& ext, const td::String& desc);

    /// @brief Returns the file extension associated with this language.
    /// @return Const reference to the extension string.
    const td::String& getExtension() const;

    /// @brief Returns the human-readable description of this language.
    /// @return Const reference to the description string.
    const td::String& getDescription() const;
};

} //namespace gui
