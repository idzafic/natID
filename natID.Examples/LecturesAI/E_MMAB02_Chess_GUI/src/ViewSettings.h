//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//
#pragma once
#include <gui/PopupView.h>
#include <gui/Label.h>
#include <gui/ComboBox.h>
#include <gui/CheckBox.h>
#include <gui/LineEdit.h>
#include <gui/Slider.h>
#include <gui/HorizontalLayout.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <gui/ToolBar.h>
#include "ViewChessBoard.h"
#include "Constants.h"

class ViewSettings : public gui::PopupView
{
    ViewSettings() = delete;
protected:
    
    ViewChessBoard* _pViewChessBoard;
    gui::ToolBar* _pMainTB;
    gui::Image _imgOnPopoverButton; //will be drawn on Toolbar button
    gui::Label _lblLangNow;
    gui::LineEdit _leLang;
    gui::Label _lblLangNew;
    gui::ComboBox _cmbLangs;
    gui::CheckBox _chbToolbarIconsAndLabels;

    gui::GridLayout _gl;
    int _initialLangSelection;
public:
    ViewSettings(ViewChessBoard* pViewChessBoard, gui::ToolBar* pMainTB)
    : _pViewChessBoard(pViewChessBoard)
    , _pMainTB(pMainTB)
    , _imgOnPopoverButton(":settings")
    , _lblLangNow(tr("lblLang"))
    , _lblLangNew(tr("lblLang2"))
    , _chbToolbarIconsAndLabels(tr("chbTBIcsAndLbls"))
    , _gl(3,2)
    {
        setPopoverButtonImage(&_imgOnPopoverButton); // tell PopupView what to draw on popover button
        
        gui::Application* pApp = getApplication();
        auto appProperties = pApp->getProperties();
        assert (appProperties);
        td::String strTr = appProperties->getValue("translation", "BA");
        
        _leLang.setAsReadOnly();
        int newLangIndex = 0;
        //populate combo
        auto& langs = getSupportedLanguages();
        auto currTranslationIndex = getTranslationLanguageIndex();
        
        auto& strCurrentLanguage = langs[currTranslationIndex].getDescription();
        _leLang.setText(strCurrentLanguage);
        
        int i = 0;
        for (const auto& lang : langs)
        {
            if (lang.getExtension() == strTr)
                newLangIndex = i;
            _cmbLangs.addItem(lang.getDescription());
            ++i;
        }
        
        bool showLabels = appProperties->getTBLabelVisibility(mu::IAppProperties::ToolBarType::Main, true);
        _chbToolbarIconsAndLabels.setChecked(showLabels);
      
        _cmbLangs.selectIndex(newLangIndex);
        _initialLangSelection = newLangIndex;
        
        //event handlers
        _cmbLangs.onChangedSelection([this, appProperties](){
            td::String strTr = getTranslationExt();
            if (strTr.length() > 0)
                appProperties->setValue("translation", strTr); //write translation info back to properties
            if (isRestartRequired())
            {
                close();
                gui::Alert::showYesNoQuestion(tr("RestartRequired"), tr("RestartRequiredInfo"), tr("Restart"), tr("DoNoRestart"), [this] (gui::Alert::Answer answer) {
                    if (answer == gui::Alert::Answer::Yes)
                    {
                        auto pApp = getApplication();
                        //clean up and save here whatever you need, the application is about to terminate... and restart fresh....
                        pApp->restart();
                    }
                });
            }
        });
        

        
        //handler for checkbox that controls labels (this one requires immediate action
        //to show/hide labels so that user can see the effects immediatelly
        _chbToolbarIconsAndLabels.onClick([this]()
                                          {
            if (_pMainTB)
            {
                bool bShowLabelsOnMTB = _chbToolbarIconsAndLabels.isChecked();
                _pMainTB->showLabels(bShowLabelsOnMTB);
            }
        });
        
        // populate grid
        gui::GridComposer gc(_gl);
        gc.appendRow(_lblLangNow) << _leLang;
        gc.appendRow(_lblLangNew) << _cmbLangs;;
        gc.appendRow(_chbToolbarIconsAndLabels, 0);
        
        setLayout(&_gl);
    }
    
    td::String getTranslationExt()
    {
        td::String strExt;
        int currSelection = _cmbLangs.getSelectedIndex();
        if (currSelection >= 0)
        {
            auto& langs = getSupportedLanguages();
            strExt = langs[currSelection].getExtension();
        }
        
        return strExt;
    }
    
    bool isRestartRequired() const
    {
        auto selectedLanguageIndex = _cmbLangs.getSelectedIndex();
        return (_initialLangSelection != selectedLanguageIndex);
    }

};
