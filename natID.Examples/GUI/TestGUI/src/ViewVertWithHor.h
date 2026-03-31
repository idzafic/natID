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
#include <gui/Button.h>

class ViewVertWithHor : public gui::View
{
private:
protected:
    gui::Label _lbl;
    gui::LineEdit _lineEdit;
    gui::HorizontalLayout _h1;
    gui::Label _lbl2;
    gui::LineEdit _lineEdit2;
    gui::HorizontalLayout _h2;
    gui::Label _lbl3;
    gui::TextEdit _textEdit;
    gui::HorizontalLayout _h3;
    
    gui::HorizontalLayout _btns;
    gui::Button _btn1;
    gui::Button _btn2;
    gui::VerticalLayout _vl;
public:
    ViewVertWithHor()
    : _lbl("This is a label:")
    , _h1(2)
    , _lbl2("This is another label:")
    , _h2(2)
    , _lbl3("Label for text edit:")
    , _h3(2)
    , _btns(2)
    , _btn1("OK")
    , _btn2("Cancel")
    , _vl(5)
    {
        _h1.append(_lbl, td::HAlignment::Left);
        _h1.append(_lineEdit);
        _vl.append(_h1);
        _vl.appendSpace(20);
        _h2.append(_lbl2, td::HAlignment::Center);
        _h2.append(_lineEdit2);
        _vl.append(_h2);
        _h3.append(_lbl3, td::HAlignment::Right);
        _h3.append(_textEdit);
        _vl.append(_h3);
        _btns.append(_btn1);
        _btns.appendSpacer();
        _btns.append(_btn2);
        _vl.append(_btns);
        setLayout(&_vl);
    }
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        auto pAI = aiDesc.getActionItem();
        
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
