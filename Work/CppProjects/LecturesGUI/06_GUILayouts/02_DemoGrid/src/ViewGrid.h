//
//  Created by Izudin Dzafic on 1 Dec 2022.
//  Copyright © 2022 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/NumericEdit.h>
#include <gui/TextEdit.h>
#include <gui/Button.h>
#include <gui/HorizontalLayout.h>
#include <gui/GridLayout.h>
#include <cnt/StringBuilder.h>
#include <gui/GridComposer.h>

class ViewGrid : public gui::View
{
protected:
    gui::Label _lblName;
    gui::LineEdit _editName;
    gui::Label _lblQuantity;
    gui::NumericEdit _neQuantity;
    gui::Label _lblPrice;
    gui::NumericEdit _nePrice;
    gui::Label _lblValue;
    gui::NumericEdit _neValue;
    gui::Button _btnAdd;
    gui::Button _btnDelete;
    gui::TextEdit _textEdit;
    gui::HorizontalLayout _hlButtons;
    gui::GridLayout _gl;
public:
    ViewGrid()
    : _lblName(tr("name"))
    , _lblQuantity(tr("quant"))
    , _neQuantity(td::decimal3, gui::LineEdit::Messages::Send)
    , _lblPrice(tr("price"))
    , _nePrice(td::decimal2, gui::LineEdit::Messages::Send)
    , _lblValue(tr("value"))
    , _neValue(td::decimal2)
    , _btnAdd(tr("add"))
    , _btnDelete(tr("delete"))
    , _hlButtons(3)
    , _gl(6,2)
    {
        //set as read only
        _neValue.setAsReadOnly();
        
        //populate horizontal layout with buttons
        _hlButtons.appendSpacer();
        _hlButtons.append(_btnAdd);
        _hlButtons.append(_btnDelete);

        //grid can be populated on two ways
        if (false) //change true -> false -> true (same GUI)
        {
            //1. using insert
            _gl.insert(0,0, _lblName); _gl.insert(0,1,_editName);
            _gl.insert(1,0, _lblQuantity); _gl.insert(1,1,_neQuantity);
            _gl.insert(2,0, _lblPrice); _gl.insert(2,1,_nePrice);
            _gl.insert(3,0, _lblValue); _gl.insert(3,1,_neValue);
            _gl.insert(4,0, _hlButtons, -1);
            _gl.insert(5,0, _textEdit, -1);
        }
        else
        {
            //using GridComposer (appendRow & appendCol)
            gui::GridComposer gc(_gl);
            gc.appendRow(_lblName); gc.appendCol(_editName);
            gc.appendRow(_lblQuantity); gc.appendCol(_neQuantity);
            gc.appendRow(_lblPrice); gc.appendCol(_nePrice);
            gc.appendRow(_lblValue); gc.appendCol(_neValue);
            gc.appendRow(_hlButtons, 0); //span columns until the last column
            gc.appendRow(_textEdit, 0); //span columns until the last column
        }
        
        setLayout(&_gl); //set the layout of this view
    }
    
    bool onFinishEdit(gui::LineEdit* pCtrl) override
    {
        if ( (pCtrl == &_neQuantity) || (pCtrl == &_nePrice) )
        {
            td::Variant quant = _neQuantity.getValue();
            td::Variant price = _nePrice.getValue();
            auto value = quant * price;
            td::Decimal2 decVal(value.r8Val());
            _neValue.setValue(decVal);
            return true;
        }
        return false;
    }
    
    
    bool onClick(gui::Button* pBtn) override
    {
        if (pBtn == &_btnAdd)
        {
            cnt::StringBuilderSmall sb;
            sb.appendString(_editName.getText());
            sb.appendCString(", Quantity=");
            sb.appendString(_neQuantity.getText());
            sb.appendCString(", Value=");
            sb.appendString(_neValue.getText());
            sb.appendCString(" ");
            td::String str = sb.toString();
            _textEdit.appendString(str);
            return true;
        }
        else if (pBtn == &_btnDelete)
        {
            _textEdit.clean();
            return true;
        }
        return false;
    }
    
    
};
