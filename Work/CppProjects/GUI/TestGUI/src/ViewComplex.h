//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/natID.h>

class ViewComplex : public gui::View
{
private:
protected:
    gui::Label _lbl;
    gui::LineEdit _lineEdit;
    gui::Label _lblQuant;
    gui::NumericEdit _quant;
    //gui::HorizontalLayout _h1;
    gui::Label _lbl2;
    gui::LineEdit _lineEdit2;
    gui::Label _lblPrice;
    gui::NumericEdit _price;
    gui::Label _lblLong;
    gui::LineEdit _lineEdit3;
    gui::Label _lblCmb;
    gui::ComboBox _cmb;
    gui::Label _lblDBCmb;
    gui::DBComboBox _dbCmb;
    gui::CheckBox _checkBox;
    //gui::Label _checkBox;
    gui::Label _lblSlider;
    gui::Slider _slider;
    //gui::HorizontalLayout _h2;
    gui::Label _lbl4;
    gui::ProgressIndicator _progressInd;
    //gui::LineEdit _lineEditInfo;
    gui::Label _lblDE;
    gui::DateEdit _dateEdit;
    gui::Label _lblTE;
    gui::TimeEdit _timeEdit;
    
//    gui::Label _lblDTE;
//    gui::DateTimeEdit _dateTimeEdit;
    
    gui::Label _lblCP;
    gui::ColorPicker _colorPicker;
    
    gui::TextEdit _textEdit;
    
    gui::HorizontalLayout _hlBtns;
    gui::Button _btn1;
    gui::Button _btnHideRow;
    gui::Button _btn2;
    gui::Button _btn3;
    gui::Button _btn4;
    //gui::Button _btnLogo;
    gui::Button _btn5;
    
    gui::GridLayout _gl;
    
    bool _isLblHidden = false;
public:
    ViewComplex()
    : _lbl("This is a label:")
    , _lblQuant("Kol:")
    , _quant(td::decimal3)
    , _lbl2("This is another label:")
    , _lblPrice("Cij:")
    , _price(td::decimal4)
    , _lblLong("Label for text edit (longest of all labels - just for demo)")
    , _lblCmb("Cmb lbl:")
    , _lblDBCmb("DB Cmb:")
    , _dbCmb(td::decimal3)
    , _checkBox("This is a check box (click me please several times)!")
    , _lblSlider("Slider:")
    , _lbl4("Prog. Ind:")
    , _lblDE("Date edit:")
    , _lblTE("Time edit:")
    //, _lblDTE("Date-time edit:")
    , _lblCP("Color picker:")
    , _hlBtns(7)
    , _btn1("Clean")
    , _btn2("Change lbl")
    , _btnHideRow("Hide row")
    , _btn3("Set Quant")
    , _btn4("Set price")
    , _btn5("OK")
    , _gl(10,4)
    {
        //_dateTimeEdit.enableMessaging();
        _timeEdit.enableMessaging();
        _dateEdit.enableMessaging();
        //_lineEditInfo.setAsReadOnly();
        _textEdit.setAsReadOnly();
        _cmb.addItem("First item");
        _cmb.addItem("Second item");
        _cmb.selectIndex(1);
        //_price.setDelegates(std::bind, std::bind)
        
        td::Variant key;
        td::Decimal3 dVal = 1005.768;
        key = dVal;
        _dbCmb.addItem("strVal = 1005.768", key);
        dVal =500.5;
        key = dVal;
        _dbCmb.addItem("strVal = 500.5", key);
        _dbCmb.selectIndex(0);
        
        _hlBtns.append(_btn1);
        _hlBtns.appendSpacer();
        _hlBtns.append(_btnHideRow);
        _hlBtns.append(_btn2);
        _hlBtns.append(_btn3);
        _hlBtns.append(_btn4);
        _hlBtns.append(_btn5);
        _btn5.setAsDefault();
#ifdef USE_COORDINATES
        _gl.insert(0,0, _lbl);
        _gl.insert(0,1, _lineEdit);
        _gl.insert(0,2, _lblQuant);
        _gl.insert(0,3, _quant);
        _gl.insert(1,0, _lbl2);
        _gl.insert(1,1, _lineEdit2);
        _gl.insert(1,2, _lblPrice);
        _gl.insert(1,3, _price);
        _gl.insert(2,0, _lblCmb);
        _gl.insert(2,1, _cmb);
        _gl.insert(2,2, _lblDBCmb);
        _gl.insert(2,3, _dbCmb);
        _gl.insert(3,0, _checkBox, 2);
        _gl.insert(3,2, _lblSlider);
        //_slider.setValue(0.5, true);
        _gl.insert(3,3, _slider);
        _gl.insert(4,0, _lbl4);
        _gl.insert(4,1, _progressInd, 0);
        _gl.insert(5,0, _lblLong, 0, td::HAlignment::Center);
        _gl.insert(6,0, _lblDE)
        //_gl.insert(6,1, _dateTimeEdit);
        _gl.insert(6,2, _lblCP)
        _gl.insert(6,3, _colorPicker)
        _gl.insert(7,0, _textEdit, 0);
        _gl.insert(8,0, _hlBtns, 0);
#else
        gui::GridComposer gc(_gl);
        gc.appendRow(_lbl); gc.appendCol(_lineEdit); gc.appendCol(_lblQuant); gc.appendCol(_quant);
        gc.appendRow(_lbl2); gc.appendCol(_lineEdit2); gc.appendCol(_lblPrice); gc.appendCol(_price);
        gc.appendRow(_lblCmb); gc.appendCol(_cmb); gc.appendCol(_lblDBCmb); gc.appendCol(_dbCmb);
        gc.appendRow(_checkBox, 2);            gc.appendCol(_lblSlider); gc.appendCol(_slider);
        gc.appendRow(_lbl4); gc.appendCol(_progressInd, 0);
        gc.appendRow(_lblLong, 0, td::HAlignment::Center);
        gc.appendRow(_lblDE); gc.appendCol(_dateEdit); gc.appendCol(_lblTE); gc.appendCol(_timeEdit);
        gc.appendRow(_lblCP); gc.appendCol(_colorPicker);
        
        gc.appendRow(_textEdit, 0);
        gc.appendRow(_hlBtns, 0);
#endif
        
        setLayout(&_gl);
        _slider.setValue(0.2);
        _checkBox.setChecked(true);
    }
protected:
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        auto pAI = aiDesc.getActionItem();
        
        td::String msgText("Handling onActionItem");
        td::String informativeText;
        informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);
        
       showAlert(msgText, informativeText);
        return true;
    }
    
    bool onChangedValue(gui::ColorPicker* pCP) override
    {
        if (pCP == &_colorPicker)
        {
            td::ColorID colorID = pCP->getValue();
            
            td::String str = toString(colorID);
            _textEdit.appendString("CP new val: ");
            _textEdit.appendString(str);
            _textEdit.appendString("\n");
            return true;
        }
        return false;
    }
    
//    bool onChangedValue(gui::DateTimeEdit* pDTE)
//    {
//        if (pDTE == &_dateTimeEdit)
//        {
//            td::DateTime dt = pDTE->getValue();
//            auto str = dt.toString();
//            _textEdit.appendString("DT new val: ");
//            _textEdit.appendString(str);
//            _textEdit.appendString("\n");
//            return true;
//        }
//        return false;
//    }
    
    bool onChangedValue(gui::DateEdit* pDE) override
    {
        if (pDE == &_dateEdit)
        {
            td::Date dVal = pDE->getValue();
            auto str = dVal.toString();
            _textEdit.appendString("Date new val: ");
            _textEdit.appendString(str);
            _textEdit.appendString("\n");
            return true;
        }
        return false;
    }
    
    bool onChangedValue(gui::TimeEdit* pTE) override
    {
        if (pTE == &_timeEdit)
        {
            td::Time tVal = pTE->getValue();
            auto str = tVal.toString();
            _textEdit.appendString("Time new val: ");
            _textEdit.appendString(str);
            _textEdit.appendString("\n");
            return true;
        }
        return false;
    }
    
    bool onChangedValue(gui::Slider* pSlider) override
    {
        if (pSlider == &_slider)
        {
            double val = _slider.getValue();
            _progressInd.setValue(val);
            return true;
        }
        return false;
    }
    
    bool onClick(gui::CheckBox* pBtn) override
    {
        if (pBtn == &_checkBox)
        {
            if (_checkBox.isChecked())
                _textEdit.appendString("Check box is CHECKED!\n");
            else
                _textEdit.appendString("Check box is UNCHECKED!\n");
            return true;
        }
        return false;
    }
    
    bool onClick(gui::Button* pBtn) override
    {
        if (pBtn == &_btnHideRow)
        {
            bool bHide = true;
            if (_lbl2.isHidden())
                bHide = false;
            _lbl2.hide(bHide, false);
            _lblPrice.hide(bHide, false);
            _price.hide(bHide, false);
            _lineEdit2.hide(bHide, false);
            _textEdit.hide(bHide, true);
            return true;
        }
        if (pBtn == &_btn1)
        {
            _textEdit.setValue("");
            return true;
        }
        
        if (pBtn == &_btn2)
        {
            _lbl2.setTitle("Drugi title:");
            //_btn5.setTitle("OK - changed");
            return true;
        }
        if (pBtn == &_btn3)
        {
            td::Decimal3 d3(1000.5);
            td::Variant val(d3);
            
            _quant.setValue(val, false);
            td::DateTime dt(2000,1,1, 19, 0, 0, 0);
           // _dateTimeEdit.setValue(dt, true);
            td::ColorID color = td::ColorID::Red;
            _colorPicker.setValue(color, true);
            
            td::String strInfo = getResText("infoTxt");
            _textEdit.appendString(strInfo);
            return true;
        }
        
        if (pBtn == &_btn4)
        {
            
            td::Decimal4 d4(123456.789);
            td::Variant val(d4);
            
            td::DateTime now;
            now.now();
            //_dateTimeEdit.setValue(now);
            _price.setValue(val, true);
            _btn5.setTitle("OK - changed");
            return true;
        }
        
        if (pBtn == &_btn5)
        {
            _isLblHidden = !_isLblHidden;
            _lblLong.hide(_isLblHidden, true);
            return true;
        }
        
        td::String msgText("Handling onButtonClick");
        td::String informativeText;
        informativeText.format("Handled onButtonClick for button with title='%s'", pBtn->getTitle().c_str());         
        showAlert(msgText, informativeText);
        
        return true;
    }
    
    bool onChangedSelection(gui::ComboBox* pCmb) override
    {
        if (pCmb == &_cmb)
        {
            
            int iSel = _cmb.getSelectedIndex();
            td::String selStr = _cmb.getSelectedText();
            td::String str;
            str.format("ComboBox cs:%d->%s\n", iSel, selStr.c_str());
            _textEdit.appendString(str);
            return true;
        }
        
        return false;
    }
    
    bool onChangedSelection(gui::DBComboBox* pCmb) override
    {
       
        if (pCmb == &_dbCmb)
        {
            int iSel = _dbCmb.getSelectedIndex();
            td::String selStr = _dbCmb.getSelectedText();
            td::String str;
            str.format("DBComboBox cs:%d->%s\n", iSel, selStr.c_str());
            _textEdit.appendString(str);
            td::Variant key;
            _dbCmb.getValue(key);
            _quant.setValue(key);
            return true;
        }
        return false;
    }
    
    bool onFinishEdit(gui::LineEdit* pCtrl) override
    {
        if (pCtrl == &_lineEdit)
        {
            _textEdit.appendString("LineEdit lost focus\n");
            return true;
        }
        else if (pCtrl == &_quant)
        {
            _textEdit.appendString("Quant lost focus\n");
            return true;
        }
        else if (pCtrl == &_lineEdit2)
        {
            _textEdit.appendString("LineEdit2 lost focus\n");
            return true;
        }
        if (pCtrl == &_price)
        {
            _textEdit.appendString("Price lost focus\n");
            return true;
        }
        
        return false;
    }
    
    bool onBeginEdit(gui::LineEdit* pCtrl) override
    {
        if (pCtrl == &_lineEdit)
        {
            //_textEdit.setValue("Line edit began editing!");
            _textEdit.appendString("LineEdit <b>begin</b> editing\n");
            return true;
        }
        else if (pCtrl == &_quant)
        {
            _textEdit.appendString("Quant begin editing\n");
            return true;
        }
        else if (pCtrl == &_lineEdit2)
        {
            _textEdit.appendString("_lineEdit2 begin editing\n");
            return true;
        }
        if (pCtrl == &_price)
        {
            _textEdit.appendString("Price begin editing\n");
            return true;
        }
        return true;
    }
};
