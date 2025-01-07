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
//#include <gui/Image.h>
//#include <gui/Symbol.h>
#include <cnt/Array.h>
#include <gui/Shape.h>
#include <td/DotPattern.h>
//#include <gui/DrawableString.h>

namespace gui
{

class NATGUI_API DotPatternPopoverView : public PopoverView
{
protected:
    cnt::Array<td::String, (size_t) td::DotPattern::NA> _labels;
    cnt::Array<Shape, (size_t) td::DotPattern::NA> _shapes;
    td::UINT2 _maxLblLen;
private:
    void drawCurrentItemOnPOButton(const Size& sz) override;
protected:
    
    const td::String& getPopoverLabel(td::UINT2 itemPos) const override;
    const td::String& getPopoverToolTip(td::UINT2 itemPos) const override;
    
    //virtual std::tuple<const td::String&, const td::String&> getPopoverLabelAndToolTip(td::UINT2 itemPos) const;
    void drawItem(td::UINT2 itemPos, const gui::Rect& r) override;
    void drawToolTipText(td::UINT2 itemPos, const gui::Rect& r) override;
    
    bool isResizible() const override;
    td::UINT2 getRequiredButtonWidth() const override;
public:
    DotPatternPopoverView();
};

} //namespace gui
