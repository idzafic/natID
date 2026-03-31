//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
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
                appProperties->setValue("translation", strTr); //write translation info back to properties
            
            bool darkQueen = _viewSettings.useDarkQueen();
            appProperties->setValue("darkQueen", darkQueen);
            
            bool playSound = _viewSettings.playSound();
            appProperties->setValue("playSound", playSound);
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
    
    bool useDarkQueen() const
    {
        return _viewSettings.useDarkQueen();
    }
    
    bool playSound() const
    {
        return _viewSettings.playSound();
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
    

};
