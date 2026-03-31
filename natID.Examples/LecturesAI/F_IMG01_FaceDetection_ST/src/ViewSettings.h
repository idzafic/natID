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
#include "ViewImageDetector.h"
#include "Constants.h"
#include <gui/Application.h>

class ViewSettings : public gui::PopupView
{
    ViewSettings() = delete;
protected:
    
    ViewImageDetector* _pViewImageDetector;
    gui::ToolBar* _pMainTB;
    gui::Image _imgOnPopoverButton; //will be drawn on Toolbar button
    gui::Label _lblLangNow;
    gui::LineEdit _leLang;
    gui::Label _lblLangNew;
    gui::ComboBox _cmbLangs;
    gui::CheckBox _chbToolbarIconsAndLabels;
    gui::Label _lblNoise;
    gui::Label _lblBrightnes;
    gui::Label _lblCorrTreshold;
    gui::Label _lblFinalTreshold;
    gui::Label _lblMaxRndImages;
    gui::Slider _slNoise;
    gui::Slider _slBrightnes;
    gui::Slider _slCorrTreshold;
    gui::Slider _slFinalTreshold;
    gui::Slider _slMaxRndImages;
    
    gui::GridLayout _glSliders;
    gui::GridLayout _mainLayout;
    int _initialLangSelection;
    
    float _brightness = cBrightness;
    float _correllationTresh = cCorrelatoinTreshold;
    float _finalTresh = cFinalTreshold;
    int _maxRndImages = cMaxRndImages;
    td::BYTE _noise = cNoiseLevel; //no noise
public:
    ViewSettings(ViewImageDetector* pViewImageDetector, gui::ToolBar* pMainTB)
    : _pViewImageDetector(pViewImageDetector)
    , _pMainTB(pMainTB)
    , _imgOnPopoverButton(":settings")
    , _lblLangNow(tr("lblLang"))
    , _lblLangNew(tr("lblLang2"))
    , _lblNoise(tr("lblNoise"))
    , _lblBrightnes(tr("lblBrightnes"))
    , _lblCorrTreshold(tr("lblTreshCorr"))
    , _lblFinalTreshold(tr("lblTreshFinal"))
    , _lblMaxRndImages(tr("lblMaxRndImages"))
    , _chbToolbarIconsAndLabels(tr("chbTBIcsAndLbls"))
    , _glSliders(5,2)   //labels and sliders
    , _mainLayout(4,2)  //all together
    {
        _glSliders.setMargins(0, 0);
        setPopoverButtonImage(&_imgOnPopoverButton); // tell PopupView what to draw on popover button
        _slNoise.setRange(0, 128);
        _slCorrTreshold.setRange(0.5, 0.98);
        _slBrightnes.setRange(-0.4, 0.4);
        _slFinalTreshold.setRange(1.2, 1.8);
        _slMaxRndImages.setRange(5, 5000);
        
        gui::Application* pApp = getApplication();
        auto appProperties = pApp->getProperties();
        assert (appProperties);
        
        _noise = appProperties->getValue("noise", _noise);
        _slNoise.setValue(double(_noise));
        
        _brightness = appProperties->getValue("brightness", _brightness);
        _slBrightnes.setValue(double(_brightness));
        
        _correllationTresh = appProperties->getValue("tresCorr", _correllationTresh);
        _slCorrTreshold.setValue(double(_correllationTresh));
        
        _finalTresh = appProperties->getValue("tresFinal", _finalTresh);
        _slFinalTreshold.setValue(double(_finalTresh));
        
        _maxRndImages = appProperties->getValue("maxRndImages", _maxRndImages);
        _slMaxRndImages.setValue(double(_maxRndImages));
        

        _slNoise.onChangedValue([this, appProperties] (){
            _noise = td::BYTE(_slNoise.getValue());
            appProperties->setValue("noise", _noise);
            _pViewImageDetector->modifyCase(_noise, _brightness);
        });
        
        _slBrightnes.onChangedValue([this, appProperties] (){
            _brightness = float(_slBrightnes.getValue());
            appProperties->setValue("brightness", _brightness);
            _pViewImageDetector->modifyCase(_noise, _brightness);
        });
        
        _slMaxRndImages.onChangedValue([this, appProperties] (){
            _maxRndImages = int(_slMaxRndImages.getValue());
            appProperties->setValue("maxRndImages", _maxRndImages);
            _pViewImageDetector->reset(_maxRndImages);
        });
        
        _slCorrTreshold.onChangedValue([this, appProperties] (){
            _correllationTresh = float(_slCorrTreshold.getValue());
            appProperties->setValue("tresCorr", _correllationTresh);
            _pViewImageDetector->modifySolutionTresholds(_correllationTresh, _finalTresh);
        });
        
        _slFinalTreshold.onChangedValue([this, appProperties] (){
            _finalTresh = float(_slFinalTreshold.getValue());
            appProperties->setValue("tresFinal", _finalTresh);
            _pViewImageDetector->modifySolutionTresholds(_correllationTresh, _finalTresh);
        });
        
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
        auto maxW = _cmbLangs.getWidthToFitLongestItem();
        _cmbLangs.setSizeLimits(maxW, gui::Control::Limit::UseAsMin);
        
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
        gui::GridComposer gc(_mainLayout);
        gc.appendRow(_lblLangNow) << _leLang;
        gc.appendRow(_lblLangNew) << _cmbLangs;;
        gc.appendRow(_chbToolbarIconsAndLabels, 0);
        
        //form 5x2 grid layout for sliders and their labels
        gui::GridComposer gcSliders(_glSliders);
        gcSliders.appendRow(_lblNoise) << _slNoise;
        gcSliders.appendRow(_lblBrightnes) << _slBrightnes;
        gcSliders.appendRow(_lblCorrTreshold) << _slCorrTreshold;
        gcSliders.appendRow(_lblFinalTreshold) << _slFinalTreshold;
        gcSliders.appendRow(_lblMaxRndImages) << _slMaxRndImages;
        
        gc.appendRow(_glSliders, 0);
        
        setLayout(&_mainLayout);
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
