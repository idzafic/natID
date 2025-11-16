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
#include <gui/DotPatternPopoverView.h>

namespace gui
{

class NATGUI_API DotPatternPicker : public PopoverButton
{
private:
    std::function<void()> _onChangedValue;
protected:
    DotPatternPopoverView _patternPopoverView;
public:
    DotPatternPicker();
    DotPatternPicker(const td::String& toolTip);
    //gui::ObjType getObjType() const override { return ObjType::DotPatternPicker;}
    bool setValue(const td::Variant& val, bool sendMessage=true) override;
    bool getValue(td::Variant& val, bool checkType = false) const override;
    td::DotPattern getValue() const;
    void setValue(td::DotPattern dotPattern, bool sendMessage = false);

    const std::function<void()>& getChangedValueHandler() const;
    void onChangedValue(const std::function<void()>& fnToCall);
};

} //namespace gui
