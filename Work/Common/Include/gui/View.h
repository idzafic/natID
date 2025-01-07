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
#include "BaseView.h"
#include <gui/_aux/Cell.h>
#include <tuple>

namespace gui
{

class Layout;
class Control;
class ViewHelper;

class NATGUI_API View : public BaseView
{
    friend class Layout;
    friend class Control;
    friend class ViewHelper;
    
protected:
    Cell _cell;
    td::BYTE _marginLeft;
    td::BYTE _marginTop;
    td::BYTE _marginRight;
    td::BYTE _marginBottom;
    
private:
    void removeAllCtrls();
    void addCtrl(Control* ctrl);
    
protected:
    Frame::FixSizes getFixSizesInfo() override;
    bool reMeasure() override;
    void measure(CellInfo& cell) override;
    void reMeasure(CellInfo& cell) override;
    virtual void adjustLayout(const Size& newSize); //called on every resize
    void setMargins(td::BYTE left, td::BYTE top, td::BYTE right, td::BYTE bottom);
    std::tuple<bool, bool> isFixedSize() const override;
    View (int k);
    View(const View&) = delete;
    View(View&&) = delete;
public:
    View();
    View(td::BYTE marginLeft, td::BYTE marginTop, td::BYTE marginRight, td::BYTE marginBottom);
    ~View();
    void getMinSize(Size& minSize) const override;
    std::tuple<td::BYTE, td::BYTE, td::BYTE, td::BYTE> getMargins() const; //left, top, right, bottom
    td::WORD getTotalHMargin() const override;
    td::WORD getTotalVMargin() const override;
    gui::ObjType getObjType() const override { return ObjType::View;}
    void setAsCentralViewInWindow(Window* pWnd, Frame::FixSizes fixSizes) override;
    void setLayout(Layout* pLayout);
    const Cell& getCell() const;
    //void setBackgroundColor(td::ColorID);
};

} //namespace gui
