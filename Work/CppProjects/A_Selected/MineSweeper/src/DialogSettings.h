//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Dialog.h>
#include "ViewSettings.h"
#include <gui/Application.h>

class DialogSettings : public gui::Dialog
{
protected:
    ViewSettings _viewSettings;
    
    bool onClick(Dialog::Button::ID btnID, gui::Button* pButton) override
    {
        if (btnID == Dialog::Button::ID::OK)
        {
            auto appProperties = getAppProperties();
            
            td::String strTr = _viewSettings.getTranslationExt();
            if (strTr.length() > 0)
            {
                //write translation info back to properties
                appProperties->setValue("translation", strTr);
            }
//            bool showLabels = _viewSettings.isTBWithLabels();            
//            appProperties->setTBLabelVisibility(mu::IAppProperties::ToolBarType::Main, showLabels);
            
            bool showStatusBar = _viewSettings.isStatusBarVisible();
            appProperties->setValue("showStatusBar", showStatusBar);
            
            bool useCombo = _viewSettings.useCombo();
            appProperties->setValue("showComboInTB", useCombo);
        }
        return true; //return true to close the dialog and to propagate to onClick(gui::Dialog* pDlg, td::UINT4 dlgID); of the parent frame
    }
public:
    DialogSettings(gui::Size sweeperSize, const gui::StatusBar* pSB, gui::Frame* pFrame, bool bAnimateTransition, td::UINT4 wndID)
    : gui::Dialog(pFrame, { {gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default},
                            {gui::Dialog::Button::ID::Cancel, tr("Cancel")}}, gui::Size(450, 100), wndID)
    , _viewSettings(sweeperSize, pSB, bAnimateTransition)
    {
        setTitle(tr("dlgSettings"));
        setCentralView(&_viewSettings);
    }
    
    ViewSettings& getView()
    {
        return _viewSettings;
    }
    
//    void setMainTB(gui::ToolBar* pTB)
//    {
//        _viewSettings.setMainTB(pTB);
//    }
//    
//    
//    bool isRestartRequired() const
//    {
//        return _viewSettings.isRestartRequired();
//    }
};
