//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
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

class ViewSettings : public gui::View
{
    ViewSettings() = delete;
protected:
    ViewChessBoard* _pViewChessBoard;
    gui::ToolBar* _pMainTB;
    gui::Label _lblLangNow;
    gui::LineEdit _leLang;
    gui::Label _lblLangNew;
    gui::ComboBox _cmbLangs;
    gui::Label _lblSpeed;
    gui::Slider _slSpeed;
    gui::CheckBox _chbToolbarIconsAndLabels;
    gui::CheckBox _chbDarkQueen;
    gui::CheckBox _chbRandomSolution;
    gui::CheckBox _chbPlaySound;
    gui::HorizontalLayout _hlSlider;        //if setRange is not called, default range is [0,1]
    gui::GridLayout _gl;
    int _initialLangSelection;
public:
    ViewSettings(ViewChessBoard* pViewChessBoard, gui::ToolBar* pMainTB)
    : _pViewChessBoard(pViewChessBoard)
    , _pMainTB(pMainTB)
    , _lblLangNow(tr("lblLang"))
    , _lblLangNew(tr("lblLang2"))
    , _lblSpeed(tr("lblSpeed"))
    , _chbToolbarIconsAndLabels(tr("chbTBIcsAndLbls"))
    , _chbDarkQueen(tr("chbDarkQueen"))
    , _chbRandomSolution(tr("chbRandomSol"))
    , _chbPlaySound(tr("chbPlaySound"))
    , _hlSlider(2)
    , _gl(7,2)
    {
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

        auto& options = _pViewChessBoard->getOptions();
        
        bool darkQueen = appProperties->getValue("darkQueen", options.useDarkQueen);
        _chbDarkQueen.setChecked(darkQueen);
        
        bool playSound = appProperties->getValue("playSound", options.playSoundOnQueenPlacement);
        _chbPlaySound.setChecked(playSound);
    
        float speed = appProperties->getValue("speed", options.speed);
        _slSpeed.setValue(double(speed));
        
        bool generateRandomSolution = appProperties->getValue("randomSolution", options.generateRandomSolution);
        _chbRandomSolution.setValue(generateRandomSolution);
        
        _cmbLangs.selectIndex(newLangIndex);
        _initialLangSelection = newLangIndex;
        
        //event handlers
        _cmbLangs.onChangedSelection([this, appProperties](){
            td::String strTr = getTranslationExt();
            if (strTr.length() > 0)
                appProperties->setValue("translation", strTr); //write translation info back to properties
        });
        
        _slSpeed.onChangedValue([this, appProperties](){
            float speed = float(_slSpeed.getValue());
            appProperties->setValue("speed", speed);
            auto& options = _pViewChessBoard->getOptions();
            options.setSpeed(speed);
        });
        
        _chbDarkQueen.onClick([this, appProperties](){
            _pViewChessBoard->refresh(); //we need to redraw in this case
            bool darkQueen = _chbDarkQueen.isChecked();
            appProperties->setValue("darkQueen", darkQueen);
            auto& options = _pViewChessBoard->getOptions();
            options.useDarkQueen = darkQueen;
            
        });
        
        _chbRandomSolution.onClick([this, appProperties](){
            bool randomSolution = _chbRandomSolution.isChecked();
            appProperties->setValue("randomSolution", randomSolution);
            auto& options = _pViewChessBoard->getOptions();
            options.generateRandomSolution = randomSolution;
            
        });
        
        _chbPlaySound.onClick([this, appProperties](){
            bool playSound = _chbPlaySound.isChecked();
            appProperties->setValue("playSound", playSound);
            auto& options = _pViewChessBoard->getOptions();
            options.playSoundOnQueenPlacement = playSound;
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
        
        _hlSlider << _lblSpeed << _slSpeed;
        auto maxCmbWidth = _cmbLangs.getWidthToFitLongestItem();
        _cmbLangs.setSizeLimits((td::UINT2) maxCmbWidth, gui::Control::Limit::UseAsMin);
        // populate grid
        gui::GridComposer gc(_gl);
        gc.appendRow(_lblLangNow) << _leLang;
        gc.appendRow(_lblLangNew) << _cmbLangs;
        gc.appendRow(_hlSlider, 0);
        gc.appendRow(_chbToolbarIconsAndLabels, 0);
        gc.appendRow(_chbDarkQueen, 0);
        gc.appendRow(_chbRandomSolution, 0);
        gc.appendRow(_chbPlaySound, 0);
        
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
