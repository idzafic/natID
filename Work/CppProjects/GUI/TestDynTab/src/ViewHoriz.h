//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/TextEdit.h>
#include <gui/HorizontalLayout.h>

class ViewHoriz : public gui::View
{
private:
protected:
    gui::Label _lbl;
    gui::LineEdit _lineEdit;
    gui::Label _lbl2;
    gui::LineEdit _lineEdit2;
    gui::Label _lbl3;
    gui::TextEdit _textEdit;
    gui::HorizontalLayout _vl;
public:
    ViewHoriz()
    : _lbl("This is a label:")
    , _lbl2("This is another label:")
    , _lbl3("Label for text edit:")
    , _vl(8)
    {
        _vl.append(_lbl, td::HAlignment::Left, td::VAlignment::Center);
        _vl.append(_lineEdit);
        _vl.appendSpace(20);
        _vl.append(_lbl2, td::HAlignment::Center, td::VAlignment::Bottom);
        _vl.append(_lineEdit2, td::HAlignment::Left, td::VAlignment::Bottom);
        _vl.appendSpacer();
        _vl.append(_lbl3, td::HAlignment::Right);
        _vl.append(_textEdit);
        setLayout(&_vl);
    }
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        if (menuID == 101)
        {
            //handle context menu in this view
            td::String msgText("Handling TabView Context menu");
            td::String informativeText;
            informativeText.format("Handled CTX-View onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);

           showAlert(msgText, informativeText);
            return true;
        }
        return false;
    }
};
