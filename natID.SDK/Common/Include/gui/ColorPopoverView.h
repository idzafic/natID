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
#include <gui/PopoverCanvas.h>

namespace gui
{

class ColorPicker;

class NATGUI_API ColorPopoverView : public PopoverCanvas
{
private:
    void drawCurrentItemOnPOButton(const Size& sz) override;
    void drawCurrentItemOnPOButton(const Rect& r) override;
protected:
    const td::String& getPopoverLabel(td::UINT2 itemPos) const override;
    const td::String& getPopoverToolTip(td::UINT2 itemPos) const override;
    
    void onDraw(const gui::Rect& rect) override;
  
    void drawItem(td::UINT2 itemPos, const gui::Rect& r) override;

    ColorPopoverView(const ColorPopoverView&) = delete;
    ColorPopoverView& operator =(const ColorPopoverView&) = delete;
public:
    ColorPopoverView();
    ~ColorPopoverView();;
};

} //namespace gui
