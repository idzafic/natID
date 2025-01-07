//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/VerticalLayout.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/TextEdit.h>
#include <gui/Button.h>

class ViewVert : public gui::View
{
private:
protected:
    gui::Button _btn;
    gui::Label _lbl;
    gui::LineEdit _lineEdit;
    gui::Label _lbl2;
    gui::LineEdit _lineEdit2;
    gui::Label _lbl3;
    gui::TextEdit _textEdit;
    gui::VerticalLayout _vl;
public:
    ViewVert()
    : _btn("Još neki duži button, veoma veoma dugačak")
    , _lbl("This is a label:")
    , _lbl2("This is another label:")
    , _lbl3("Label for text edit:")
    , _vl(8)
    {
        _vl.append(_btn);
        _vl.append(_lbl, td::HAlignment::Left);
        _vl.append(_lineEdit);
        _vl.appendSpace(20);
        _vl.append(_lbl2, td::HAlignment::Center);
        _vl.append(_lineEdit2);
        _vl.append(_lbl3, td::HAlignment::Right);
        _vl.append(_textEdit);
        setLayout(&_vl);
    }
    
};
