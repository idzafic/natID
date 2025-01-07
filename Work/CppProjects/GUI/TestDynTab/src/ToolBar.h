//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>

class ToolBar : public gui::ToolBar
{
protected:
    gui::Image _imgAddView;
    gui::Image _imgMoveViewLeft;
    gui::Image _imgMoveViewRight;
    gui::Image _imgRemoveView;
    gui::Image _imgRemoveAllViews;
public:
    ToolBar()
    : gui::ToolBar("myToolBar", 7)
    , _imgAddView(":add")
    , _imgMoveViewLeft(":mvLeft")
    , _imgMoveViewRight(":mvRight")
    , _imgRemoveView(":rem")
    , _imgRemoveAllViews(":remAll")
    {
        addItem(tr("addView"), &_imgAddView, tr("addViewTT"), 10, 0, 0, 10);
        
        addSpaceItem();
        
        addItem(tr("mvvLeft"), &_imgMoveViewLeft, tr("mvvLeftTT"), 10, 0, 0, 20);
        
        addItem(tr("mvvRight"), &_imgMoveViewRight, tr("mvvRightTT"), 10, 0, 0, 30);
        
        addSpaceItem();
        
        addItem(tr("remView"), &_imgRemoveView, tr("remViewTT"), 10, 0, 0, 40);
        
        addItem(tr("remAllViews"), &_imgRemoveAllViews, tr("remAllViewsTT"), 10, 0, 0, 50);
    }
    
};
