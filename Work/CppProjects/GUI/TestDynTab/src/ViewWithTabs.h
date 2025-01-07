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
protected:
    
    bool onChangedSelection(gui::StandardTabView* pTab, int newPos) override
    {
        if (pTab == this)
        {
            
//            int iSel = _cmb.getSelectedIndex();
//            td::String selStr = _cmb.getSelectedText();
//            td::String str;
//            str.format("ComboBox cs:%d->%s\n", iSel, selStr.c_str());
            td::String msgText("TabView:Handling onChangedSelection");
            td::String informativeText("Add here handling tabView selection change");
            showAlert(msgText, informativeText);
            //_textEdit.appendString(str);
            return true;
        }
        
        
        return false;
    }
    
    
};
