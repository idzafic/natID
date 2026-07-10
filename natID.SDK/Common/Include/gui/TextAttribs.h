// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file TextAttribs.h
    @brief Text attribute and styled range structures for rich-text rendering. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Font.h>
#include <cnt/Array.h>
#include <td/Color.h>

namespace gui
{

/// @brief Holds font markup, style, and color attributes for a run of text.
class TextAttribs
{
public:
    cnt::Array<Font::Markup, (size_t)gui::Font::Markup::NA> markups; ///< Array of font markup flags applied to the text.
    gui::Font::Style style = gui::Font::Style::Normal; ///< Font style (e.g. Normal, Bold, Italic).
    td::ColorID colorID = td::ColorID::SysText; ///< Color applied to the text run.

    /// @brief Default constructor; leaves all attributes at their defaults.
    TextAttribs()
    {
    }

    /// @brief Resets all attributes to their default values.
    void clean()
    {
        colorID = td::ColorID::SysText;
        style = gui::Font::Style::Normal;
        for(auto& e: markups)
        {
            e = gui::Font::Markup::NA;
        }
    }
};

/// @brief Extends TextAttribs with a character range, associating attributes with a specific span of text.
class TextAttribsRange : public TextAttribs
{
public:
    gui::Range range; ///< Character range within the text to which the attributes apply.

    /// @brief Default constructor; leaves all attributes and range at their defaults.
    TextAttribsRange()
    {
    }
};

} //namespace gui
