//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/PasswordEdit.h>
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

#include <gui/ColorPicker.h>
#include <gui/LinePatternPicker.h>
#include "DialogLogin.h"


class ViewWithPickers : public gui::View
{
private:
    enum class DlgID : td::UINT4 {Login = 1};
protected:
    gui::Label _lblUn;
    gui::LineEdit _editUN;
    gui::Label _lblPwd;
    gui::PasswordEdit _editPwd;
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
    gui::Button _btnLogin;
    gui::Button _btnDelete;
    gui::Button _btnGetDT;
    gui::TextEdit _textEdit;
    gui::GridLayout _gl;
    gui::Label _lblColor;
    gui::ColorPicker _colorPicker;
    gui::Label _lblLinePattern;
    gui::LinePatternPicker _lpPicker;
public:
    ViewWithPickers()
    : _lblUn("User name:")
    , _editUN(gui::LineEdit::Messages::Send)
    , _lblPwd("Password:")
    , _editPwd(gui::LineEdit::Messages::Send)
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
    , _btnLogin("Login...")
    , _btnDelete("Brisi")
    , _btnGetDT("Date&Time->TE")
    //, _lbl3("Label for text edit:")
    , _gl(9,4)
    , _lblColor("Color;")
    , _lblLinePattern("Line pattern:")
    {
//        _editUN.enableMessaging();
//        _editPwd.enableMessaging();
        _editUN.setToolTip("Unesite korisnicko ime");
        _textEdit.setToolTip("Ovo je text edit za log");
        _iznos.setAsReadOnly();
        _comboBox.addItem("Kafa");
        _comboBox.addItem("Sypa");
        _comboBox.addItem("Neki artikal");
        _comboBox.selectIndex(0);
        
        _btnLogin.setAsDefault();
        _btnDelete.setAsCritical();
        _btnDelete.setToolTip("This is critical button type!");
        
        _gl.insert(0,0, _lblUn);
        _gl.insert(0,1, _editUN);
        _gl.insert(0,2, _lblPwd);
        _gl.insert(0,3, _editPwd);
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
        _gl.insert(6,0, _lblColor);
        _gl.insert(6,1, _colorPicker);
        _gl.insert(6,2, _lblLinePattern);
        _gl.insert(6,3, _lpPicker);
        _hl.append(_btnGetDT);
        _hl.appendSpacer();
        _hl.append(_btnDelete);
        _hl.append(_btnLogin);
        
        _gl.insert(7,0, _hl, -1);
        _gl.insert(8,0, _textEdit, -1);
        
        setLayout(&_gl);
    }
    
    bool onClick(gui::Button* pBtn) override
    {
        if (pBtn == &_btnLogin)
        {
            {
                DialogLogin* dlg = new DialogLogin(pBtn); //centrira se u odnosu na button
                dlg->setVisualID(DlgID::Login);
                dlg->openModal(this);
            }

            return true;
        }
        else if (pBtn == &_btnDelete)
        {
            _comboBox.toZero();
//
//            //_textEdit.clean();
//            auto nItems = _comboBox.getNoOfItems();
//            if (nItems <= 0)
//                showAlert("Combo is empty!", "There is nothing to remove from combo!");
//            else
//            {
//
//                _comboBox.removeItem(0);
//                _comboBox.selectIndex(0);
//            }
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

    bool onBeginEdit(gui::LineEdit* pCtrl) override
    {
        _textEdit.appendString(" Begin edit! ");
        mu::dbgLog(" Begin Edit ");
        return true;
    }
    
    bool onFinishEdit(gui::LineEdit* pCtrl) override
    {
        _textEdit.appendString(" End edit! ");
//        td::String strVal(pCtrl->getText());
//        _textEdit.appendString(strVal);
//        _textEdit.appendString("\t");
        mu::dbgLog(" End Edit!");
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
        else if (pCtrl == &_editUN)
        {
            td::String strUN = _editUN.getText();
            _textEdit.appendString(" UserName=");
            _textEdit.appendString(strUN);
//            _textEdit.appendString("\n");
        }
        else if (pCtrl == &_editPwd)
        {
            td::String strPwd = _editPwd.getText();
            _textEdit.appendString(" Password=");
            _textEdit.appendString(strPwd);
//            _textEdit.appendString("\n");
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
    
    //check dialog clicks
    bool onClick(gui::Dialog* pDlg, td::UINT4 dlgID) override
    {
        if (dlgID == (td::UINT4) DlgID::Login)
        {
            gui::Dialog::Button::ID btnID = pDlg->getClickedButtonID();
            if (btnID == gui::Dialog::Button::ID::OK)
            {
                showAlert("Success!!", "User managed to login!");
            }
            else
            {
                showAlert("Error!", "User didn't login!");
            }
            return true;
        }
        return false;
    }
};
