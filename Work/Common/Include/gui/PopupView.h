// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

class NATGUI_API PopupView : public View
{
    friend class PopupViewHelper;
    
protected:
    enum class SymbolType : td::BYTE {Image = 0, Symbol, CustomDrawing};
    IPopoverButton* _btnPopover = nullptr;
    Handle _popoverFrame = nullptr;
    Popover::Location _location = Popover::Location::AboveButton;
    union{
        gui::Image* _pImage = nullptr;
        gui::Symbol* _pSymbol;
    };
    SymbolType _symbolType = SymbolType::CustomDrawing;
    bool _bDisabled = false;
private:
    const gui::Handle getFrame() const;
protected:
    PopupView(const PopupView&) = delete;
    PopupView(PopupView&&) = delete;
    
    virtual void drawPopoverButtonSymbol(const Size& sz); //to draw on parent button
    
    void close(); //closes popup view (if open). In cases you want to call it programmatically
public:
    
    PopupView();
    PopupView(td::BYTE marginLeft, td::BYTE marginTop, td::BYTE marginRight, td::BYTE marginBottom);
    
    void setPopoverButton(gui::IPopoverButton* pIBtn);

    void setPopoverButtonImage(gui::Image* pImg);
    void setPopoverButtonSymbol(gui::Symbol* pSymb);
    
    void disable(bool bDisable);
    bool isEnabled() const;
    bool isDisabled() const;
    
    void setLocation(Popover::Location location);
    Popover::Location getLocation() const;
};

} //namespace gui
