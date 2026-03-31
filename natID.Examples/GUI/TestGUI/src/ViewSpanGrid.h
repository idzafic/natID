//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/TextEdit.h>
#include <gui/VerticalLayout.h>
#include <gui/HorizontalLayout.h>
#include <gui/GridLayout.h>


class ViewSpanGrid : public gui::View
{
private:
protected:
    gui::Label _lbl;
    gui::LineEdit _lineEdit;
    //gui::HorizontalLayout _h1;
    gui::Label _lbl2;
    gui::LineEdit _lineEdit2;
    //gui::HorizontalLayout _h2;
    gui::Label _lbl3;
    gui::TextEdit _textEdit;
    //gui::HorizontalLayout _h3;
    gui::GridLayout _gl;
public:
    ViewSpanGrid()
    : _lbl("This is a label:")
    , _lbl2("This is another label:")
    , _lbl3("Label for text edit (longest of all labels - just for demo):")
    , _gl(4,2)
    {
        _gl.insert(0,0, _lbl);
        _gl.insert(0,1, _lineEdit);
        _gl.insert(1,0, _lbl2);
        _gl.insert(1,1, _lineEdit2);
        _gl.insert(2,0, _lbl3, 0, td::HAlignment::Center, 1, td::VAlignment::Top);
        _gl.insert(3,0, _textEdit, 0);
        setLayout(&_gl);
    }
    
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
                return true;
            }
        }
        td::String msgText("Handling onActionItem");
        td::String informativeText;
        informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);
        
       showAlert(msgText, informativeText);
        return true;
    }
};
