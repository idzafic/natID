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
#include <gui/Alert.h>
#include <gui/GridComposer.h>
#define CONTEXT_MENU_ID 100

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
    gui::Button _btnShowWindow;
    gui::Button _btnGetDT;
    gui::TextEdit _textEdit;
    gui::GridLayout _gl;

protected:
    void updateIznos()
    {
        td::Variant kol = _kol.getValue();
        td::Variant cij = _cij.getValue();

        td::Variant dblIznos = kol * cij;
        td::Decimal2 dec2(dblIznos.r8Val());

        td::Variant iznos(dec2);

        _iznos.setValue(iznos, false);
        td::Decimal2 maxIznos(100.0);
        if (dec2 > maxIznos)
        {
            _lblIzn.setTitle("Veoma skup artikal:");
            //_cbKupiti.setTitle("Ne kupovati");
        }
        else
            _lblIzn.setTitle("Iznos ok:");
    }

    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        auto pAI = aiDesc.getActionItem();

        if (menuID == CONTEXT_MENU_ID)
        {
            showAlert("Context menu", "Ove of window context menu options selected");
            return true;
        }
        return false;
    }

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
    , _hl(5)
    , _btnGetText("Daj text")
    , _btnClearText("Brisi")
    , _btnGetDT("Date&Time->TE")
    , _btnShowWindow("Window...")
    , _gl(8,4)
    {
        _btnShowWindow.setType(gui::Button::Type::Constructive);

        //set context menu (with id=100) to the show window button
        _btnShowWindow.setContextMenuID(CONTEXT_MENU_ID);

        _btnClearText.setType(gui::Button::Type::Destructive);

        _lineEdit.setToolTip("Ovo je tooltip");
        _lineEdit2.setToolTip("Ovo je drugi tooltip....");
        _textEdit.setToolTip("Ovo je text edit za log");
        _iznos.setAsReadOnly();
        _comboBox.addItem("Kafa");
        _comboBox.addItem("Supa");
        _comboBox.addItem("Neki artikal");
        _comboBox.selectIndex(0);
        
        _btnGetText.setAsDefault();
        
        gui::GridComposer gc(_gl); //composer transfers controls to grid layout
        //first row
        gc.appendRow(_lbl) << _lineEdit << _lbl2 << _lineEdit2;
        //second row
        gc.appendRow(_lblKol) << _kol << _lblCij << _cij;
        //third row
        gc.appendRow(_lblIzn) << _iznos << _lblKupiti << _cbKupiti;
        //fourth row
        gc.appendRow(_lblSlider) << _slider; gc.appendCol(_progIndicator, 0); //last one spans until last column
        //5th row
        gc.appendRow(_lblCB) << _comboBox;
        //6th row
        gc.appendRow(_lblDate) << _date << _lblTime << _time;

        //populate horizontal layout
        _hl << _btnGetDT << _btnShowWindow;
        _hl.appendSpacer();
        _hl << _btnClearText << _btnGetText;
        
        //7th row
        gc.appendRow(_hl, 0); //spans over all columns
        //8th row
        gc.appendRow(_textEdit, 0); //spans over all columns

        //add lambda handlers
        _kol.onBeginEdit([this]()
            {
                _textEdit.appendString("Kolicina Begin edit! ");
                mu::dbgLog("Kolicina Begin Edit");
            });

        _kol.onFinishEdit([this]()
            {
                _textEdit.appendString("Kolicina Finish edit: ");
                mu::dbgLog("Kolicina Finish Edit");
                updateIznos();
            });

        _cij.onBeginEdit([this]()
            {
                _textEdit.appendString("Cijena Begin edit! ");
                mu::dbgLog("Cijena Begin Edit");
            });

        _cij.onFinishEdit([this]()
            {
                _textEdit.appendString("Cijena Finish edit: ");
                mu::dbgLog("Cijena Finish Edit");
                updateIznos();
            });

        _cbKupiti.onClick([this]()
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
            });

        _slider.onChangedValue([this]()
            {
                double val = _slider.getValue();
                _progIndicator.setValue(val);
            });

        _comboBox.onChangedSelection([this]()
            {
                td::String strSel = _comboBox.getSelectedText();
                _textEdit.appendString(strSel);
            });

        _btnShowWindow.onClick([this]()
            {
                gui::Window* parentWnd = getParentWindow();
                assert(parentWnd);
                td::UINT4 wndID = 100;
                gui::Window* pAllreadyAttachedWnd = parentWnd->getAttachedWindow(wndID);
                if (pAllreadyAttachedWnd)
                    pAllreadyAttachedWnd->setFocus();
                else
                {
                    gui::Window* newWindow = new gui::Window(gui::Size(400, 300), parentWnd, wndID);
                    newWindow->open(gui::Window::FrameSize::UseSpecified);
                }
            });

        _btnGetText.onClick([this]()
            {
                td::String str = _textEdit.getText();
                const char* pStr = str.c_str();
                int k = 0;
                ++k;
            });

        _btnClearText.onClick([this]()
            {
                _textEdit.clean();
            });

        _btnGetDT.onClick([this]() //ili je bolje bez parametra: _btnGetDT.onClick([this]()
            {
                td::Date dateVal = _date.getValue();
                td::Time timeVal = _time.getValue();

                td::String strDate = dateVal.toString();
                _textEdit.appendString(" Date=");
                _textEdit.appendString(strDate);
                td::String strTime = timeVal.toString();
                _textEdit.appendString(" Time=");
                _textEdit.appendString(strTime);
            });
        setLayout(&_gl);
    }
};
