//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once


#include <gui/Window.h>
#include <gui/ActionItem.h>
#include "MenuBar.h"
#include "Reports.h"
#include <gui/Image.h>
#include <rpt/IResources.h>

class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
//    gui::Image _repImg;
    gui::Image _imgInvoice;
    gui::Image _imgCardRep;
//    ContextManager _contextManger;
    //ToolBar _toolBar;
    //ViewTableEdit _mainView;
public:
    MainWindow()
    : gui::Window(gui::Geometry(50, 50, 400, 300))
    , _imgInvoice(":invoiceRep")
    , _imgCardRep(":cardRep")
    {
        setTitle("TableEdit Test");
        _mainMenuBar.setAsMain(this);
        
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
    
protected:
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
        
        switch (actionID)
        {
        case 10: racunV1(&_imgInvoice); return true;
        case 20: racunV2(&_imgInvoice); return true;
        case 30: racunV3(&_imgInvoice); return true;
        case 40: racunV4(&_imgInvoice); return true;
        case 50: racunV5(&_imgInvoice, false); return true;
        case 60: racunV5(&_imgInvoice, true); return true;
        case 70: racunParallelTables(&_imgInvoice); return true ;
        case 110: karticaReport1(&_imgCardRep); return true ;
        case 120: karticaReport4(&_imgCardRep); return true;
        case 123: landScapeWithFormulaTest(&_imgCardRep); return true;
        case 124: port2LandscapeRotation(&_imgCardRep); return true;
        case 125: multiLayout(&_imgCardRep, 1); return true;
        case 126: multiLayout(&_imgCardRep, 2); return true;
        case 127: multiLayout(&_imgCardRep, 3); return true;
        case 128: multiLayout(&_imgCardRep, 4); return true;

        case 130: karticaMultiReport(&_imgCardRep); return true;
        case 131: karticaMultiReport2(&_imgCardRep); return true; //Demonstrates excludes zeros and qtu::Dialog overloading
        case 135: karticaMultiReport_WF(&_imgCardRep); return true;
        case 140: itemsForStoreWindow6x3(&_imgCardRep); return true;
        case 150: itemsForStoreWindow7x3(&_imgCardRep); return true;
        case 170: showWaterfallSimple(&_imgCardRep); return true;
        case 180: showWaterfallAdvanced(&_imgCardRep); return true;
        case 190: showWaterfallAdvanced2(&_imgCardRep); return true;
        case 200: dynamicBodyWithParallelTables(&_imgCardRep, this); return true;

        break;
        }

        td::String msgText("Handling onActionItem MainWindow");
        td::String informativeText;
        informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);
        
       showAlert(msgText, informativeText);
        return true;
    }
    
};
