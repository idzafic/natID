//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "TabView.h"

class WindowPlugin : public gui::Window
{
protected:
    TabView _tabView;
    sc::IPlugin::Cleaner _cleanPlugin;
    
    void onClose() override final   //When user closes button by clicking x
    {
        _cleanPlugin();
        onClosedPluginWindow();
    }
    
public:
    WindowPlugin(gui::Window* parentWnd, sc::IPlugin* pIPlugin, const sc::IPlugin::CallBack& onComplete, const sc::IPlugin::Cleaner& cleaner, td::UINT4 wndID = 0)
    : gui::Window(gui::Size(800, 600), parentWnd, wndID)
    , _tabView(pIPlugin, onComplete)
    , _cleanPlugin(cleaner)
    {
        setTitle(tr("Earthquake Plugin"));
        setCentralView(&_tabView);
    }
    
    td::String getOutFileName() const
    {
        return _tabView.getOutFileName();
    }
};
