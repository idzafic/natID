//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/ComboBox.h>
#include <cnt/SafeFullVector.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>

class ViewSettings : public gui::View
{

protected:

    gui::ToolBar* _pMainTB = nullptr;

    gui::Label _lblLangNow;
    gui::LineEdit _leLang;
    gui::Label _lblLangNew;
    gui::ComboBox _cmbLangs;
    gui::GridLayout _mainLayout;

    gui::CheckBox _toolbarVisible;


    gui::Label _lblToolbarIconSize;
    gui::ComboBox _toolbarIconSize;

    gui::CheckBox _toolbarShowLbls;

public:
    ViewSettings(gui::ToolBar* pMainTB = nullptr)
        : _pMainTB(pMainTB)
        , _lblLangNow(tr("lblLang"))
        , _lblLangNew(tr("lblLang2"))
        , _toolbarVisible(tr("TBVisible"))
        , _lblToolbarIconSize(tr("TBIconSize"))
        , _toolbarShowLbls(tr("TBShowLbl"))
        , _mainLayout(5,2)
    {
        gui::Application* pApp = getApplication();
        auto settings = pApp->getProperties();
        
        td::String strTr = settings->getValue("translation", "BA");
        
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
        _cmbLangs.selectIndex(newLangIndex);


        _toolbarIconSize.addItem(tr("Small"));
        _toolbarIconSize.addItem(tr("Regular"));
        _toolbarIconSize.addItem(tr("SystemDefault"));

        auto TBIconSz = settings->getValue("TBIconSz", 2);
        _toolbarIconSize.selectIndex(TBIconSz);

        _toolbarVisible.setValue(settings->getValue("TBVisible", true));
        _toolbarShowLbls.setValue(settings->getValue("TBLbls", true));


        
        // populate grid
        gui::GridComposer gc(_mainLayout);
        gc.appendRow(_lblLangNow) << _leLang;
        gc.appendRow(_lblLangNew) << _cmbLangs;
        gc.appendRow(_toolbarVisible);
        gc.appendRow(_lblToolbarIconSize) << _toolbarIconSize;
        gc.appendRow(_toolbarShowLbls);
        setLayout(&_mainLayout);
    }
    
    td::String getTranslationExt() const
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

    auto getTBIconSize() const
    {
        return static_cast<gui::ToolBar::IconSize>(_toolbarIconSize.getSelectedIndex());
    }
    auto isTBVisible() const
    {
        return _toolbarVisible.getValue();
    }
    auto getTBShowLabels() const
    {
        return _toolbarShowLbls.getValue();
    }
public:
    virtual bool onDialogButton(gui::Dialog::Button::ID btnID, gui::Button* pButton)
    {
        return true;
    }

    virtual bool onDialogClose(gui::Dialog* pDlg)
    {
        //update settings, user clicked ok
        if (pDlg->getClickedButtonID() == gui::Dialog::Button::ID::OK)
        {
            auto settings = getApplication()->getProperties();
            //update translation if needed
            td::String strTr = getTranslationExt();
            if (strTr.length() > 0)
            {
                //write translation info back to properties
                settings->setValue("translation", strTr);
            }

            if (_pMainTB)
            {
                //show or hide toolbar
                _pMainTB->show(isTBVisible());
                settings->setValue("TBVisible", isTBVisible());

                //adjust toolbar icon size if needed
                _pMainTB->setIconSize(getTBIconSize());
                settings->setValue("TBIconSz", static_cast<std::underlying_type_t<gui::ToolBar::IconSize>>(getTBIconSize()));

                //show or hide toolbar labels
                _pMainTB->showLabels(getTBShowLabels());
                settings->setValue("TBLbls", getTBShowLabels());
            }

        }
        return true;
    }
};
