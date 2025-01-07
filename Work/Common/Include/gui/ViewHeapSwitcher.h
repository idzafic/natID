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
#include "ViewSwitcher.h"


namespace gui
{

class NATGUI_API ViewHeapSwitcher : public ViewSwitcher
{
protected:
    BaseView* _pBackgroundView = nullptr;
protected:
    BaseView* getBackgroundView() override final;
public:
    ViewHeapSwitcher(size_t nViews = 0);
    ~ViewHeapSwitcher();
    
    void setBackgroundView(gui::BaseView* pBackgroundView);

    void removeView(int pos);
    int removeView(const BaseView* pView); //pView will be deleted (unusable)
    void removeView(int posToRemove, int posToSelect);
    void removeAll(bool bShowBackground = true);

    void setViewOwnership(int viewPos, td::Ownership ownership); //Intern - TabView will delete the view (default)
    td::Ownership getViewOwnership() const;
    BaseView* detach(int viewPos);
};

} //namespace gui
