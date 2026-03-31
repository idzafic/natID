//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/StandardTabView.h>
#include "ViewGrid.h"
#include "ViewVert.h"
#include "ViewWithPickers.h"

class MainView : public gui::StandardTabView
{
private:
protected:
    ViewGrid _v1;
    ViewVert _v2;
    ViewWithPickers _v3;
   
public:
    MainView()
    {
        addView(&_v1, "GridView tab");
        addView(&_v2, "Drugi tab");
        addView(&_v3, "Pickers tab");
        
        gui::StandardTabView::onChangedSelection([](int newPos)
            {
            mu::dbgLog("New position=%d", newPos);
            });
    }
    
};
