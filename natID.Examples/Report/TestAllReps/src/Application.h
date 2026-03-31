//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Application.h>
#include "MainWindow.h"
#include "ContextMenus.h"
#include <rpt/IViewer.h>

class Application : public gui::Application
{
    ContextMenus _contextMenus;
protected:
    
    gui::Window* createInitialWindow() override
    {        
        //play sounds on selection change in a Report (in case if there is context menu in the report)
        rpt::IViewer::setPlaySoundOnSelectionChange(true);
        
//        setInitialFrameSize(gui::Window::FrameSize::Maximized);
        return new MainWindow();
    }

    bool onResourcesLoaded() override
    {
        td::String dbName = gui::getResFileName(":allReps.db");
        dp::IDatabase* pDB = dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3);
        if (!pDB->connect(dbName))
        {
            assert(false);
            return false;
        }

        dp::setMainDatabase(pDB);
        return true;
    }
    
public:
    Application(int argc, const char** argv)
    : gui::Application(argc, argv)
    {
        setContextMenus(&_contextMenus);
    }
};
