// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DrawableString.h
 * @brief Platform-native string that can be measured and drawn in a GUI context.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once
#include <gui/Types.h>
#include <gui/Font.h>

namespace gui
{

/// @brief A lightweight wrapper around a native string object that supports measurement and rendering.
class NATGUI_API DrawableString
{
public:
//    enum class LineBreak : unsigned char {Spase, Word}; //Breaks on spaces for now
private:
    NatString _natString; ///< Underlying native platform string handle.

private:
    DrawableString(const DrawableString&) = delete;
    DrawableString& operator=(const DrawableString&) = delete;
public:
    /// @brief Default constructor; produces an uninitialised drawable string.
    DrawableString();

    /// @brief Constructs a drawable string from a null-terminated C string.
    /// @param str Null-terminated UTF-8 source string.
    DrawableString(const char*);

    /// @brief Constructs a drawable string from a td::String.
    /// @param str Source string.
    DrawableString(const td::String& str);

    // Move constructor
    /// @brief Move constructor.
    /// @param ds Source DrawableString to move from.
    DrawableString(DrawableString&& ds) noexcept;

    // Move assignment operator
    /// @brief Move assignment operator.
    /// @param ds Source DrawableString to move from.
    /// @return Reference to this object.
    DrawableString& operator=(DrawableString&& ds) noexcept;

    /// @brief Destructor; releases the underlying native string.
    ~DrawableString();

    /// @brief Returns whether the string has been initialised with content.
    /// @return True if the string is ready for measurement and drawing.
    bool isInitialized() const;

    /// @brief Assigns a new value from a null-terminated C string.
    /// @param pStr Null-terminated UTF-8 source string.
    void operator = (const char* pStr);

    /// @brief Assigns a new value from a td::String.
    /// @param str Source string.
    void operator = (const td::String& str);

    /// @brief Measures the rendered size of this string using a system font ID.
    /// @param fontID        System font to use for measurement.
    /// @param resultingSize Output size that fits the rendered string.
    void measure(gui::Font::ID, gui::Size& resultingSize) const;

    /// @brief Measures the rendered size of this string constrained to a maximum width.
    /// @param fontID        System font to use for measurement.
    /// @param maxWidth      Maximum available width; text wraps beyond this value.
    /// @param resultingSize Output size that fits the rendered string.
    void measure(gui::Font::ID, CoordType maxWidth, gui::Size& resultingSize) const;

    /// @brief Measures the rendered size of this string using a custom font.
    /// @param pFont         Pointer to the font to use for measurement.
    /// @param resultingSize Output size that fits the rendered string.
    void measure(const gui::Font* pFont, gui::Size& resultingSize) const;

    /// @brief Measures the rendered size of this string constrained to a maximum width, using a custom font.
    /// @param pFont         Pointer to the font to use for measurement.
    /// @param maxWidth      Maximum available width.
    /// @param resultingSize Output size that fits the rendered string.
    void measure(const gui::Font* pFont, CoordType maxWidth, gui::Size& resultingSize) const;

//    static void getSize(const gui::DrawableString& drawString, gui::Font::ID fontID, gui::Size& resultingSize);
//    void measure(gui::Font::ID fontID, gui::Size& resultingSize) const;
//    static void getSize(const gui::DrawableString& drawString, gui::Font::ID fontID, gui::CoordType maxWidth, gui::Size& resultingSize);
//    void measure(gui::Font::ID fontID, gui::CoordType maxWidth, gui::Size& resultingSize) const;

    /// @brief Measures the average rendered size of N characters using a system font.
    /// @param nChars        Number of characters to measure.
    /// @param fontID        System font identifier.
    /// @param resultingSize Output size for the N-character string.
    static void measureNChars(size_t nChars, gui::Font::ID fontID, gui::Size& resultingSize);

    //measureString -> measeure (moved from Font)
    /// @brief Measures a null-terminated C string using a system font.
    /// @param pStr          Null-terminated UTF-8 string.
    /// @param fontID        System font identifier.
    /// @param resultingSize Output rendered size.
    static void measure(const char* pStr, gui::Font::ID fontID, gui::Size& resultingSize);

    /// @brief Measures a td::String using a system font.
    /// @param str           String to measure.
    /// @param resultingSize Output rendered size.
    /// @param fontID        System font identifier (defaults to SystemNormal).
    static void measure(const td::String& str, gui::Size& resultingSize, gui::Font::ID fontID = gui::Font::ID::SystemNormal);

    /// @brief Draws this string inside a rectangle using a system font and color.
    /// @param r       Target bounding rectangle.
    /// @param fntID   System font identifier.
    /// @param clrID   Text color identifier.
    /// @param hAlign  Horizontal text alignment.
    /// @param vAlign  Vertical text alignment.
    /// @param ellips  Ellipsization strategy when text overflows.
    void draw(const gui::Rect& r, gui::Font::ID fntID, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left, td::VAlignment vAlign = td::VAlignment::Top, td::TextEllipsize ellips = td::TextEllipsize::End) const;

    /// @brief Draws this string at a point using a system font and color.
    /// @param pt    Top-left drawing origin.
    /// @param fntID System font identifier.
    /// @param clrID Text color identifier.
    void draw(const gui::Point& pt, gui::Font::ID fntID, td::ColorID clrID) const;

    /// @brief Draws this string inside a rectangle using a custom font and color.
    /// @param r       Target bounding rectangle.
    /// @param pFont   Pointer to the font to use.
    /// @param clrID   Text color identifier.
    /// @param hAlign  Horizontal text alignment.
    /// @param vAlign  Vertical text alignment.
    /// @param ellips  Ellipsization strategy when text overflows.
    void draw(const gui::Rect& r, const gui::Font* pFont, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left, td::VAlignment vAlign = td::VAlignment::Top, td::TextEllipsize ellips = td::TextEllipsize::End) const;

    /// @brief Draws this string at a point using a custom font and color.
    /// @param pt    Top-left drawing origin.
    /// @param pFont Pointer to the font to use.
    /// @param clrID Text color identifier.
    void draw(const gui::Point& pt, const gui::Font* pFont, td::ColorID clrID) const;

    /// @brief Draws a td::String inside a rectangle using a system font and color.
    /// @param txt    String to draw.
    /// @param r      Target bounding rectangle.
    /// @param fntID  System font identifier.
    /// @param clrID  Text color identifier.
    /// @param hAlign Horizontal text alignment.
    /// @param vAlign Vertical text alignment.
    /// @param ellips Ellipsization strategy.
    static void draw(const td::String& txt, const gui::Rect& r, gui::Font::ID fntID, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left, td::VAlignment vAlign = td::VAlignment::Top,  td::TextEllipsize ellips = td::TextEllipsize::End);

    /// @brief Draws a td::String at a point using a system font and color.
    /// @param txt   String to draw.
    /// @param pt    Top-left drawing origin.
    /// @param fntID System font identifier.
    /// @param clrID Text color identifier.
    static void draw(const td::String& txt, const gui::Point& pt, gui::Font::ID fntID, td::ColorID clrID);

    /// @brief Draws a C string of given length inside a rectangle using a system font and color.
    /// @param pStr   Pointer to the character data.
    /// @param nChars Number of characters to draw.
    /// @param r      Target bounding rectangle.
    /// @param fntID  System font identifier.
    /// @param clrID  Text color identifier.
    /// @param hAlign Horizontal text alignment.
    /// @param vAlign Vertical text alignment.
    /// @param ellips Ellipsization strategy.
    static void draw(const char* pStr, size_t nChars, const gui::Rect& r, gui::Font::ID fntID, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left, td::VAlignment vAlign = td::VAlignment::Top,  td::TextEllipsize ellips = td::TextEllipsize::End);

    /// @brief Draws a C string of given length at a point using a system font and color.
    /// @param pStr   Pointer to the character data.
    /// @param nChars Number of characters to draw.
    /// @param pt     Top-left drawing origin.
    /// @param fntID  System font identifier.
    /// @param clrID  Text color identifier.
    static void draw(const char* pStr, size_t nChars, const gui::Point& pt, gui::Font::ID fntID, td::ColorID clrID);

    /// @brief Draws a td::String inside a rectangle using a custom font and color.
    /// @param txt    String to draw.
    /// @param r      Target bounding rectangle.
    /// @param pFont  Pointer to the font to use.
    /// @param clrID  Text color identifier.
    /// @param hAlign Horizontal text alignment.
    /// @param vAlign Vertical text alignment.
    /// @param ellips Ellipsization strategy.
    static void draw(const td::String& txt, const gui::Rect& r, const gui::Font* pFont, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left,  td::VAlignment vAlign = td::VAlignment::Top, td::TextEllipsize ellips = td::TextEllipsize::End);

    /// @brief Draws a td::String at a point using a custom font and color.
    /// @param txt   String to draw.
    /// @param pt    Top-left drawing origin.
    /// @param pFont Pointer to the font to use.
    /// @param clrID Text color identifier.
    static void draw(const td::String& txt, const gui::Point& pt, const gui::Font* pFont, td::ColorID clrID);

    /// @brief Draws a C string of given length inside a rectangle using a custom font and color.
    /// @param pStr   Pointer to the character data.
    /// @param nChars Number of characters to draw.
    /// @param r      Target bounding rectangle.
    /// @param pFont  Pointer to the font to use.
    /// @param clrID  Text color identifier.
    /// @param hAlign Horizontal text alignment.
    /// @param vAlign Vertical text alignment.
    /// @param ellips Ellipsization strategy.
    static void draw(const char* pStr, size_t nChars, const gui::Rect& r, const gui::Font* pFont, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left,  td::VAlignment vAlign = td::VAlignment::Top, td::TextEllipsize ellips = td::TextEllipsize::End);

    /// @brief Draws a C string of given length at a point using a custom font and color.
    /// @param pStr   Pointer to the character data.
    /// @param nChars Number of characters to draw.
    /// @param pt     Top-left drawing origin.
    /// @param pFont  Pointer to the font to use.
    /// @param clrID  Text color identifier.
    static void draw(const char* pStr, size_t nChars, const gui::Point& pt, const gui::Font* pFont, td::ColorID clrID);

    /// @brief Draws an HTML-formatted string inside a rectangle using a custom font and color.
    /// @param txtHTML HTML-formatted source string.
    /// @param r       Target bounding rectangle.
    /// @param pFont   Pointer to the base font to use.
    /// @param clrID   Default text color identifier.
    /// @param hAlign  Horizontal text alignment.
    static void drawHTML(const td::String& txtHTML, const gui::Rect& r, const gui::Font* pFont, td::ColorID clrID, td::TextAlignment hAlign = td::TextAlignment::Left);
//    static void drawHTML(const char* txtHTML, size_t nChars, const gui::Rect& r, const gui::Font* pFont);

    //gui::NatString getNatStrPtr();
    /// @brief Returns the underlying native string handle.
    /// @return Native string reference (platform-specific).
    const gui::NatString getNatStrPtr() const;

    /// @brief Converts the drawable string to a plain td::String.
    /// @return Plain-text representation.
    td::String toString() const;
};

} //namepace gui
