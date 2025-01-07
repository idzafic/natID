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
#include <cnt/PushBackVector.h>

namespace gui
{
class NATGUI_API ViewSwitcher : public BaseView
{
protected:
    using ViewOwnership= struct
    {
        BaseView* pView;
        td::Ownership ownerShip;
    };
    cnt::PushBackVector<ViewOwnership, 64> _views;
    int _currPos = -1;
protected:
    void measure(CellInfo& cell) override;
    void reMeasure(CellInfo& cell) override;
    virtual BaseView* getBackgroundView();
    //void showBackgroundView();
    ViewSwitcher() = delete;
public:
    ViewSwitcher(size_t nViews);
    ~ViewSwitcher();
    void setParentFrame(Frame* pFrame) override;
    gui::ObjType getObjType() const override { return ObjType::ViewSwitcher;}
    int addView(BaseView* pView, bool bMakeVisible = false);
    int insertView(BaseView* pView, int atPos);
    
    int getViewPos(const BaseView* pView) const;
    BaseView* getView(int pos) const;
    const BaseView* getCurrentView() const;
    BaseView* getCurrentView();
    
    void showView(BaseView* pView, bool bSetFocus = true);
    void showView(int pos, bool bSetFocus = true);
    
    int getNoOfViews() const;
    int getCurrentViewPos() const;
};

} //namespace gui
