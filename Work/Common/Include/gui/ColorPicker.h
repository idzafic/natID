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
#include "PopoverButton.h"
#include <gui/ColorPopoverView.h>
//#include <td/Color.h>


namespace gui
{

class ColorPopoverView;

class NATGUI_API ColorPicker : public PopoverButton
{
private:
    std::function<void()> _onChangedValue;
protected:
    gui::ColorPopoverView _colorPopoverView;
public:
    ColorPicker();
    ColorPicker(const td::String& toolTip);
    //~ColorPicker();
    virtual gui::ObjType getObjType() const override{ return ObjType::ColorPicker;}
    bool setValue(const td::Variant& val, bool sendMessage=true) override;
    bool getValue(td::Variant& val, bool checkType = false) const override;
    td::ColorID getValue() const;
    void setValue(td::ColorID colorID, bool sendMessage = false);

    const std::function<void()>& getChangedValueHandler() const;
    void onChangedValue(const std::function<void()>& fnToCall);
};
}
