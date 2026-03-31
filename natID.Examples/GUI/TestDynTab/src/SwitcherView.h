//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ViewSwitcher.h>
#include "EditorViewScroller.h"
#include "ViewComplex.h"
#include "ViewWithTable.h"
#include "ViewHoriz.h"
#include "ViewVert.h"
#include "ViewWithTabs.h"

class SwitcherView : public gui::ViewSwitcher
{
private:
protected:
    ViewComplex _complexView;
    EditorViewScroller _scrollCanvasView;
    ViewWithTable _viewWithTable;
    ViewHoriz _horView;
    ViewVert _vertView;
    ViewWithTabs _viewWithStandardTabs;
    int _viewPos = 0;
public:
    SwitcherView()
    : gui::ViewSwitcher(6)
    {
        addView(&_complexView, true);
        addView(&_scrollCanvasView, false);
        addView(&_viewWithTable, false);
        addView(&_horView, false);
        addView(&_vertView, false);
        addView(&_viewWithStandardTabs, false);
    }
    
    std::tuple<int, int> showPrev(gui::ActionItem* pAI)
    {
        int nElems = getNoOfViews();
        
        --_viewPos;
        if (_viewPos < 0)
        {
            _viewPos = 0;
            return {0, nElems};
        }
        showView(_viewPos);
        if (_viewPos <= 0)
            pAI->disable();
        return {_viewPos, nElems};
    }
    
    std::tuple<int, int> showNext(gui::ActionItem* pAI)
    {
        int nElems = getNoOfViews();
        ++_viewPos;
        if (_viewPos >= nElems)
        {
            _viewPos = nElems - 1;
            return {0, nElems};
        }
        showView(_viewPos);
        
        if (_viewPos + 1 >= nElems)
            pAI->disable();
        
        return {_viewPos, nElems};
    }
    
//    std::tuple<int, int> removeCurrent()
//    {
//        int nElems = getNoOfViews();
//        if (nElems < 1)
//            return {-1,0};
//
//        const auto pCurrView = getCurrentView();
//        int iPos = removeView(pCurrView);        
//        return {iPos, nElems-1};
//    }
    
};
