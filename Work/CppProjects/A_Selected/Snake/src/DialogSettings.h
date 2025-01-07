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
//            auto pApp = getApplication();
            auto appProperties = getAppProperties();
            
            td::String strTr = _viewSettings.getTranslationExt();
            if (strTr.length() > 0)
            {
                //write translation info back to properties
                appProperties->setValue("translation", strTr);
            }
//            bool showLabels = _viewSettings.isTBWithLabels();            
//            appProperties->setTBLabelVisibility(mu::IAppProperties::ToolBarType::Main, showLabels);
            int difficulty = _viewSettings.getDifficulty();
            appProperties->setValue("difficulty", difficulty);
        }
        return true;
    }
public:
    DialogSettings(gui::Frame* pFrame, td::UINT4 wndID = 0)
    : gui::Dialog(pFrame, { {gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default},
                            {gui::Dialog::Button::ID::Cancel, tr("Cancel")}}, gui::Size(450, 100), wndID)
    {
        setTitle(tr("dlgSettings"));
        setCentralView(&_viewSettings);
    }
    
    void setMainTB(gui::ToolBar* pTB)
    {
        _viewSettings.setMainTB(pTB);
    }

    ~DialogSettings(){
        if (_viewSettings.isRestartRequired())
            {
                gui::Alert::showYesNoQuestion(tr("RestartRequired"), tr("RestartRequiredInfo"), tr("Restart"), tr("DoNoRestart"), [this] (gui::Alert::Answer answer) {
                if (answer == gui::Alert::Answer::Yes)
                {
                    auto pApp = getApplication();
                    //clean up and save here whatever you need, the application is about to terminate... and restart fresh....
                    pApp->restart();
                }
                });
            }
    }

    
    
//    bool shouldRestartFromInitialPosition() const
//    {
//        return _viewSettings.shouldRestartFromInitialPosition();
//    }

};
