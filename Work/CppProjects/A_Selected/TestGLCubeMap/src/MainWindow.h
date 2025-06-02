//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "ViewCubeMap.h"
#include "DialogSettings.h"
#include "ViewSettings.h"
#include <gui/Panel.h>
#include <gui/Application.h>

class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    ViewCubeMap _viewCubeMap;
    td::UINT4 _cSettingsDlgID = 10;
public:
    MainWindow()
    : gui::Window(gui::Geometry(50, 50, 1200, 600))
    {
        _mainMenuBar.setAsMain(this);
        _toolBar.forwardMessagesTo(this);
        
        _toolBar.setFontScaleUpdater(std::bind(&ViewCubeMap::setFont3ScaleFactor, &_viewCubeMap, std::placeholders::_1));
        setToolBar(_toolBar);
        setCentralView(&_viewCubeMap);
    }
    
protected:
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSM, lastSM1, actionID] = aiDesc.getIDs();

        if(menuID == 10){
            if(actionID == 10)
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
        }

        if (menuID == 20)
        {
            if (actionID == 10)
            {
                //export to image
                _viewCubeMap.exportFrame();
                return true;
            }
        }
        return false;
    }
    
    bool onChangedValue(gui::Slider* pSlider) override
    {
        if (pSlider == _toolBar.getSpeedSlider())
        {
            double val = pSlider->getValue();
            _viewCubeMap.updateSpeed((float)val);
            return true;
        }
        return false;
    }
    
    bool onClick(gui::CheckBox* pBtn) override
    {
        if (pBtn == _toolBar.getTextureSwitch())
        {
            _viewCubeMap.switchTexture();
            return true;
        }
        return false;
    }
    
};
