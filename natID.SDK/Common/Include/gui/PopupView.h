// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PopupView.h @brief Declares the PopupView class, a view-based popover that can host arbitrary content. */
//
//  Created by Izudin Dzafic on 20/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//
#pragma once
#include "View.h"

namespace gui
{

class IPopoverButton;
class Image;
class Symbol;
class PopupViewHelper;

/// @brief A view that acts as a popup (popover) attached to a button, displaying arbitrary content.
class NATGUI_API PopupView : public View
{
    friend class PopupViewHelper;

protected:
    /// @brief Indicates what kind of graphic is shown on the associated popover button.
    enum class SymbolType : td::BYTE {Image = 0, ///< The button shows an image.
                                      Symbol,     ///< The button shows a symbol.
                                      CustomDrawing ///< The button symbol is drawn with custom code.
                                     };
    IPopoverButton* _btnPopover = nullptr;  ///< Pointer to the button that triggers this popup view.
    Handle _popoverFrame = nullptr;         ///< Native handle to the popup window frame.
    Popover::Location _location = Popover::Location::AboveButton; ///< Location of the popup relative to the button.
    union{
        gui::Image* _pImage = nullptr; ///< Image shown on the button (when SymbolType::Image).
        gui::Symbol* _pSymbol;         ///< Symbol shown on the button (when SymbolType::Symbol).
    };
    SymbolType _symbolType = SymbolType::CustomDrawing; ///< Type of button graphic in use.
    bool _bDisabled = false; ///< Whether the popup is currently disabled.
private:
    const gui::Handle getFrame() const;
protected:
    PopupView(const PopupView&) = delete;
    PopupView& operator =(const PopupView&) = delete;

    /// @brief Override to draw a custom symbol on the associated popover button.
    /// @param sz Size of the button drawing area.
    virtual void drawPopoverButtonSymbol(const Size& sz); //to draw on parent button

    /// @brief Closes this popup view programmatically (if it is open).
    void close(); //closes popup view (if open). In cases you want to call it programmatically
public:

    /// @brief Default constructor with zero margins.
    PopupView();

    /// @brief Constructs a PopupView with explicit margin values.
    /// @param marginLeft Left margin in pixels.
    /// @param marginTop Top margin in pixels.
    /// @param marginRight Right margin in pixels.
    /// @param marginBottom Bottom margin in pixels.
    PopupView(td::BYTE marginLeft, td::BYTE marginTop, td::BYTE marginRight, td::BYTE marginBottom);

    /// @brief Attaches a popover button that will trigger this popup view.
    /// @param pIBtn Pointer to the IPopoverButton interface.
    void setPopoverButton(gui::IPopoverButton* pIBtn);

    /// @brief Sets an image to be displayed on the associated popover button.
    /// @param pImg Pointer to the image.
    void setPopoverButtonImage(gui::Image* pImg);

    /// @brief Sets a symbol to be displayed on the associated popover button.
    /// @param pSymb Pointer to the symbol.
    void setPopoverButtonSymbol(gui::Symbol* pSymb);

    /// @brief Disables or enables this popup view.
    /// @param bDisable True to disable, false to enable.
    void disable(bool bDisable);

    /// @brief Returns whether this popup view is enabled.
    /// @return True if enabled.
    bool isEnabled() const;

    /// @brief Returns whether this popup view is disabled.
    /// @return True if disabled.
    bool isDisabled() const;

    /// @brief Sets where the popup appears relative to the button.
    /// @param location Desired popup location.
    void setLocation(Popover::Location location);

    /// @brief Returns the current popup location setting.
    /// @return Current Popover::Location value.
    Popover::Location getLocation() const;
};

} //namespace gui
