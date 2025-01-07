//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "MainView.h"
#include "StatusBar.h"
#include <functional>
#include "DialogSettings.h"

class MainWindow : public gui::Window
{
protected:
    gui::Image _imgStart;
    gui::Image _imgStop;
    gui::Image _imgOK;
    gui::Image _imgNOK;
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    StatusBar _statusBar;
    
    std::function<void()> _fnUpdateMenuAndTB;
    
    std::function<void(td::INT1, int, int)> _fnUpdateStatusBar;
    
    MainView _mainView;
    const td::UINT4 _cSettingsDlgID = 17; //any unique id among dialogs
protected:
    
    //will be called only once
    void onInitialAppearance() override
    {
        auto appProperties = getAppProperties();
        Difficulty difficulty = (Difficulty) appProperties->getValue("difficulty", (int) Difficulty::Medium);
        
        if (_mainView.setGameDifficulty(difficulty))
            updateDifficultyIcons(difficulty);
        
        _mainView.focusOnCanvas();
    }
    
    bool onKeyPressed(const gui::Key& key) override
    {
        if (key.isASCII())
        {
            char ch = key.getChar();
            if (ch == ' ')
            {
                _mainView.startStop();
                if (_mainView.isRunning())
                {
                    _statusBar.resetPoints();
                }
            }
        }
        return false;
    }
    

    void updateMenuAndTB()
    {
        bool isRunning = _mainView.isRunning();
        gui::MenuItem* pMenuItem = _mainMenuBar.getItem(20, 0, 0, 10);
        pMenuItem->setChecked(isRunning);
        
        gui::ToolBarItem* pTBItem = _toolBar.getItem(20, 0, 0, 10);
        if (pTBItem)
        {
            if (isRunning)
            {
                pTBItem->setImage(&_imgStop);
                pTBItem->setLabel(tr("stop"));
                pTBItem->setTooltip(tr("stopTT"));
            }
            else
            {
                pTBItem->setImage(&_imgStart);
                pTBItem->setLabel(tr("start"));
                pTBItem->setTooltip(tr("startTT"));
            }
        }
    }
    
    bool shouldClose() override
    {
        //if (_mainView.isRunning())
        //{
        //    showAlert(tr("closeNOK"), tr("cloeErr"));
        //    return false;
        //}
        return true;
    }

    void updateDifficultyIcons(Difficulty d)
    {
        {
            gui::MenuItem* pMenuItem = _mainMenuBar.getItem(20, 0, 0, 20);
            pMenuItem->setChecked(d == Difficulty::Easy);
            gui::ToolBarItem* pTBItem = _toolBar.getItem(20, 0, 0, 20);
            if (pTBItem)
                pTBItem->setImage((d == Difficulty::Easy) ? &_imgOK : &_imgNOK);
        }
        {
            gui::MenuItem* pMenuItem = _mainMenuBar.getItem(20, 0, 0, 21);
            pMenuItem->setChecked(d == Difficulty::Medium);
            gui::ToolBarItem* pTBItem = _toolBar.getItem(20, 0, 0, 21);
            if (pTBItem)
                pTBItem->setImage((d == Difficulty::Medium) ? &_imgOK : &_imgNOK);
        }
        {
            gui::MenuItem* pMenuItem = _mainMenuBar.getItem(20, 0, 0, 22);
            pMenuItem->setChecked(d == Difficulty::Hard);
            gui::ToolBarItem* pTBItem = _toolBar.getItem(20, 0, 0, 22);
            if (pTBItem)
                pTBItem->setImage((d == Difficulty::Hard) ? &_imgOK : &_imgNOK);
        }
    }


    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        auto pAI = aiDesc.getActionItem();
        
        if (menuID == 10 & actionID == 10)
        {
            auto pDlg = getAttachedWindow(_cSettingsDlgID);
            if (pDlg)
                pDlg->setFocus();
            else
            {
                DialogSettings* pSettingsDlg = new DialogSettings(this, _cSettingsDlgID);
                pSettingsDlg->keepOnTopOfParent();
                pSettingsDlg->setMainTB(&_toolBar);
                pSettingsDlg->open();
            }
            return true;
        }
        
        
        if (menuID == 20 && firstSubMenuID== 0 && lastSubMenuID == 0)
        {
            switch (actionID)
            {
                case 10:
                {
                    _mainView.startStop();
                    if (_mainView.isRunning())
                    {
                        _statusBar.resetPoints();
                    }
                    return true;
                }

                case 20:
                {
                    if (_mainView.setGameDifficulty(Difficulty::Easy))
                        updateDifficultyIcons(Difficulty::Easy);

                    _mainView.focusOnCanvas();
                    return true;
                }
                case 21:
                {
                    if (_mainView.setGameDifficulty(Difficulty::Medium))
                        updateDifficultyIcons(Difficulty::Medium);

                    _mainView.focusOnCanvas();
                    return true;
                }
                case 22:
                {
                    if (_mainView.setGameDifficulty(Difficulty::Hard))
                        updateDifficultyIcons(Difficulty::Hard);

                    _mainView.focusOnCanvas();
                    return true;
                }
            }
        }
        return false;
    }
    
    void updateStatusBar(td::INT1 fruit, int fruitPts, int totalScore)
    {
        _statusBar.updatePoints(fruit, fruitPts, totalScore);
    }
    
public:
    MainWindow()
    : gui::Window(gui::Size(1000, 600))
    , _imgStart(":start")
    , _imgStop(":stop")
    , _imgOK(":ok")
    , _imgNOK(":nok")
    , _toolBar()
    , _fnUpdateMenuAndTB(std::bind(&MainWindow::updateMenuAndTB, this))
    , _fnUpdateStatusBar(std::bind(&MainWindow::updateStatusBar, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3))
    , _mainView(&_fnUpdateMenuAndTB, &_fnUpdateStatusBar)
    {
        setResizable(false);
        
//#define TEST_IMG_SAVE
//        
//#ifdef TEST_IMG_SAVE
//#ifdef MU_WINDOWS
//        td::String strStart("R/Start");
//        td::String strStop("R/Stop");
//#else
//        td::String strStart("/Volumes/RAMDisk/Start");
//        td::String strStop("/Volumes/RAMDisk/Stop");
//#endif
//        _imgStart.saveToFile(strStart); //PNG
//        _imgStop.saveToFile(strStop); //PNG
//#endif
        setTitle(tr("appTitle"));
        _mainMenuBar.setAsMain(this);
        setToolBar(_toolBar);
        setStatusBar(_statusBar);
        setCentralView(&_mainView);
    }
    
    ~MainWindow()
    {
    }
    
};
