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
#include <cnt/SafeFullVector.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <gui/ToolBar.h>

class ViewSettings : public gui::View
{

protected:
    gui::Label _lblLangNow;
    gui::LineEdit _leLang;
    gui::Label _lblLangNew;
    gui::ComboBox _cmbLangs;
    gui::CheckBox _chbToolbarIconsAndLabels;
    gui::CheckBox _cbShowNamesForEachTown;
    gui::CheckBox _cbShowRoadNames;
    gui::CheckBox _cbShowRoadLengths;
    gui::CheckBox _chbPlaySound;
    gui::GridLayout _mainLayout;
    gui::ToolBar* _pMainTB = nullptr;
    int _initialLangSelection;
public:
    ViewSettings()
    : _lblLangNow(tr("lblLang"))
    , _lblLangNew(tr("lblLang2"))
    , _chbToolbarIconsAndLabels(tr("chbTBIcsAndLbls"))
    , _cbShowNamesForEachTown(tr("chbShowTownNames"))
    , _cbShowRoadNames(tr("chbShowRoadNames"))
    , _cbShowRoadLengths(tr("cbShowRoadLengths"))
    , _chbPlaySound(tr("chbPlaySound"))
    , _mainLayout(7,2)
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
        
        bool showTownNames = appProperties->getValue("showTownNames", true);
        _cbShowNamesForEachTown.setChecked(showTownNames);
        
        bool showRoadNames = appProperties->getValue("showRoadNames", false);
        _cbShowRoadNames.setChecked(showRoadNames);
        
        bool playSound = appProperties->getValue("playSound", false);
        _chbPlaySound.setChecked(playSound);
        
        bool showRoadLengths = appProperties->getValue("showRoadLengths", true);
        _cbShowRoadLengths.setChecked(showRoadLengths);
        
        _cmbLangs.selectIndex(newLangIndex);
        _initialLangSelection = newLangIndex;
        
        // populate grid
        gui::GridComposer gc(_mainLayout);
        gc.appendRow(_lblLangNow) << _leLang;
        gc.appendRow(_lblLangNew) << _cmbLangs;
        gc.appendRow(_chbToolbarIconsAndLabels, 0);
        gc.appendRow(_cbShowNamesForEachTown, 0);
        gc.appendRow(_cbShowRoadNames, 0);
        gc.appendRow(_cbShowRoadLengths, 0);
        gc.appendRow(_chbPlaySound, 0);
        
        setLayout(&_mainLayout);
        
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
    
//    bool isTBWithLabels() const
//    {
//        return _chbToolbarIconsAndLabels.isChecked();
//    }
    
    bool showTownNames() const
    {
        return _cbShowNamesForEachTown.isChecked();
    }
    
    bool showRoadNames() const
    {
        return _cbShowRoadNames.isChecked();
    }
    
    bool showRoadLengths() const
    {
        return _cbShowRoadLengths.isChecked();
    }
    
    bool playSound() const
    {
        return _chbPlaySound.isChecked();
    }
    
    void setMainTB(gui::ToolBar* pTB)
    {
        _pMainTB = pTB;
    }

    bool isRestartRequired() const
    {
        auto selectedLanguageIndex = _cmbLangs.getSelectedIndex();
        return (_initialLangSelection != selectedLanguageIndex);
    }

};
