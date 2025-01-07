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
#include <gui/DataCtrl.h>
#include <gui/IPopoverButton.h>

namespace gui
{

class PopoverView;
class PopoverButtonHelper;

class NATGUI_API PopoverButton : public DataCtrl, public IPopoverButton
{
private:
    std::function<void()> _onChangedSelection;
protected:
    PopoverView* _popoverView = nullptr;
    //td::UINT2 _currentSelection = 0;
protected:
    void measure(CellInfo&) override;
    void reMeasure(CellInfo&) override;
    void setPopoverCurrentSelection(td::UINT2 pos, bool closePopover) override;
    void sendPopoverMessage() override;
    
    void closePopover() override;
    
    PopoverButton(td::DataType dt);
public:
    PopoverButton(PopoverView* popoverView);
    ~PopoverButton();
    gui::ObjType getObjType() const override { return ObjType::PopoverButton;}
    PopoverView* getPopoverView();
    td::UINT2 getCurrentSelection() const;
    void enablePopover(bool bEnable) override;
    const std::function<void()>& getChangedSelectionHandler() const;
    void onChangedSelection(const std::function<void()>& fnToCall);
};

} //namespace gui
