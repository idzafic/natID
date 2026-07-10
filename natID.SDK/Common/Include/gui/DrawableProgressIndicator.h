// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DrawableProgressIndicator.h
 * @brief Custom-drawn progress indicator supporting rectangular and circular styles.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once

#include <gui/Shape.h>
#include <gui/Font.h>
#include <gui/DrawableString.h>

namespace gui
{

/// @brief A non-control progress indicator rendered directly via the drawing API.
class NATGUI_API DrawableProgressIndicator
{
public:
    /// @brief Visual style of the progress indicator.
    enum  class Type : unsigned char {NA=0, Rect, Circle, FillCircle};

protected:
    gui::Shape _shapeEmpty; ///< Shape used to represent the unfilled (empty) portion of the indicator.
    gui::Shape _shapeFull;  ///< Shape used to represent the filled (complete) portion of the indicator.
    gui::DrawableString _str; ///< Drawable string used to render the percentage or label text.
    gui::Size _size;          ///< Overall bounding size of the indicator.
    double _progres;          ///< Current progress value in the range [0, 1].
    gui::Font* _pFont = nullptr; ///< Custom font used for label text, or nullptr to use _fontID.
    float _lineWidth = 3.0f;     ///< Stroke width used when drawing the indicator outlines.
    gui::Font::ID _fontID = gui::Font::ID::NA; ///< System font identifier used when _pFont is nullptr.
    Type _type;                ///< Rendering style of this indicator.
    td::ColorID _colorEmpty;   ///< Color of the unfilled portion.
    td::ColorID _colorFull;    ///< Color of the filled portion.
    td::BYTE _prevDisplayed = 255; ///< Previously rendered progress value (0-100), used to avoid redundant redraws.
    //td::BYTE _systemColors = 1;
protected:
    DrawableProgressIndicator(const DrawableProgressIndicator& pi) = delete;
    DrawableProgressIndicator& operator=(const DrawableProgressIndicator&) = delete;

    /// @brief Builds the internal shapes based on the current size and type.
    void createShapes();
public:
    /// @brief Constructs the indicator with the given size, style, and system font.
    /// @param sz    Bounding size of the indicator.
    /// @param type  Visual style (Rect, Circle, or FillCircle).
    /// @param fntID System font identifier for the label text.
    DrawableProgressIndicator(const gui::Size& sz, Type type, gui::Font::ID fntID = gui::Font::ID::SystemNormal);

    /// @brief Constructs the indicator with the given size, style, and custom font.
    /// @param sz    Bounding size of the indicator.
    /// @param type  Visual style (Rect, Circle, or FillCircle).
    /// @param pFont Pointer to a custom font for the label text.
    DrawableProgressIndicator(const gui::Size& sz, Type type, gui::Font* pFont);

    // Move constructor
    /// @brief Move constructor.
    /// @param pi Source indicator to move from.
    DrawableProgressIndicator(DrawableProgressIndicator&& pi) noexcept;

    // Move assignment operator
    /// @brief Move assignment operator.
    /// @param dpi Source indicator to move from.
    /// @return Reference to this indicator.
    DrawableProgressIndicator& operator=(DrawableProgressIndicator&& dpi) noexcept;
//
//    ~DrawableProgressIndicator();

    /// @brief Sets the stroke width used when drawing indicator outlines.
    /// @param lw Line width in points.
    void setLineWidth(float lw);

    /// @brief Sets the colors for the empty and filled portions of the indicator.
    /// @param colorEmpty Color of the unfilled portion.
    /// @param _colorFull Color of the filled portion.
    void setColors(td::ColorID colorEmpty, td::ColorID _colorFull);

    /// @brief Draws the indicator at the specified top-left position.
    /// @param tl Top-left drawing origin in current coordinate space.
    void draw(const gui::Point& tl);

    //value has to be between 0 and 1
    /// @brief Sets the progress value.
    /// @param value Progress ratio in [0.0, 1.0].
    void setValue(double value);

    /// @brief Returns the current progress value.
    /// @return Progress ratio in [0.0, 1.0].
    double getValue() const;

    /// @brief Returns the bounding size of the indicator.
    /// @return Reference to the indicator's size.
    const gui::Size& getSize() const;
};

} //namepace gui
