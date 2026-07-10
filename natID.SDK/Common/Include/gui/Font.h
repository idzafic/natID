// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Font.h
    @brief Font creation, metrics, and glyph measurement utilities. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Types.h>
#include <cnt/SafeFullVector.h>
#include <cnt/PushBackVector.h>

namespace gui
{

#ifdef MU_WINDOWS
const char* const cg_DefFontName = "Times New Roman"; ///< Default font family name on Windows.
#else
#ifdef MU_LINUX
const char* const cg_DefFontName = "Liberation Serif"; ///< Default font family name on Linux.
#else
//MACOS
const char* const cg_DefFontName = "Times New Roman"; ///< Default font family name on macOS.
#endif
#endif

class DrawableString;
class FontHelper;

/// @brief Represents a typeface and size combination used for text rendering.
class NATGUI_API Font
{
    friend class FontHelper;

public:
    using GlyphID = td::UTF32; ///< Unicode code-point type used to identify individual glyphs.

    /// @brief Unit system for the font size value.
    enum class Unit : unsigned char {
        LogicalPixel=0, ///< Size expressed in logical (screen-independent) pixels.
        Point           ///< Size expressed in typographic points.
    };

    /// @brief Text style flags.
    enum class Style : unsigned char {
        Normal=0,   ///< Regular upright weight.
        Bold,       ///< Bold weight.
        Italic,     ///< Italic style.
        BoldItalic  ///< Combined bold and italic.
    };

    /// @brief Text decoration markup types.
    enum class Markup : unsigned char {
        Subscript=0, ///< Lower the text below the baseline.
        Superscript, ///< Raise the text above the baseline.
        Underline,   ///< Draw an underline below the text.
        NA           ///< No markup.
    };

    /// @brief Well-known system font identifiers.
    enum class ID : unsigned char
        {   SystemSmallest=0,           ///< Smallest system font, normal weight.
            SystemSmallestBold,         ///< Smallest system font, bold weight.
            SystemSmallestItalic,       ///< Smallest system font, italic style.
            SystemSmallestBoldItalic,   ///< Smallest system font, bold-italic style.
            SystemSmaller,              ///< Smaller-than-normal system font.
            SystemSmallerBold,          ///< Smaller system font, bold weight.
            SystemSmallerItalic,        ///< Smaller system font, italic style.
            SystemSmallerBoldItalic,    ///< Smaller system font, bold-italic style.
            SystemNormal,               ///< Normal-size system font.
            SystemBold,                 ///< Normal system font, bold weight.
            SystemItalic,               ///< Normal system font, italic style.
            SystemBoldItalic,           ///< Normal system font, bold-italic style.
            SystemLarger,               ///< Larger-than-normal system font.
            SystemLargerBold,           ///< Larger system font, bold weight.
            SystemLargerItalic,         ///< Larger system font, italic style.
            SystemLargerBoldItalic,     ///< Larger system font, bold-italic style.
            SystemLargest,              ///< Largest system font.
            SystemLargestBold,          ///< Largest system font, bold weight.
            SystemLargestItalic,        ///< Largest system font, italic style.
            SystemLargestBoldItalic,    ///< Largest system font, bold-italic style.
            SystemControl,              ///< Font used for standard controls.
            SystemControlTitle,         ///< Font used for control titles.
            Custom,                     ///< User-defined custom font.
            NA                          ///< Uninitialized/invalid font identifier.
        };

    /// @brief Typographic measurement data for a font.
    typedef struct _metrics
    {
        float ascender;          ///< Distance from baseline to the top of the tallest glyph.
        float descender;         ///< Distance from baseline to the bottom of the deepest glyph (negative).
        float xHeight;           ///< Height of the lowercase letter 'x'.
        float capHeight;         ///< Height of an uppercase letter.
        float defaultLineHeight; ///< Recommended distance between consecutive baselines.
        float leading;           ///< Extra space added between lines.
    }Metrics;

    /// @brief Per-glyph layout metrics.
    typedef struct _glyphMetrics
    {
        gui::Point bottomLeft;    ///< Bottom-left corner of the glyph bounding box.
        gui::Size boundingSize;   ///< Width and height of the glyph bounding box.
        gui::Size advancement;    ///< Horizontal and vertical advance after drawing this glyph.
    } GlyphMetrics;

protected:
    td::String _familyName; ///< Font family name string (e.g. "Arial").
    float _size;            ///< Nominal font size in the chosen unit.
    td::UINT2 _customID = 0; ///< Application-assigned identifier for custom fonts.
    ID _id = ID::NA;         ///< System font identifier; ID::Custom for user-created fonts.
    Style _style = Style::Normal; ///< Current style flags.
    td::BYTE _dynamic = 0;  ///< Non-zero if the font tracks system-preference changes dynamically.

    Font(const Font&) = delete;
    Font& operator =(const Font&) = delete;
public:
    /// @brief Default constructor. Creates an uninitialised font object.
    Font();

    // Move constructor
    /// @brief Move constructor. Transfers font resources from another instance.
    /// @param fnt Source font to move from (left in a valid but empty state).
    Font(Font&& fnt) noexcept;

    // Move assignment operator
    /// @brief Move assignment operator.
    /// @param fnt Source font to move from.
    /// @return Reference to this font.
    Font& operator=(Font&& fnt) noexcept;

    /// @brief Destructor. Releases native font resources.
    ~Font();

    /// @brief Creates a font by family name string object.
    /// @param fntName Font family name.
    /// @param size Nominal size in the given unit.
    /// @param style Desired style (normal, bold, italic, bold-italic).
    /// @param sizeUnit Unit system for the size value.
    /// @return True if the font was created successfully.
    bool create(const td::String& fntName, float size, gui::Font::Style style, gui::Font::Unit sizeUnit);

    /// @brief Creates a font by family name C-string.
    /// @param fntName Font family name.
    /// @param size Nominal size in the given unit.
    /// @param style Desired style.
    /// @param sizeUnit Unit system for the size value.
    /// @return True if the font was created successfully.
    bool create(const char* fntName, float size, gui::Font::Style style, gui::Font::Unit sizeUnit);

    /// @brief Returns the application-assigned custom identifier.
    /// @return Custom identifier value.
    td::UINT2 getCustomID() const;

    /// @brief Returns the style of the font.
    /// @return Style enum value.
    Style getStyle() const;

    /// @brief Returns the system font identifier.
    /// @return ID enum value.
    ID getType() const;

    /// @brief Returns whether the font has bold weight.
    /// @return True if bold.
    bool isBold() const;

    /// @brief Returns whether the font is italic.
    /// @return True if italic.
    bool isItalic() const;

    /// @brief Returns the font family name.
    /// @return Const reference to the family name string.
    const td::String& getFamilyName() const;

    /// @brief Returns the nominal font size in its configured unit.
    /// @return Font size.
    float getSize() const;

    /// @brief Returns the font size scaled to the current display's backing scale.
    /// @return Scaled font size.
    float getScaledSize() const;

    /// @brief Marks the font as dynamic so it follows system font preference changes.
    void setDynamic();

    /// @brief Returns whether the font is in dynamic mode.
    /// @return True if dynamic.
    bool isDynamic() const;

    /// @brief Returns whether the font has been successfully initialised.
    /// @return True if the font is ready for use.
    bool isOk() const;

    /// @brief Returns whether the font is a custom (non-system) font.
    /// @return True if custom.
    bool isCustom() const;

    //metrics
    /// @brief Fills a Metrics structure with typographic measurements for this font.
    /// @param metrics Metrics structure to fill.
    void getMetrics(Metrics& metrics) const;

    /// @brief Returns layout metrics for a single glyph.
    /// @param glyph Unicode code point of the glyph.
    /// @param glyphMetrics GlyphMetrics structure to fill.
    void getGlyphMetrics(GlyphID glyph, GlyphMetrics& glyphMetrics) const;

    /// @brief Returns layout metrics for a sequence of glyphs.
    /// @param pGlyphs Pointer to an array of glyph code points.
    /// @param nGlyphs Number of glyphs in the array.
    /// @param cntMetrics Vector to receive one GlyphMetrics entry per glyph.
    void getGlyphMetrics(const GlyphID* pGlyphs, size_t nGlyphs, cnt::PushBackVector<GlyphMetrics>& cntMetrics) const;

    //strin dymensions with this font
    /// @brief Measures the rendered size of a C-string using this font.
    /// @param pStr Pointer to the UTF-8 string.
    /// @param strLen Number of bytes in the string.
    /// @param resultingSize Size structure filled with width and height.
    void measureString(const char* pStr, size_t strLen, gui::Size& resultingSize) const;

    /// @brief Measures the rendered size of a String object using this font.
    /// @param str String to measure.
    /// @param resultingSize Size structure filled with width and height.
    void measureString(const td::String& str, gui::Size& resultingSize) const;

    /// @brief Estimates the average rendered size for a given number of characters.
    /// @param nChars Number of characters.
    /// @param fontID System font identifier.
    /// @param resultingSize Size structure filled with estimated width and height.
    static void getAverageSize(size_t nChars, gui::Font::ID fontID, gui::Size& resultingSize);

    /// @brief Returns the line height for a system font.
    /// @param fontID System font identifier.
    /// @return Line height in logical pixels.
    static float getHeight(gui::Font::ID fontID);

    /// @brief Measures the space needed for N characters using a system font.
    /// @param nChars Number of characters.
    /// @param fontID System font identifier.
    /// @param resultingSize Size structure filled with the estimated dimensions.
    static void measureNChars(size_t nChars, gui::Font::ID fontID, gui::Size& resultingSize);

    /// @brief Converts a Style enum value to its string representation.
    /// @param style Style to convert.
    /// @return C-string name of the style.
    static const char* toString(gui::Font::Style style);

    /// @brief Converts a Markup enum value to its string representation.
    /// @param markup Markup to convert.
    /// @return C-string name of the markup.
    static const char* toString(gui::Font::Markup markup);

    /// @brief Returns all available system font family names.
    /// @return Const reference to the vector of family names.
    static const cnt::SafeFullVector<td::String>& getSystemFamilyNames();

    /// @brief Returns all available monospaced system font family names.
    /// @return Const reference to the vector of monospaced family names.
    static const cnt::SafeFullVector<td::String>& getSystemMonospacedFamilyNames();

    //returns system font height
    /// @brief Returns the height of the default system font.
    /// @return Height in logical pixels.
    static float getSystemHeight();
};


} //namespace gui
