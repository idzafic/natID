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

class ViewSettings : public gui::View
{
protected:
    gui::Label _lblLangNow;
    gui::LineEdit _leLang;
    gui::Label _lblLangNew;
    gui::ComboBox _cmbLangs;
    gui::CheckBox _chbToolbarIconsAndLabels;
    gui::CheckBox _chbShowStatusBar;
    gui::StatusBar* _pStatusBar = nullptr;

    gui::GridLayout _gl;
    gui::ToolBar* _pMainTB = nullptr;
    gui::Size _sweeperSize;
    int _initialLangSelection;
public:
    ViewSettings()
    : _lblLangNow(tr("lblLang"))
    , _lblLangNew(tr("lblLang2"))
    , _chbToolbarIconsAndLabels(tr("chbTBIcsAndLbls"))
    , _chbShowStatusBar(tr("showStatusBar"))
    , _gl(4,2)
    {
        auto app = getApplication();
        gui::Window* mainWnd = (gui::Window*) app->getMainWnd();
        _pStatusBar = const_cast<gui::StatusBar*>(mainWnd->getStatusBar());
        
        auto appProperties = getAppProperties();
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


                //combo is full, let's check its maximum size
        //applying the following 3 lines will cause that the dialog becomes fixed sized (non-resizable)
        auto maxCmbWidth = _cmbLangs.getWidthToFitLongestItem();
        _cmbLangs.setSizeLimits((td::UINT2) maxCmbWidth, gui::Control::Limit::Fixed);
        //set same width to _leLang (it holds one of the strings from the combo)
        _leLang.setSizeLimits((td::UINT2) maxCmbWidth, gui::Control::Limit::Fixed);
        
        bool showLabels = appProperties->getTBLabelVisibility(mu::IAppProperties::ToolBarType::Main, true);
        _chbToolbarIconsAndLabels.setChecked(showLabels);
        _cmbLangs.selectIndex(newLangIndex);
        _initialLangSelection = newLangIndex;
        
        bool showStatusBar = false;
        if (_pStatusBar)
        {
            showStatusBar = _pStatusBar->isVisible();
        }
//        appProperties->getValue("showStatusBar", true);
        _chbShowStatusBar.setChecked(showStatusBar);
        // populate grid
        gui::GridComposer gc(_gl);
        gc.appendRow(_lblLangNow) << _leLang;
        gc.appendRow(_lblLangNew) << _cmbLangs;
        gc.appendRow(_chbToolbarIconsAndLabels, 0);
        gc.appendRow(_chbShowStatusBar, 0);
        setLayout(&_gl);
        
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
    
    bool isRestartRequired() const
    {
        auto selectedLanguageIndex = _cmbLangs.getSelectedIndex();
        return (_initialLangSelection != selectedLanguageIndex);
    }

    void setMainTB(gui::ToolBar* pTB)
    {
        _pMainTB = pTB;
    }
    
    bool isStatusBarVisible() const
    {
        return _chbShowStatusBar.isChecked();
    }
};
