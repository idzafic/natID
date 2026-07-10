// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Application.h
    @brief Main GUI application class managing the application lifecycle, windows, menus, and desktop mode. */
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
#include <cnt/PushBackVector.h>
#include <functional>

namespace gui
{
class AppHelper;
class ContextMenus;
class MenuItem;
class MenuBar;

/// @brief Core GUI application class that manages the main window, menu bar, context menus, and application lifecycle events.
class NATGUI_API Application : public mu::Application
{
    friend class AppHelper;

private:
    gui::Handle _handle = nullptr;              ///< Native handle to the underlying GUI toolkit.
    Window* _initialWindow = nullptr;           ///< Pointer to the application's main initial window.
    MenuBar* _menu = nullptr;                   ///< Pointer to the application-level menu bar.
    Window* _repWindow = nullptr;               ///< Pointer to the report window, if any.

    ContextMenus* _systemContextMenus = nullptr; ///< Pointer to system-provided context menus.
    ContextMenus* _contextMenus = nullptr;       ///< Pointer to application-defined context menus.

    gui::Window::FrameSize _initialSize = gui::Window::FrameSize::AdjustToContent; ///< Initial frame size mode for the main window.
    td::BYTE _menuBarLocation = 0;              ///< Location setting for the menu bar.
    td::BYTE _appDevResID = 0;                  ///< Application developer resource ID.

    td::BYTE _closing = 0;                      ///< Flag indicating that the application is in the closing process.
    static td::BYTE darkMode;                   ///< Static flag indicating whether dark mode is currently active.
    td::BYTE _userInitiatedClose = 1; //in case when user presses close button on the main window (without calling close())

private:
    /// @brief Internal callback structure for desktop mode change notifications.
    using CMCallback = struct CMCB
    {
        void* pObj;                              ///< Pointer to the observer object.
        std::function<void(td::BYTE)> callBack;  ///< Callback invoked when desktop color mode changes.
    };
    static cnt::PushBackVector<CMCallback> _onDesktopModeChange; ///< List of registered desktop mode change callbacks.
private:
    Application(const Application&) = delete;
    Application& operator =(const Application&) = delete;
    /// @brief Internal constructor used by derived classes.
    /// @param devResPos Developer resource position index.
    Application(int devResPos = 0);

    /// @brief Prepares the GUI subsystem with the given native handle.
    /// @param handle Native GUI handle.
    /// @return True if preparation succeeded.
    bool _prepareGUI(gui::Handle handle);
    /// @brief Sets the initial window for the application.
    /// @param pWnd Pointer to the window to set as initial.
    void setInitialWindow(Window* pWnd);
    /// @brief Sets the graphics color mode (dark or light).
    /// @param darkMode Non-zero for dark mode, zero for light mode.
    void setGraphicsMode(td::BYTE darkMode);
    /// @brief Returns the developer resource ID.
    /// @return Developer resource ID byte.
    td::BYTE getDevResID() const;
    /// @brief Inverts resource colors (e.g., for dark/light mode switching).
    void invertResColors();

protected:
    /// @brief Called to initialize graphics resources; override to specify model and shape counts.
    /// @param nModels Output number of 3D models.
    /// @param nSharedShapes Output number of shared shapes.
    /// @param nSharedTexts Output number of shared texts.
    virtual void initGraphics(td::UINT2& nModels, td::UINT2& nSharedShapes, td::UINT2& nSharedTexts);
    /// @brief Creates the application's initial window; must be overridden by derived classes.
    /// @return Pointer to the newly created initial Window.
    virtual Window* createInitialWindow(); //has to be overriden

    //call in App constructor if you want to start Maximimized, Minimized, FullScreen, etc.
    /// @brief Sets the initial frame size mode for the main window; call in the constructor.
    /// @param initialFrameSize The desired initial frame size mode.
    void setInitialFrameSize(gui::Window::FrameSize initialFrameSize);
    /// @brief Registers application-level context menus.
    /// @param pContextMenus Pointer to the ContextMenus to register.
    void setContextMenus(ContextMenus* pContextMenus);

    /// onResourcesLoaded will be called after all app resources are loaded
    ///override method should call base implementation
    /// @brief Called after all application resources are loaded; override to perform post-load initialization.
    /// @return True if resource loading succeeded.
    virtual bool onResourcesLoaded();
    /// @brief Called on the first activation of the application.
    /// @return True if initial activation handling succeeded.
    virtual bool onInitialActivation();
    /// @brief Called each time the application becomes active (gains focus).
    virtual void onActivated();
    /// @brief Called each time the application becomes inactive (loses focus).
    virtual void onDeactivated();
    /// @brief Called to determine whether the application should close.
    /// @return True if the application is allowed to close.
    virtual bool shouldClose();
    /// @brief Called when the application has been closed.
    virtual void onClosed();
    /// @brief Called when the system color mode switches between dark and light.
    /// @param bDarkMode True if switching to dark mode, false if switching to light mode.
    virtual void systemColorModeChanged(bool bDarkMode); //switch from dark -> light -> dark
    /// @brief Returns the resource string ID of the main toolbar.
    /// @return Pointer to the toolbar string ID.
    const char* getMainToolBarStrID() const override;
    /// @brief Stops the application event loop.
    void stop();

public:
    /// @brief Constructs the Application and initializes the platform-specific GUI backend.
    /// @param argc Argument count from main().
    /// @param argv Argument values from main().
    /// @param applicationGroup Optional application group identifier string.
    /// @param devResPos Developer resource position index.
    /// @param createLogFile If true, creates a log file on startup.
    Application(int argc, const char** argv, const char* applicationGroup = nullptr, int devResPos = 0, bool createLogFile = true);
    /// @brief Destructor; cleans up application resources.
    virtual ~Application();
    /// @brief Returns whether the application is in the process of closing.
    /// @return True if the application is closing.
    bool isClosing() const;

    /// @brief Initializes the environment with the specified general initialization flags.
    /// @param genInit Bitmask of general initialization flags (default: all).
    void initEnvironment(td::UINT4 genInit = (td::UINT4) GenInit::GI_FT_ALL);
    /// @brief Loads regional/translation data from files with the given extension.
    /// @param translationExtension File extension for translation resource files.
    void initRegionals(const td::String& translationExtension);

    /// @brief Convenience method combining initEnvironment and initRegionals.
    /// @param translationExtension File extension for translation resource files.
    /// @param genInit Bitmask of general initialization flags (default: all).
    void init(const td::String& translationExtension, td::UINT4 genInit =(td::UINT4) GenInit:: GI_FT_ALL);
    /// @brief Starts the application event loop.
    /// @return Exit code when the event loop terminates.
    int run();
    /// @brief Returns a pointer to the application's initial (main) window.
    /// @return Pointer to the initial Window.
    Window* getInitialWindow();
    /// @brief Returns a const pointer to the application's initial (main) window.
    /// @return Const pointer to the initial Window.
    const Window* getInitialWindow() const;
    /// @brief Returns a pointer to the report window.
    /// @return Pointer to the report Window.
    Window* getReportWindow();

    /// @brief Returns a const pointer to the application's menu bar.
    /// @return Const pointer to the MenuBar.
    const MenuBar* getMenuBar() const;

    ///if fileToCheck doesn't exist, the data will be installed (this is valid only in deployment)
    /// @brief Checks whether the specified user data file exists; if not, installs default user data.
    /// @param fileToCheck Relative path of the file to check for existence.
    /// @return True if data was already present or was successfully installed.
    bool checkAndInstallUserData(const char* fileToCheck) const;

    /// @brief Resolves the full path for a user data file, falling back to a developer path if necessary.
    /// @param fileName Base name of the file.
    /// @param devPath Developer fallback path.
    /// @param outPath Output resolved filesystem path.
    /// @return True if the path was resolved successfully.
    bool getUserDataFilePath(const char* fileName, const char* devPath, fo::fs::path& outPath) const;

    /// @brief Returns the native main window handle for platform interoperability.
    /// @return Void pointer to the native window handle.
    void* getMainWnd() override;

    /// @brief Returns whether the last close was initiated by the user pressing the main window's close button.
    /// @return True if the user initiated the close.
    bool isUserInitiatedClose(); //when user presses x the main window (without calling close())

    /// @brief Posts a user-defined event to the application event queue.
    /// @param eventID Identifier for the user event.
    /// @param userParam Optional variant parameter passed with the event.
    /// @return True if the event was handled successfully.
    static bool handleUserEvent(td::UINT4 eventID, const td::Variant& userParam = td::Variant(td::TD_NONE));

    /// @brief Returns whether the application is currently running in dark mode.
    /// @return True if dark mode is active.
    static bool isDarkMode();
    //register desktop mode change callback
    /// @brief Registers a callback to be invoked when the desktop color mode changes.
    /// @param pObject Pointer to the observer object (used as key for removal).
    /// @param callBack Function called with the new mode byte when the mode changes.
    static void addDesktopModeCallback(void* pObject, const std::function<void(td::BYTE)>& callBack);

    /// @brief Registers a typed callback to be invoked when the desktop color mode changes.
    /// @tparam TOBJ Type of the observer object.
    /// @param pObject Pointer to the observer object.
    /// @param callBack Function called with the new mode byte when the mode changes.
    template <class TOBJ>
    static void addDesktopModeCallback(TOBJ* pObject, const std::function<void(td::BYTE)>& callBack)
    {
        Application::addDesktopModeCallback((void*) pObject, callBack);
    }

    //unregister desktop mode change callback
    /// @brief Unregisters a previously added desktop mode change callback.
    /// @param pObject Pointer to the observer object whose callback should be removed.
    static void removeDesktopModeCallback(void* pObject);

    /// @brief Unregisters a previously added typed desktop mode change callback.
    /// @tparam TOBJ Type of the observer object.
    /// @param pObject Pointer to the observer object whose callback should be removed.
    template <class TOBJ>
    static void removeDesktopModeCallback(TOBJ* pObject)
    {
        Application::removeDesktopModeCallback((void*) pObject);
    }

    /// @brief Returns the foreground color ID appropriate for the given color scheme.
    /// @param colorScheme The color scheme to query.
    /// @return The foreground ColorID.
    static td::ColorID getForegroundColor(td::ColorScheme colorScheme);

    /// @brief Returns the background color ID appropriate for the given color scheme.
    /// @param colorScheme The color scheme to query.
    /// @return The background ColorID.
    static td::ColorID getBackgroundColor(td::ColorScheme colorScheme);

    /// @brief Shows the default About dialog for this application.
    /// @param dlgID Visual identifier for the About dialog resource.
    /// @param title Optional custom title for the dialog.
    void showDefaultAbout(td::UINT4 dlgID, const td::String& title = "");

    //ContextMenus
    /// @brief Retrieves a menu item from the context menu hierarchy by its IDs.
    /// @param menuID ID of the menu.
    /// @param firstSubMenuID ID of the first submenu level.
    /// @param lastSubMenuID ID of the last submenu level.
    /// @param actionID ID of the action item.
    /// @return Pointer to the found MenuItem, or nullptr if not found.
    MenuItem* getItem(td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE lastSubMenuID, td::BYTE actionID);
    /// @brief Opens a context menu at the specified point within a frame.
    /// @param menuID ID of the context menu to open.
    /// @param pFrame Pointer to the frame in which to open the menu.
    /// @param pointOnFrame Position within the frame at which to open the menu.
    /// @param pConsumer Optional message consumer to handle the resulting action.
    /// @param pContextData Optional context data to pass to the menu.
    /// @param contextMenuGroup Optional group ID for context menu filtering.
    void openContextMenu(td::BYTE menuID, gui::Frame* pFrame, const gui::Point& pointOnFrame, gui::IMessageConsumer* pConsumer = nullptr, const gui::ContextData* pContextData = nullptr, td::UINT2 contextMenuGroup = 0);

    //restart
    /// @brief Closes the running instance and starts a new instance of the application.
    void restart(); //closes runing and starts new instance of an applicagion

};

/// @brief Returns a pointer to the global application instance.
/// @return Pointer to the current Application.
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

/// @brief Lightweight GLX-based application class used when the full GUI stack is not needed.
class NATGLX_API Application : public mu::Application
{
    friend class glx::AppHelper;
protected:
    /// @brief Creates the application's initial window; must be overridden by derived classes.
    /// @return Pointer to the newly created initial Window.
    virtual Window* createInitialWindow(); //has to be overriden
public:
    /// @brief Constructs the GLX Application.
    /// @param argc Argument count from main().
    /// @param argv Argument values from main().
    /// @param applicationGroup Optional application group identifier string.
    /// @param devResPos Developer resource position index.
    Application(int argc, const char** argv, const char* applicationGroup = nullptr, int devResPos = 0);
    /// @brief Destructor; cleans up GLX application resources.
    virtual ~Application();
    /// @brief Initializes environment and regional settings.
    /// @param translationExtension File extension for translation resources.
    /// @param genInit General initialization flags bitmask.
    void init(const td::String& translationExtension, td::UINT4 genInit = GI_FT_ALL);
    /// @brief Starts the GLX application event loop.
    /// @return Exit code when the event loop terminates.
    int run();
};

} //namespace gui

#endif
