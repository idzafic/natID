// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Button.h
 * @brief Clickable button control with support for text, image, and context-menu modes.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Control.h"
#include "ContextMenu.h"

namespace gui
{

class Image;
class Symbol;

/// @brief A clickable button control that can display text, an image, or a symbol.
class NATGUI_API Button : public Control
{
public:
    /// @brief Visual style of the button.
    enum class Type : td::BYTE {Normal=0, Default, Critical, Constructive, Destructive};

    /// @brief Indicates what content the button displays.
    enum class ContentType : td::BYTE {Text=0, Image, Symbol};
private:
    union
    {
        const Image* _pImage;   ///< Pointer to the image displayed on the button.
        const Symbol* _pSymbol; ///< Pointer to the symbol displayed on the button.
    };
    std::function<void()> _onClick; ///< Click event callback.
    Type _type = Type::Normal; ///< Current button style type.
    ContentType _contentType = ContentType::Text; ///< Current content type displayed.
    td::BYTE _menuID = 0; ///< Context menu ID shown on click (0 means no menu).
    ContextMenu::Location _preferableMenuLocation = ContextMenu::Location::Top; ///< Preferred location for the context menu.

protected:
    /// @brief Measures the button's preferred size.
    /// @param ci Cell info to populate.
    void measure(CellInfo&) override;

    /// @brief Re-measures the button after a layout change.
    /// @param ci Cell info to update.
    void reMeasure(CellInfo&) override;

    Button(const Button&) = delete;
    Button& operator =(const Button&) = delete;
public:
    Button(); //creates txt button with empty title
    /// @brief Constructs a text button with a C-string label.
    /// @param lbl Button label.
    Button(const char* lbl);

    /// @brief Constructs a text button with a String label.
    /// @param lbl Button label.
    Button(const td::String& lbl);

    /// @brief Constructs a text button with a label and tooltip.
    /// @param lbl Button label.
    /// @param toolTip Tooltip text.
    Button(const td::String& lbl, const td::String& toolTip);

    /// @brief Constructs an image button with an optional tooltip.
    /// @param img Pointer to the image to display.
    /// @param toolTip Tooltip text.
    Button(const gui::Image* img, const td::String& toolTip = td::String());
//    ~Button();

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::Button.
    gui::ObjType getObjType() const override { return ObjType::Button;}

    /// @brief Marks the button as the default action button.
    void setAsDefault();

    /// @brief Marks the button as a critical-action button.
    void setAsCritical();

    /// @brief Sets the button's text label.
    /// @param title New label text.
    void setTitle(const td::String& title) override;

    /// @brief Sets the visual style of the button.
    /// @param type The button style type.
    void setType(Button::Type type);

    /// @brief Sets the image displayed on the button.
    /// @param img Pointer to the image.
    void setImage(const gui::Image* img);

    /// @brief Makes the button appear flat (no border).
    void setFlat();

    /// @brief Makes the button appear circular.
    void setCircular();

    /// @brief Shrinks the button to its minimum content size.
    void setToMinSize();

    /// @brief Sets the checked state of a toggle button.
    /// @param bCheck True to check, false to uncheck.
    void setChecked(bool bCheck);

    /// @brief Returns the current visual style type.
    /// @return The button's Type.
    Type getType() const;

    /// @brief Returns the content type currently displayed.
    /// @return The button's ContentType.
    ContentType getContentType() const;

    /// @brief Returns the image associated with this button.
    /// @return Pointer to the image, or nullptr.
    const Image* getImage() const;

    /// @brief Returns the symbol associated with this button.
    /// @return Pointer to the symbol, or nullptr.
    const Symbol* getSymbol() const;

    //use context menu ID to show menu on this button
    //in this case onClick will not be called, instead onAction event will be generated
    /// @brief Associates a context menu with the button.
    /// @param nenuID Context menu identifier.
    /// @param preferableMenuLocation Preferred display location for the menu.
    void setMenuID(td::BYTE nenuID, ContextMenu::Location preferableMenuLocation = ContextMenu::Location::Top);

    /// @brief Returns the associated context menu ID.
    /// @return The context menu ID.
    td::BYTE getMenuID() const;

    /// @brief Returns the preferred location for the context menu.
    /// @return The preferred menu location.
    ContextMenu::Location getMenuPreferableLocation() const;

    /// @brief Returns the registered click callback.
    /// @return Reference to the click handler function.
    const std::function<void()>& getClickHandler() const;

    /// @brief Registers a callback to be invoked when the button is clicked.
    /// @param fnToCall The callback function.
    void onClick(const std::function<void()>& fnToCall);
};
}
