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
#include "NavigatorView.h"


namespace gui
{

class NATGUI_API Navigator : public ViewScroller
{
private:
    std::function<void(gui::Navigator*)> _onChangedSelection;
protected:
    NavigatorView _view;
protected:
    //virtual Frame::FixSizes getFixSizesInfo();
    //bool reMeasure();
protected:
    void measure(CellInfo&) override;
    void reMeasure(CellInfo&) override;
    Navigator() = delete;
    bool isHidden() const override;
    void setGeometry(const Geometry& cellFrame, const Cell& cell) override;
    Navigator(td::UINT2 nItems, gui::Orientation orientation = gui::Orientation::Vertical, td::UINT2 imageHeight = 64, float widthMultiplier=2.5);
public:
    ~Navigator();
    gui::ObjType getObjType() const override { return ObjType::Navigator;}
    void setItem(td::UINT2 itemPos, Image* pImg, const td::String& str);
    void setItem(td::UINT2 itemPos, Symbol* pSymb, const td::String& str, float rotateDeg=0);
    td::UINT2 getCurrentSelection() const;
    void setCurrentSelection(td::UINT2 selection, bool bSendMsg = false);

    const std::function<void(gui::Navigator*)>& getChangedSelectionHandler() const;
    void onChangedSelection(std::function<void(gui::Navigator*)> fnToCall);
};

} //namespace gui
