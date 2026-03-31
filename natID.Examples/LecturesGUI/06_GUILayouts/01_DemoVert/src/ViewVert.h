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
#include <gui/VerticalLayout.h>
#include <cnt/StringBuilder.h>

class ViewVert : public gui::View
{
private:
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
    gui::VerticalLayout _vl;
public:
    ViewVert()
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
    , _vl(10)
    {
        //set as read only
        _neValue.setAsReadOnly();
        
        //populate horizontal layout with buttons
        _hlButtons.append(_btnAdd);
        _hlButtons.appendSpacer();
        _hlButtons.append(_btnDelete);

        //populate vertical layout
        _vl << _lblName << _editName << _lblQuantity << _neQuantity << _lblPrice << _nePrice
            << _lblValue << _neValue << _hlButtons << _textEdit;
        //could be done in this way too:
        //_vl.append(_lblName);
        //_vl.append(_editName); ...
        
        //use lambdas for handling events per object
        _btnAdd.onClick([this]()
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
        });
        
        _btnDelete.onClick([this]()
        {
            _textEdit.clean();
        });
    
        setLayout(&_vl); //set the layout of this view
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
    
    //NOTE: This method will not be invoked if lambda handlers for both buttons are provided (see constructor)
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
