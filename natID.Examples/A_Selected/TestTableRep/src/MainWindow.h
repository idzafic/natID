//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include <gui/ActionItem.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "ViewTableEdit.h"
#include <rpt/IViewer.h>
#include <rpt/ReportData.h>
#include <rpt/IResources.h>
#include "DialogSettings.h"
#include "ContextMenus.h"

class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    ViewTableEdit _mainView;
    ContextMenus _contextMenus;
public:
    MainWindow()
    : gui::Window(gui::Size(1000, 600))
    {
        setTitle("TableRep Test");
        _mainMenuBar.setAsMain(this);
        setToolBar(_toolBar);      
        setContextMenus(&_contextMenus);
        setCentralView(&_mainView);
        
        //load report resources
        rpt::IResources* pRes = rpt::IResources::getManager();
        
        //RepConfig.xml must be located in res subfolder
        if (!pRes->loadFromXML("MutableResources.xml"))
        {
            assert(false);
            return;
        }
    }
    
    ~MainWindow()
    {
    }
    
    bool shouldClose() override
    {
        return true;
    }
    
    void onClose() override
    {        
    }

    bool handleReportContextMenu(gui::ActionItemDescriptor& aiDesc)
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        const gui::ContextData* pData = aiDesc.getContextData();
        td::String strTitle("Context Memu Option Selected");
        td::String strContent(pData->configName);
        strContent += " : ";
        strContent += pData->layoutName;
        strContent += " : ";
        strContent += pData->elementName;
        td::String rowNo;
        rowNo.format("%u", pData->recNo);
        strContent += " : recNo=";
        strContent += rowNo;
        showAlert(strTitle, strContent);

        return true;
    }
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {

        auto source = aiDesc.getSource();
        if (source == gui::ActionItemDescriptor::Source::Report)
        {
            //here comes handling of Report context menus
            return handleReportContextMenu(aiDesc);
        }

        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        
        if (menuID == 10 && actionID == 1)
        {
            td::UINT4 settingsID = 728289; //bilo koji unikatan broj
            auto pDlg = getAttachedWindow(settingsID);
            if (pDlg)
                pDlg->setFocus();
            else
            {
                DialogSettings* pSettingsDlg = new DialogSettings(this, settingsID);
                pSettingsDlg->keepOnTopOfParent();
                pSettingsDlg->setMainTB(&_toolBar);
                pSettingsDlg->open();
            }
            return true;
        }
            
        if (menuID == 20 && firstSubMenuID== 0 && lastSubMenuID == 0)
        {
            return false;
        }
        if (menuID == 10 && firstSubMenuID== 0 && lastSubMenuID == 0)
        {
            switch (actionID)
            {
                case 10:
                    _mainView.showNormalReport(false);
                    return true;
                case 11:
                    _mainView.showNormalReport(true);
                    return true;
                case 20:
                    _mainView.showWaterfallReport(false);
                    return true;
                case 21:
                    _mainView.showWaterfallReport(true);
                    return true;
                case 22:
                    _mainView.showOtherRep(this);
                    return true;
                default:
                    return false;
            }
        }
        if (menuID == 255)
        {
            if (actionID == 10)
            {
                //increase window size
                gui::Geometry g;
                getGeometry(g);
                g.size.width *= 1.2f;
                g.size.height *= 1.2f;
                setGeometry(g, true, gui::Frame::Animation::Yes);
                return true;
            }
            else if (actionID == 20)
            {
                //decrease window size
                gui::Geometry g;
                getGeometry(g);
                g.size.width *= 0.8f;
                g.size.height *= 0.8f;
                setGeometry(g, true, gui::Frame::Animation::Yes);
                return true;
            }
        }
        
        td::String msgText("Handling onActionItem MainWindow");
        td::String informativeText;
        informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);
        
       showAlert(msgText, informativeText);
        return true;
    }
    
};
