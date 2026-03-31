//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/StandardTabView.h>
#include "ViewGrid.h"
#include "ViewCanvas.h"

class MainView : public gui::StandardTabView
{
private:
protected:
    ViewGrid _view;
    ViewCanvas _canvas;   
public:
    MainView()
    {
        addView(&_view, "Prvi tab");
        addView(&_canvas, "Canvas tab");
        //setTabLocation(StandardTabView::TabLocation::Bottom);
        setCurrentViewPos(1);
    }
    
    bool onChangedSelection(gui::StandardTabView* pTV, int newPos) override
    {
        gui::BaseView* pBV = pTV->getView(newPos);
        assert(pBV);
        if (newPos >= 0)
        {
            if (newPos == 0)
            {
                assert(pBV = &_view);
            }
            else
            {
                assert(pBV == &_canvas);
            }
        }
            
        mu::dbgLog("Current viewPos=%d", newPos);
        return false;
    }
    
};
