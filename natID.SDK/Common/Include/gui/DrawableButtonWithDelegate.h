// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DrawableButtonWithDelegate.h
 * @brief Drawable button that delegates its rendering to an IDrawDelegate.
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
/// @brief A DrawableButton whose visual content is provided by an external IDrawDelegate.
class NATGUI_API DrawableButtonWithDelegate : public DrawableButton
{
public:
    /// @brief Rendering behaviour modes for this button.
    enum class Type : unsigned char {Apply, Restore, DelegateDraw};
private:
    gui::IDrawDelegate* _pDelegate = nullptr; ///< External delegate responsible for drawing the button.
    //td::UINT4 _tag = 0;
//    Type _type;
    td::BYTE _checked = 0; ///< Non-zero when the button is in the checked/pressed state.

protected:
//    void getMinSize(gui::Size& sz) const override;

    /// @brief Performs initial measurement using the delegate's minimum size.
    /// @param cellInfo Cell layout information to populate.
    void measure(CellInfo&) override;

    /// @brief Delegates the draw call to the registered IDrawDelegate.
    /// @param rect The dirty rectangle that needs to be redrawn.
    void onDraw(const gui::Rect& rect) override;

    DrawableButtonWithDelegate(const DrawableButtonWithDelegate&) = delete;
    DrawableButtonWithDelegate& operator =(const DrawableButtonWithDelegate&) = delete;

    /// @brief Default constructor used internally.
    DrawableButtonWithDelegate();

    //td::UINT4 getTagID() const override;

    /// @brief Returns the preferred width of the button for a given height.
    /// @param forHeight  Available height in pixels.
    /// @param minWidth   Minimum acceptable width.
    /// @return Preferred width in pixels.
    td::UINT2 getPreferableWidth(td::UINT2 forHeight, td::UINT2 minWidth) const override;
public:
    /// @brief Sets the checked state of the button.
    /// @param checked True to mark the button as checked/selected.
    void setChecked(bool checked);

    /// @brief Returns whether the button is currently in the checked state.
    /// @return True if the button is checked.
    bool isChecked() const;

    /// @brief Constructs a button with an external draw delegate and a visual identifier.
    /// @param pDrawDelegate Pointer to the delegate that performs custom drawing.
    /// @param visualID      Application-defined identifier passed to the delegate.
    DrawableButtonWithDelegate(gui::IDrawDelegate* pDrawDelegatem, td::UINT4 visualID);

    /// @brief Destructor.
    ~DrawableButtonWithDelegate();

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::DrawableButtonWithDelegate.
    gui::ObjType getObjType() const override { return ObjType::DrawableButtonWithDelegate;}

};

}//namespace gui
