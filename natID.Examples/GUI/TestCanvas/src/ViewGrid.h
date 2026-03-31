//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/TextEdit.h>
#include <gui/NumericEdit.h>
#include <gui/CheckBox.h>
#include <gui/Slider.h>
#include <gui/ComboBox.h>
#include <gui/Button.h>
#include <gui/DateEdit.h>
#include <gui/TimeEdit.h>

#include <gui/HorizontalLayout.h>
#include <gui/ProgressIndicator.h>

#include <gui/GridLayout.h>

class ViewGrid : public gui::View
{
private:
protected:
    gui::Label _lbl;
    gui::LineEdit _lineEdit;
    gui::Label _lbl2;
    gui::LineEdit _lineEdit2;
    gui::Label _lblKol;
    gui::NumericEdit _kol;
    gui::Label _lblCij;
    gui::NumericEdit _cij;
    gui::Label _lblIzn;
    gui::NumericEdit _iznos;
    gui::Label _lblKupiti;
    gui::CheckBox _cbKupiti;
    gui::Label _lblSlider;
    gui::Slider _slider;
    gui::ProgressIndicator _progIndicator;
    gui::Label _lblCB;
    gui::ComboBox _comboBox;
    gui::Label _lblDate;
    gui::DateEdit _date;
    gui::Label _lblTime;
    gui::TimeEdit _time;
    //gui::Label _lbl3;
    gui::HorizontalLayout _hl;
    gui::Button _btnGetText;
    gui::Button _btnClearText;
    gui::Button _btnGetDT;
    gui::TextEdit _textEdit;
    gui::GridLayout _gl;
public:
    ViewGrid()
    : _lbl("First lbl:")
    , _lbl2("Second lbl:")
    , _lblKol("Kolicina:")
    , _kol(td::decimal3, gui::LineEdit::Messages::Send)
    , _lblCij("Cijena:")
    , _cij(td::decimal4, gui::LineEdit::Messages::Send)
    , _lblIzn("Iznos:")
    , _iznos(td::decimal2)
    , _lblKupiti("Kupiti:")
    , _lblSlider("Slider:")
    , _cbKupiti(nullptr)
    , _lblCB("Kombo:")
    , _lblDate("Datum:")
    , _lblTime("Vrijeme:")
    , _hl(4)
    , _btnGetText("Daj text")
    , _btnClearText("Brisi")
    , _btnGetDT("Date&Time->TE")
    //, _lbl3("Label for text edit:")
    , _gl(8,4)
    {
//        _lineEdit.enableMessaging();
//        _lineEdit2.enableMessaging();
        _lineEdit.setToolTip("Ovo je tooltip");
        _lineEdit2.setToolTip("Ovo je drugi tooltip....");
        _textEdit.setToolTip("Ovo je text edit za log");
        _iznos.setAsReadOnly();
        _comboBox.addItem("Kafa");
        _comboBox.addItem("Sypa");
        _comboBox.addItem("Neki artikal");
        _comboBox.selectIndex(0);
        
        _btnGetText.setAsDefault();
        
        _gl.insert(0,0, _lbl);
        _gl.insert(0,1, _lineEdit);
        _gl.insert(0,2, _lbl2);
        _gl.insert(0,3, _lineEdit2);
        _gl.insert(1,0, _lblKol);
        _gl.insert(1,1, _kol);
        _gl.insert(1,2, _lblCij);
        _gl.insert(1,3, _cij);
        _gl.insert(2,0, _lblIzn);
        _gl.insert(2,1, _iznos);
        _gl.insert(2,2, _lblKupiti);
        _gl.insert(2,3, _cbKupiti);
        _gl.insert(3,0, _lblSlider);
        _gl.insert(3,1, _slider);
        _gl.insert(3,2, _progIndicator, -1);
        _gl.insert(4,0, _lblCB);
        _gl.insert(4,1, _comboBox);
        _gl.insert(5,0, _lblDate);
        _gl.insert(5,1, _date);
        _gl.insert(5,2, _lblTime);
        _gl.insert(5,3, _time);
        
        _hl.append(_btnGetDT);
        _hl.appendSpacer();
        _hl.append(_btnClearText);
        _hl.append(_btnGetText);
        
        _gl.insert(6,0, _hl, -1);
        _gl.insert(7,0, _textEdit, -1);
        setLayout(&_gl);
    }
    
    bool onBeginEdit(gui::LineEdit* pCtrl) override
    {
        _textEdit.appendString("Begin edit!");
        mu::dbgLog("Begin Edit");
        return true;
    }
    
    bool onFinishEdit(gui::LineEdit* pCtrl) override
    {
        _textEdit.appendString("End edit: ");
//        td::String strVal(pCtrl->getText());
//        _textEdit.appendString(strVal);
//        _textEdit.appendString("\t");
        mu::dbgLog("End Edit");
        if (pCtrl == &_kol || pCtrl == &_cij)
        {
            td::Variant kol = _kol.getValue();
            td::Variant cij = _cij.getValue();
            
            td::Variant dblIznos = kol * cij;
            td::Decimal2 dec2(dblIznos.r8Val());
            
            td::Variant iznos(dec2);
            
            _iznos.setValue(iznos);
            td::Decimal2 maxIznos(100.0);
            if (dec2 > maxIznos)
            {
                _lblIzn.setTitle("Veoma skup artikal:");
                //_cbKupiti.setTitle("Ne kupovati");
            }
            else
                _lblIzn.setTitle("Iznos ok:");
                
            return true;
        }
        return false;
    }
    
    bool onClick(gui::CheckBox* pCB) override
    {
        if (pCB== &_cbKupiti)
        {
            bool isChecked = _cbKupiti.isChecked();
            if (isChecked)
            {
                _textEdit.appendString(" Odabrano KUPITI ");
            }
            else
            {
                _textEdit.appendString(" Odabrano NEKupiti ");
            }
            return true;
        }
        return false;
    }
    
    bool onChangedValue(gui::Slider* pSlider) override
    {
        if (pSlider == &_slider)
        {
            double val = _slider.getValue();
            _progIndicator.setValue(val);
            return true;
        }
        return false;
    }
    
    bool onChangedSelection(gui::ComboBox* pCB) override
    {
        if (pCB == &_comboBox)
        {
            td::String strSel = pCB->getSelectedText();
            _textEdit.appendString(strSel);
            return true;
        }
        return false;
    }
    
    bool onClick(gui::Button* pBtn) override
    {
        if (pBtn == &_btnGetText)
        {
           td::String str = _textEdit.getText();
            const char* pStr = str.c_str();
            int k=0;
            ++k;
            return true;
        }
        else if (pBtn == &_btnClearText)
        {
            _textEdit.clean();
            return true;
        }
        else if (pBtn == &_btnGetDT)
        {
            td::Date dateVal = _date.getValue();
            td::Time timeVal = _time.getValue();
            
            td::String strDate = dateVal.toString();
            _textEdit.appendString(" Date=");
            _textEdit.appendString(strDate);
            td::String strTime = timeVal.toString();
            _textEdit.appendString(" Time=");
            _textEdit.appendString(strTime);
            return true;
        }
        return false;
    }
    
};
