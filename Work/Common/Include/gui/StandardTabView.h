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
#include <gui/Types.h>
#include <td/String.h>

namespace gui
{

class Image;

class NATGUI_API StandardTabView : public BaseView
{
private:
    std::function<void(int)> _onChangedSelection;
public:
    enum class TabLocation: unsigned char {Top=0, Bottom, Left, Right};
    enum class NoTabsStyle : unsigned char {BezelBorder=0, LineBorder, NoBorder};
protected:
    void measure(CellInfo& cell) override;
    void reMeasure(CellInfo& cell) override;
    
    StandardTabView(int);
public:
    StandardTabView();
    ~StandardTabView();
    void setParentFrame(Frame* pFrame) override;
    gui::ObjType getObjType() const override { return ObjType::StandardTabView;}
    int addView(BaseView* pView, const char* title, Image* image = nullptr);
    int addView(BaseView* pView, const td::String& strTitle, Image* image = nullptr);
   // void removeView(int pos);
    //void removeView(const BaseView* pView);
    const BaseView* getView(int pos) const;
    BaseView* getView(int pos);
    BaseView* getCurrentView();
    const BaseView* getCurrentView() const;
    int getCurrentViewPos() const;
    void setCurrentViewPos(int pos);
    //int getViewPos(const BaseView* pView);
    int getNumberOfViews() const;
    
    void setTabLocation(StandardTabView::TabLocation tabLocation);
    void setNoTabsStyle(StandardTabView::NoTabsStyle noTabsStyle);
    
    StandardTabView::TabLocation getTabLocation() const;

    const std::function<void(int)>& getChangedSelectionHandler() const;
    void onChangedSelection(const std::function<void(int)>& fnToCall);
};

} //namespace gui
