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
public:
    EditorViewScroller()
    : gui::ViewScroller(ViewScroller::Type::ScrollAndAutoHide, ViewScroller::Type::ScrollAndAutoHide)
    {
        setContentView(&_contentView);
    }
    
    inline void drawSymbols(gui::CoordType x, gui::CoordType y, gui::SymbolDrawingAttribs* pGroupAttribs = nullptr)
    {
       
    }
    
//    virtual void onDraw(const gui::Rect& rDraw)
//    {
//        
//    }
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        
        if (menuID == 255)
        {
            if (actionID == 10)
            {
                //increase window size
//                gui::Geometry g;
//                getGeometry(g);
//                g.size.width *= 1.2f;
//                g.size.height *= 1.2f;
//                setGeometry(g, true, gui::Frame::Animation::Yes);
//                return;
            }
            else if (actionID == 20)
            {
                //decrease window size
//                gui::Geometry g;
//                getGeometry(g);
//                g.size.width *= 0.8f;
//                g.size.height *= 0.8f;
//                setGeometry(g, true, gui::Frame::Animation::Yes);
//                return;
            }
        }
        td::String msgText("Handling onActionItem");
        td::String informativeText;
        informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);
        
       showAlert(msgText, informativeText);
        return true;
    }
};
