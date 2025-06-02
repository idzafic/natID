// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

class NATGUI_API Button : public Control
{
public:
    enum class Type : td::BYTE {Normal=0, Default, Critical, Constructive, Destructive};
    enum class ContentType : td::BYTE {Text=0, Image, Symbol};
private:
    union
    {
        const Image* _pImage;
        const Symbol* _pSymbol;
    };
    std::function<void()> _onClick;
    Type _type = Type::Normal;
    ContentType _contentType = ContentType::Text;
    td::BYTE _menuID = 0;
    ContextMenu::Location _preferableMenuLocation = ContextMenu::Location::Top;

protected:
    void measure(CellInfo&) override;
    void reMeasure(CellInfo&) override;    
public:
    Button(); //creates txt button with empty title
    Button(const char* lbl);
    Button(const td::String& lbl);
    Button(const td::String& lbl, const td::String& toolTip);
    Button(const gui::Image* img, const td::String& toolTip = td::String());
//    ~Button();
    
    gui::ObjType getObjType() const override { return ObjType::Button;}
    
    void setAsDefault();
    void setAsCritical();
    void setTitle(const td::String& title) override;
    void setType(Button::Type type);
    void setImage(const gui::Image* img);
    void setFlat();
    void setCircular();
    void setToMinSize();
    void setChecked(bool bCheck);
    
    Type getType() const;
    ContentType getContentType() const;
    const Image* getImage() const;
    const Symbol* getSymbol() const;
    
    //use context menu ID to show menu on this button 
    //in this case onClick will not be called, instead onAction event will be generated
    void setMenuID(td::BYTE nenuID, ContextMenu::Location preferableMenuLocation = ContextMenu::Location::Top);
    td::BYTE getMenuID() const;
    ContextMenu::Location getMenuPreferableLocation() const;
    
    const std::function<void()>& getClickHandler() const;
    void onClick(const std::function<void()>& fnToCall);
};
}
