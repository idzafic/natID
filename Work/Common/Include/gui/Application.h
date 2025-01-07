// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#ifndef MU_NATGLXS
#include "natGUI.h"
#include <gui/Types.h>
#include <cnt/List.h>
#include <mu/Application.h>
#include <gui/InitialParams.h>

namespace gui
{
class AppHelper;
class ContextMenus;
class MenuItem;
class MenuBar;

class NATGUI_API Application : public mu::Application
{
    friend class AppHelper;
    
private:
    gui::Handle _handle = nullptr;
    Window* _initialWindow = nullptr;
    MenuBar* _menu = nullptr;
    Window* _repWindow = nullptr;
    ContextMenus* _contextMenus = nullptr;

    gui::Window::FrameSize _initialSize = gui::Window::FrameSize::AdjustToContent;
    td::BYTE _menuBarLocation = 0;
    td::BYTE _appDevResID = 0;
    td::BYTE _darkMode = 0;
    td::BYTE _closing = 0;
    td::BYTE _userInitiatedClose = 1; //in case when user presses close button on the main window (without calling close())
    
private:
    Application(int devResPos = 0);
    bool _prepareGUI(gui::Handle handle);
    void setInitialWindow(Window* pWnd);
    void setGraphicsMode(td::BYTE darkMode);
    td::BYTE getDevResID() const;
    void invertResColors();
protected:
    virtual void initGraphics(td::UINT2& nModels, td::UINT2& nSharedShapes, td::UINT2& nSharedTexts);
    virtual Window* createInitialWindow(); //has to be overriden
    
    //call in App constructor if you want to start Maximimized, Minimized, FullScreen, etc.
    void setInitialFrameSize(gui::Window::FrameSize initialFrameSize);
    void setContextMenus(ContextMenus* pContextMenus);
    
    virtual bool onInitialActivation();
    virtual void onActivated();
    virtual void onDeactivated();
    virtual bool shouldClose();
    virtual void onClosed();
    virtual void systemAppearanceChanged(bool bDarkMode); //switch from dark -> light -> dark
    const char* getMainToolBarStrID() const override;
    void stop();
    
public:
    Application(int argc, const char** argv, const char* applicationGroup = nullptr, int devResPos = 0, bool createLogFile = true);
    virtual ~Application();
    bool isClosing() const;
    bool isDarkMode() const;
    
    void initEnvironment(td::UINT4 genInit = (td::UINT4) GenInit::GI_FT_ALL);
    void initRegionals(const td::String& translationExtension);
    
    void init(const td::String& translationExtension, td::UINT4 genInit =(td::UINT4) GenInit:: GI_FT_ALL);
    int run();
    Window* getInitialWindow();
    const Window* getInitialWindow() const;
    Window* getReportWindow();

    const MenuBar* getMenuBar() const;

    const fo::fs::path& getResPath() const;
    
    ///if fileToCheck doesn't exist, the data will be installed (this is valid only in deployment)
    bool checkAndInstallUserData(const char* fileToCheck) const;
    
    bool getUserDataFilePath(const char* fileName, const char* devPath, fo::fs::path& outPath) const;
    
    void* getMainWnd() override;
    
    bool isUserInitiatedClose(); //when user presses x the main window (without calling close())
    
    //ContextMenus
    MenuItem* getItem(td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);
    void openContextMenu(td::BYTE menuID, gui::Frame* pFrame, const gui::Point& pointOnFrame, gui::IMessageConsumer* pConsumer = nullptr, const gui::ContextData* pContextData = nullptr, td::UINT2 contextMenuGroup = 0);
    
    void restart(); //closes runing and starts new instance of an applicagion
};

NATGUI_API Application* getApplication();

} //namespace gui
#else
//MU_NATGLXS defined
#include <glx/natGLX.h>
#include <gui/Types.h>
#include <gui/NatObject.h>
#include <mu/Application.h>

namespace glx
{
class AppHelper;
}

namespace gui
{
class Window;

class NATGLX_API Application : public mu::Application
{
    friend class glx::AppHelper;
protected:
    virtual Window* createInitialWindow(); //has to be overriden
public:
    Application(int argc, const char** argv, const char* applicationGroup = nullptr, int devResPos = 0);
    virtual ~Application();
    void init(const td::String& translationExtension, td::UINT4 genInit = GI_FT_ALL);
    int run();
};

} //namespace gui

#endif
