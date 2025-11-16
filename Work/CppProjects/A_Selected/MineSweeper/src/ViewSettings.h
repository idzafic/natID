//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
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
#include <gui/ComboBox.h>
#include <gui/BaseView.h>
#include <gui/StatusBar.h>

class ViewSettings : public gui::View
{
protected:
    gui::Label _lblLangNow;
    gui::LineEdit _leLang;
    gui::Label _lblLangNew;
    gui::ComboBox _cmbLangs;
    gui::Label _lblTheme;
    gui::ComboBox _cmbTheme;
    gui::CheckBox _chbToolbarIconsAndLabels;
    gui::CheckBox _chbShowStatusBar;
    gui::CheckBox _chbUseCombo;
    gui::StatusBar* _pStatusBar = nullptr;

    gui::GridLayout _mainLayout;
    gui::ToolBar* _pMainTB = nullptr;
    gui::Size _sweeperSize;
    int _initialLangSelection = 0;
    bool _initialUseCombo = false;
public:
    ViewSettings(const gui::Size& sweeperSize, const gui::StatusBar* pSB, bool bAnimateTransition)
    : _lblLangNow(tr("lblLang"))
    , _lblLangNew(tr("lblLang2"))
    , _lblTheme(tr("theme"))
    , _chbToolbarIconsAndLabels(tr("chbTBIcsAndLbls"))
    , _chbShowStatusBar(tr("showStatusBar"))
    , _chbUseCombo(tr("chbUseCombo"))
    , _pStatusBar(const_cast<gui::StatusBar*>(pSB))
    , _sweeperSize(sweeperSize)
    , _mainLayout(6,2)
    {
        _chbUseCombo.setChecked(bAnimateTransition);
        
        auto appProperties = getAppProperties();
        assert (appProperties);
        
        
        bool bShowComboInTB = appProperties->getValue("showComboInTB", false);
        _chbUseCombo.setChecked(bShowComboInTB);
        _initialUseCombo = bShowComboInTB;
        
        _leLang.setAsReadOnly();
//        int newLangIndex = 0;
        //populate combo
        auto& langs = getSupportedLanguages();
        auto currTranslationIndex = getTranslationLanguageIndex();
        
        auto& strCurrentLanguage = langs[currTranslationIndex].getDescription();
        _leLang.setText(strCurrentLanguage);
        
        const td::String themeNames[]{"Classic", "Modern", "Modern2"};
        td::String currentTheme = appProperties->getValue("Theme", themeNames[1]);

        for(auto &name : themeNames){
            _cmbTheme.addItem(name);
            if(name == currentTheme)
                _cmbTheme.selectIndex(_cmbTheme.getNoOfItems() - 1);
        }




        td::String strTr = appProperties->getValue("translation", "BA");
        int i = 0;
        for (const auto& lang : langs)
        {
            if (lang.getExtension() == strTr)
                _initialLangSelection = i;
            _cmbLangs.addItem(lang.getDescription());
            ++i;
        }
        //combo is full, let's check its maximum size
        //applying the following 4 lines will cause that the dialog becomes fixed sized (non-resizable)
        auto maxCmbWidth = std::max(_cmbLangs.getWidthToFitLongestItem(), _cmbTheme.getWidthToFitLongestItem());
        _cmbLangs.setSizeLimits((td::UINT2) maxCmbWidth, gui::Control::Limit::Fixed);
        //set same width to _leLang (it holds one of the strings from the combo)
        _leLang.setSizeLimits((td::UINT2) maxCmbWidth, gui::Control::Limit::Fixed);
        _cmbTheme.setSizeLimits(maxCmbWidth, gui::Control::Limit::Fixed);
        
        bool showLabels = appProperties->getTBLabelVisibility(mu::IAppProperties::ToolBarType::Main, true);
        _chbToolbarIconsAndLabels.setChecked(showLabels);
        
        _cmbLangs.selectIndex(_initialLangSelection);
        
        bool showStatusBar = false;
        if (_pStatusBar)
        {
            showStatusBar = _pStatusBar->isVisible();
        }
//        appProperties->getValue("showStatusBar", true);
        _chbShowStatusBar.setChecked(showStatusBar);
        // populate grid
        gui::GridComposer gc(_mainLayout);
        gc.appendRow(_lblLangNow) << _leLang;
        gc.appendRow(_lblLangNew) << _cmbLangs;
        gc.appendRow(_lblTheme) << _cmbTheme;
        gc.appendRow(_chbToolbarIconsAndLabels, 0);
        gc.appendRow(_chbUseCombo, 0);
        gc.appendRow(_chbShowStatusBar, 0);
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
        
        _chbShowStatusBar.onClick([this]()
                                  {
                                      if (_pStatusBar)
                                      {
                                          bool showStatusBar = _chbShowStatusBar.isChecked();
                                          _pStatusBar->show(showStatusBar);
                                          auto wnd = _pStatusBar->getParentWindow();
                                          wnd->adjustSizeToContentSize(_sweeperSize);
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
    
    void setMainTB(gui::ToolBar* pTB)
    {
        _pMainTB = pTB;
    }
    
    bool isStatusBarVisible() const
    {
        return _chbShowStatusBar.isChecked();
    }
    
    bool isRestartRequired() const
    {
        auto selectedLanguageIndex = _cmbLangs.getSelectedIndex();
        bool useCombo = _chbUseCombo.isChecked();
        return (_initialLangSelection != selectedLanguageIndex || _initialUseCombo != useCombo);
    }
    
    bool useCombo() const
    {
        return _chbUseCombo.isChecked();
    }

    td::String getTheme() const
    {
        return _cmbTheme.getSelectedText();
    }

    ~ViewSettings(){
        
    }

};
