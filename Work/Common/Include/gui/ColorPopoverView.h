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
#include <gui/PopoverView.h>

namespace gui
{

class ColorPicker;

class NATGUI_API ColorPopoverView : public PopoverView
{
    
private:
//    gui::ColorPicker* _pColorPicker;
//    gui::Rect _rectColor;
//    td::BYTE _nColors;
//    td::BYTE _nRows;
//    td::BYTE _colorIndex;
//    td::BYTE _selectedColorIndex;
//    td::BYTE _closeOnClick;
private:
    void drawCurrentItemOnPOButton(const Size& sz) override;
    void drawCurrentItemOnPOButton(const Rect& r) override;
protected:
    gui::ObjType getObjType() const override { return ObjType::ColorPopoverView;}
    const td::String& getPopoverLabel(td::UINT2 itemPos) const override;
    const td::String& getPopoverToolTip(td::UINT2 itemPos) const override;
    
    void onDraw(const gui::Rect& rect) override;
    
//    virtual void onPrimaryButtonPressed(const InputDevice& inputDevice);
//    virtual void onCursorMoved(const InputDevice& inputDevice);
//    virtual void onCursorExited(const gui::Point& modelPoint);
    void drawItem(td::UINT2 itemPos, const gui::Rect& r) override;
//    virtual void drawToolTipText(td::UINT2 itemPos, const gui::Rect& r);
public:
    ColorPopoverView();
    ~ColorPopoverView();
//    gui::Size getMinSize() const;
//    void setColorID(td::ColorID colorID);
//    void setColorPicker(gui::ColorPicker* pColorPicker);
//    td::ColorID getSelectedColor() const;
};

} //namespace gui
