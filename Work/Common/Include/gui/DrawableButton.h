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
#include <gui/Control.h>
#include <gui/IPopoverButton.h>
namespace gui
{
class PopoverView;
class DrawableButtonHelper;


class NATGUI_API DrawableButton : public Control
{
    friend class DrawableButtonHelper;
private:
    std::function<void()> _onClick;
protected:
    gui::Size _size;
protected:
    void measure(CellInfo&) override;
    void reMeasure(CellInfo&) override;
    virtual void onResize(const gui::Size& newSize);
    virtual void onDraw(const gui::Rect& rect);
    
    DrawableButton(int);
    DrawableButton();
public:
    ~DrawableButton();
    gui::ObjType getObjType() const override { return ObjType::DrawableButton;}
    virtual PopoverCanvas* getPopoverCanvas();
    virtual gui::IPopoverButton* getPopoverButton();
    virtual td::UINT2 getPopoverCurrentSelection();

    const std::function<void()>& getClickHandler() const;
    void onClick(const std::function<void()>& fnToCall);
};

} //namespace gui
