//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "EditorViewScroller.h"
#include "ViewComplex.h"
#include "ViewWithTable.h"

class ViewWithTabs : public gui::StandardTabView
{
private:
protected:
    ViewComplex _complexView;
    EditorViewScroller _scrollCanvasView;
    ViewWithTable _viewWithTable;
public:
    ViewWithTabs()
    {
        //Image* pImage = nullptr;
        int pos1 = addView(&_complexView, "Complex view", nullptr);
        int pos2 = addView(&_scrollCanvasView, "Canvas with scrolling", nullptr);
        int pos3 = addView(&_viewWithTable, "Table editing view", nullptr);
    }


};
