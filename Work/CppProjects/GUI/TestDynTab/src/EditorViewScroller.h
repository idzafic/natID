//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/ViewScroller.h>
//#include <gui/Image.h>
//#include <gui/Transformation.h>
#include "EditorView.h"
//#include "ViewCanvas.h"

class EditorViewScroller : public gui::ViewScroller
{
private:
protected:
    EditorView _contentView;

protected:
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        if (menuID == 100)
        {
            showAlert("EditorViewScroller ctx menu", "TabView Header context menu option clicked!");
            return true;
        }
        return false;
    }
public:
    EditorViewScroller()
    : gui::ViewScroller(gui::ViewScroller::Type::ScrollAndAutoHide, gui::ViewScroller::Type::ScrollAndAutoHide)
    {
        setContentView(&_contentView);
    }
    

};
