// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DrawableButtonIntern.h
 * @brief Internal drawable button used for Apply and Restore actions in property editors.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/DrawableButton.h>
#include <gui/IDrawDelegate.h>

namespace gui
{

/// @brief A DrawableButton variant used internally by property editors with Apply and Restore button types.
class NATGUI_API DrawableButtonIntern : public DrawableButton
{
public:
    /// @brief Enumerates the visual roles this internal button can fulfil.
    enum class Type : unsigned char {Apply, Restore};
private:
    Type _type; ///< The functional type (Apply or Restore) of this button.
//    td::BYTE _checked = 0; //To do
protected:
    /// @brief Measures the preferred size of this button.
    /// @param cell Cell info structure to populate with size constraints.
    void measure(CellInfo&) override;
    /// @brief Re-measures this button after layout changes.
    /// @param cell Cell info structure to update.
    void reMeasure(CellInfo&) override;

    /// @brief Draws the button icon for the current type within the given rectangle.
    /// @param rect The rectangle region to draw into.
    void onDraw(const gui::Rect& rect) override;
    /// @brief Default constructor. Constructs an uninitialised internal button.
    DrawableButtonIntern();
    /// @brief Returns the preferable width for a given height and minimum width constraint.
    /// @param forHeight The height for which the preferred width is calculated.
    /// @param minWidth The minimum acceptable width.
    /// @return The preferred width in pixels.
    td::UINT2 getPreferableWidth(td::UINT2 forHeight, td::UINT2 minWidth) const override;
public:
//    void setTag(td::UINT4 tag);
//    td::UINT4 getTag() const;
//    void setChecked(bool checked);
//    bool isChecked() const;

    /// @brief Constructs an internal button of the specified type.
    /// @param type The functional type (Apply or Restore) for this button.
    DrawableButtonIntern(DrawableButtonIntern::Type type);
    /// @brief Destructor.
    ~DrawableButtonIntern();
    /// @brief Returns the functional type of this button.
    /// @return The Type value (Apply or Restore).
    Type getType() const;
    /// @brief Returns the GUI object type identifier for this button.
    /// @return ObjType::DrawableButtonIntern.
    gui::ObjType getObjType() const override { return ObjType::DrawableButtonIntern;}
};

}//namespace gui
