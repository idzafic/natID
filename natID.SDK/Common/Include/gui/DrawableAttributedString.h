// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DrawableAttributedString.h
 * @brief Attributed string that can be measured and drawn in a GUI context.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once
#include <gui/Types.h>
#include <gui/Font.h>
#include <td/ColorID.h>

namespace gui
{

/// @brief Describes a single tab-stop entry used for text layout.
using TabStop = struct Tabstop {
    int start;  ///< Starting position of the tab stop.
    int nSteps; ///< Number of steps for this tab stop.
    int delta;  ///< Pixel distance between steps.
};

/// @brief An attributed string wrapper that supports rendering with per-segment color and style.
class NATGUI_API DrawableAttributedString
{
public:
    /// @brief Describes a contiguous run of glyphs that share a single color.
    using ColorRange = struct ClrRange
    {
        td::UINT4 len; //in glyphs ///< Number of glyphs in this color run.
        td::ColorID color; ///< Color applied to the glyphs in this run.
    };
//    enum class LineBreak : unsigned char {Spase, Word}; //Breaks on spaces for now
private:
    AttributedString _natString; ///< Native platform attributed string handle.

    DrawableAttributedString(const DrawableAttributedString&) = delete;
    DrawableAttributedString& operator =(const DrawableAttributedString&) = delete;
public:
    /// @brief Default constructor; produces an uninitialised attributed string.
    DrawableAttributedString();
//    DrawableAttributedString(const char*);
//    DrawableAttributedString(const td::String& str);
    /// @brief Destructor; releases the underlying native attributed string.
    ~DrawableAttributedString();
//
    /// @brief Returns whether the attributed string has been initialised.
    /// @return True if the string is ready for use.
    bool isInitialized() const;
//    void operator = (const char* pStr);
//    void operator = (const td::String& str);

//    void measure(gui::Font::ID, gui::Size& resultingSize) const;
//    void measure(gui::Font::ID, CoordType maxWidth, gui::Size& resultingSize) const;

//    static void measureNChars(size_t nChars, gui::Font::ID fontID, gui::Size& resultingSize);

    /// @brief Draws the attributed string inside the given rectangle.
    /// @param r Target bounding rectangle for rendering.
    void draw(const gui::Rect& r) const;

    /// @brief Creates the attributed string from plain text with per-segment color information.
    /// @param str       The plain-text content.
    /// @param range     Array of ColorRange segments describing per-run colors.
    /// @param nSegments Number of entries in the range array.
    /// @param styleID   Optional style identifier for additional formatting.
    void create(const td::String& str, DrawableAttributedString::ColorRange* range, size_t nSegments, td::UINT2 styleID = 0);

//    void draw(const gui::Rect& r) const;
    //gui::NatString getNatStrPtr();

    /// @brief Returns the underlying native attributed string handle.
    /// @return Native attributed string reference.
    const gui::AttributedString getNatStrPtr() const;

    /// @brief Converts the attributed string content to a plain td::String.
    /// @return Plain-text representation of the attributed string.
    td::String toString() const;

    /// @brief Creates a collection of native attributed string blocks from an HTML string.
    /// @param strHTML      Input HTML-formatted string.
    /// @param natStrings   Output vector that receives the resulting blocks.
    /// @param width        Available width for layout.
    /// @param firstHeight  Output height of the first block.
    /// @param lastHeight   Output height of the last block.
    /// @param pFont        Font to apply, or nullptr for the default font.
    /// @param clrID        Text color identifier.
    /// @param hAlign       Horizontal text alignment.
    static void createNatAttributedStrings(const td::String& strHTML, cnt::PushBackVector<gui::AttributedStringBlock>& natStrings, gui::CoordType width, gui::CoordType& firstHeight, gui::CoordType& lastHeight, const gui::Font* pFont, td::ColorID clrID, td::TextAlignment hAlign);

    /// @brief Updates font, color, and alignment on an existing collection of attributed string blocks.
    /// @param natStrings Output vector of blocks to update.
    /// @param pFont      New font to apply.
    /// @param clrID      New text color identifier.
    /// @param hAlign     New horizontal text alignment.
    static void updateNatAttributedStrings(cnt::PushBackVector<gui::AttributedStringBlock>& natStrings, const gui::Font* pFont, td::ColorID clrID, td::TextAlignment hAlign);

    /// @brief Measures the size required to render a native attributed string within a given width.
    /// @param natStr         The native attributed string to measure.
    /// @param width          Maximum available width.
    /// @param resultingSize  Output size needed to render the string.
    static void measure(gui::AttributedString natStr, gui::CoordType width, gui::Size& resultingSize);

    /// @brief Draws a native attributed string inside the given rectangle.
    /// @param natStr Native attributed string to draw.
    /// @param r      Target bounding rectangle.
    static void draw(const AttributedString natStr, const gui::Rect& r);

    /// @brief Sanitises an HTML string for use with the attributed string API.
    /// @param strHTML HTML string to adjust in-place.
    static void adjustHTMLString(td::String& strHTML);

    /// @brief Releases the resources held by a native attributed string handle.
    /// @param natStr Handle to release; set to null after release.
    static void release(AttributedString& natStr);

//    static void setStyle(td::UINT2 styleID, const td::String fontName, float fontSize, const TabStop* pTabStops = nullptr, size_t nTabStops = 0);
//
//    static void setStyle(td::UINT2 styleID, float fontSize, const td::String& fontName = "", const TabStop* pTabStops = nullptr, size_t nTabStops = 0);
};

} //namepace gui
