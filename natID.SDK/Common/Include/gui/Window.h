// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Window.h
    @brief Top-level application window with toolbar, central view, status bar, and lifecycle callbacks. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#ifndef MU_NATGLXS

#include <gui/Frame.h>
#include <cnt/List.h>
#include <gui/WndID.h>
#include <cnt/PushBackVector.h>


namespace gui
{

class Application;
class ToolBar;
class BaseView;
class WindowHelper;
class Dialog;
class StatusBar;
class ContextMenus;
class FileDialog;

/// @brief Top-level window that manages a toolbar, central view, status bar, and child dialogs.
class NATGUI_API Window : public Frame
{
    friend class Application;
    friend class WindowHelper;
    friend class Dialog;
protected:
public:
    /// @brief Specifies the initial size state of a window when it is first opened.
    enum class FrameSize : td::BYTE { Minimized = 0, ///< Open in a minimized (iconified) state.
        UseSpecified,     ///< Use the size provided at construction time.
        AdjustToContent,  ///< Automatically size to fit the central view's preferred size.
        Maximized,        ///< Open maximized to fill the screen.
        FullScreen        ///< Open in full-screen mode.
    };
private:
    /// @brief Attaches this window as a child of the given parent window.
    /// @param pParent Pointer to the parent window.
    void addToParent(Window* pParent);
protected:
#ifdef MU_DEBUG
    td::String _name; ///< Debug name assigned to this window for identification in logs.
#endif
    Window* _parent = nullptr;            ///< Pointer to the parent window, or nullptr for top-level windows.
    ToolBar* _pToolBar = nullptr;         ///< Optional toolbar displayed at the top of the window.
    BaseView* _pCentralView = nullptr;    ///< Primary content view occupying the main area of the window.
    StatusBar* _pStatusBar = nullptr;     ///< Optional status bar displayed at the bottom of the window.
    BaseView* _pAuxView = nullptr;        ///< Optional auxiliary view (e.g. side panel).
    void* _menuActions = nullptr;         ///< Platform-specific storage for menu action bindings.
    td::Size<int> _initialSize;           ///< Requested initial size used when FrameSize::UseSpecified is set.
    cnt::PushBackVector<NatObject*> _attachments; ///< Child windows and dialogs attached to this window.
    td::UINT4 _wndID = 0;                 ///< Numeric identifier used to locate this window in attachment lists.
    Frame::FixSizes _fixSizes = Frame::FixSizes::No; ///< Which dimensions (if any) are fixed during layout.
    td::BYTE _wndCloseFlag : 1;  ///< Set when the window has been requested to close.
    td::BYTE _isDialog : 1;      ///< Non-zero when this window is a dialog.
    td::BYTE _isModal : 1;       ///< Non-zero when this window is displayed modally.
    td::BYTE _keepOnTop : 1;     ///< Non-zero to keep this window above its parent.
    td::BYTE _resizable : 1;     ///< Non-zero when the user can resize this window.
    td::BYTE _isOpen : 1;        ///< Non-zero while this window is visible and not closed.
    td::BYTE _attachToParent : 1; ///< Non-zero to attach this window to its parent's lifetime.
    td::BYTE _frozen : 1;         ///< Non-zero while layout updates are suspended.
    td::BYTE _focusEvents : 1;    ///< Non-zero when focus gain/loss callbacks are enabled.
    td::BYTE _hasHeaderBar : 1;   ///< Non-zero when the window has a native header/title bar.
    //    td::BYTE _bulkActionItemInsert : 1;

private:
    Window() = delete;
    Window(const Window&) = delete;
    Window& operator = (const Window&) = delete;

    /// @brief Adjusts the initial geometry based on the measured content size.
    /// @param sz Measured content size.
    /// @param g Geometry structure to adjust.
    void adjustInitialGeometry(const gui::Size& sz, gui::Geometry& g);
    /// @brief Attaches a NatObject (e.g. child window or dialog) to this window.
    /// @param pObj Pointer to the object to attach.
    void attach(NatObject* pObj);
    /// @brief Removes a previously attached NatObject.
    /// @param pObj Pointer to the object to remove.
    /// @return True if the object was found and removed.
    bool removeAttachment(NatObject* pObj);
    /// @brief Internal release of resources; called by the framework during window destruction.
    void _release();
    /// @brief Checks whether all attached child windows/dialogs can be closed.
    /// @return True if all attachments can be closed.
    bool canCloseAttachments();
    /// @brief Handles the close sequence; closes all attachments and calls onClose().
    /// @return True if the window was successfully closed.
    bool handleClosing(); //true if closed
    /// @brief Platform-level destruction of the native window handle.
    void destroy() override;
    /// @brief Populates the window with its toolbar, central view, and status bar.
    void composeContent();
protected:
    /// @brief Assigns a debug name to this window (no-op in release builds).
    /// @param name Null-terminated debug name string.
    inline void setDbgName(const char* name)
    {
#ifdef MU_DEBUG
        _name = name;
#endif
    }
    /// @brief Attaches a toolbar to the top of this window.
    /// @param pToolBar Reference to the ToolBar to attach.
    void setToolBar(ToolBar& pToolBar);
    /// @brief Sets the central content view of the window.
    /// @param pView Pointer to the view to use as the central view.
    /// @param fixSizes Specifies which dimensions should be fixed (default FixAuto).
    void setCentralView(gui::BaseView* pView, Frame::FixSizes fixSizes = Frame::FixSizes::FixAuto);
    /// @brief Attaches a status bar to the bottom of this window.
    /// @param sb Reference to the StatusBar to attach.
    void setStatusBar(gui::StatusBar& sb);
    /// @brief Registers context menus to be available in this window.
    /// @param pContextMenus Pointer to the ContextMenus collection.
    void setContextMenus(ContextMenus* pContextMenus);
    /// @brief Called the first time the window becomes visible; override for one-time initialisation.
    virtual void onInitialAppearance(); //will be called only once

    //onFocus & onFocusLost needs to enabled with enableFocusEvents
    /// @brief Enables delivery of onFocus() and onFocusLost() callbacks.
    void enableFocusEvents();
    /// @brief Called when this window gains keyboard focus; requires enableFocusEvents().
    virtual void onFocus(); //will be called when window obtains focus
    /// @brief Called when this window loses keyboard focus; requires enableFocusEvents().
    virtual void onFocusLost(); //will be called when window is not in focus anymore

    /// @brief Override and return false to prevent the window from closing.
    /// @return True to allow the close, false to cancel it.
    virtual bool shouldClose(); //window will not be closed if this method returns false
    //void invokeOnCloseEnvent();
    /// @brief Called once when the window is about to be closed and destroyed.
    virtual void onClose(); //will be called only once
    /// @brief Called when the operating system's colour scheme (light/dark) changes.
    /// @param bDarkMode True if the system has switched to dark mode.
    virtual void systemColorModeChanged(bool bDarkMode);

    /// @brief Internal constructor used by Dialog subclasses.
    /// @param parent Pointer to the parent frame.
    /// @param size Initial size of the dialog.
    /// @param wndID Numeric identifier for the dialog.
    Window(Frame* parent, const gui::Size& size, td::UINT4 wndID); //for dialog
public:
    /// @brief Constructs a window with a given size and optional parent.
    /// @param sz Desired initial size.
    /// @param pParent Optional parent window (default nullptr).
    /// @param wndID Numeric identifier used to look up this window (default 0).
    Window(const gui::Size& sz, Window* pParent = nullptr, td::UINT4 wndID = 0);
    /// @brief Constructs a window whose size is derived from the central view.
    /// @param pParent Optional parent window (default nullptr).
    /// @param pSize Optional pointer to a desired size; pass nullptr to auto-size.
    /// @param wndID Numeric identifier (default 0).
    Window(Window* pParent, const gui::Size* pSize = nullptr, td::UINT4 wndID = 0);
    /// @brief Constructs a window placed at a specific position and size.
    /// @param g Desired initial geometry (position + size).
    /// @param pParent Optional parent window (default nullptr).
    /// @param wndID Numeric identifier (default 0).
    Window(const gui::Geometry& g, Window* pParent = nullptr, td::UINT4 wndID = 0);
    /// @brief Virtual destructor.
    virtual ~Window();
    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::Window.
    gui::ObjType getObjType() const override { return ObjType::Window; }

    /// @brief Moves and/or resizes the window to the specified geometry.
    /// @param g New geometry (position + size).
    /// @param redraw True to redraw after the change (default true).
    /// @param animation Animation style to use (default SystemDefault).
    void setGeometry(const Geometry& g, bool redraw=true, Frame::Animation animation = Frame::Animation::SystemDefault);
    /// @brief Resizes the window to the desired size.
    /// @param desiredSize Requested size.
    /// @param redraw True to redraw after resize (default false).
    /// @param animate True to animate the resize (default false).
    /// @return The actual size applied after constraints.
    Size setSize(const Size& desiredSize, bool redraw = false, bool animate = false);
    /// @brief Adjusts the window size so that it exactly fits the given content size.
    /// @param contentSize Desired content area size.
    /// @param animate True to animate the resize (default false).
    void adjustSizeToContentSize(const Size& contentSize, bool animate = false);

//    const td::PointAndSize<int>& getInitialRequestedGeometry() const;
    /// @brief Returns the initial size requested at construction time.
    /// @return Const reference to the initial size.
    const td::Size<int>& getInitialSize() const;

    /// @brief Returns the parent window.
    /// @return Pointer to the parent Window, or nullptr if this is a top-level window.
    Window* getParent() const;

    /// @brief Finds an attached child window by its numeric ID.
    /// @param wndID Numeric identifier to search for.
    /// @return Pointer to the matching Window, or nullptr if not found.
    gui::Window* getAttachedWindow(td::UINT4 wndID);

    /// @brief Finds an attached file dialog by its numeric ID.
    /// @param wndID Numeric identifier to search for.
    /// @return Pointer to the matching FileDialog, or nullptr if not found.
    gui::FileDialog* getAttachedFileDialog(td::UINT4 wndID);

    /// @brief Finds an attached child window by a typed enum ID.
    /// @tparam TID Enum or integer type of the window identifier.
    /// @param wndID Identifier to search for.
    /// @return Pointer to the matching Window, or nullptr if not found.
    template <typename TID>
    gui::Window* getAttachedWindow(TID wndID)
    {
        return getAttachedWindow((td::UINT4) wndID);
    }

    /// @brief Finds an attached dialog by a typed enum ID and casts it to gui::Dialog.
    /// @tparam TID Enum or integer type of the dialog identifier.
    /// @param wndID Identifier to search for.
    /// @return Pointer to the matching Dialog, or nullptr if not found.
    template <typename TID>
    gui::Dialog* getAttachedDialog(TID wndID)
    {
        gui::Window* pWnd = getAttachedWindow((td::UINT4) wndID);
        if (!pWnd)
            return nullptr;
        gui::Dialog* pDlg = reinterpret_cast<gui::Dialog*>(pWnd);
        return pDlg;
    }

    /// @brief Finds an attached file dialog by a typed enum ID.
    /// @tparam TID Enum or integer type of the dialog identifier.
    /// @param wndID Identifier to search for.
    /// @return Pointer to the matching FileDialog, or nullptr if not found.
    template <typename TID>
    gui::FileDialog* getAttachedFileDialog(TID wndID)
    {
        return getAttachedFileDialog((td::UINT4) wndID);
    }

    /// @brief Returns whether the window is currently open (visible).
    /// @return True if the window is open.
    bool isOpen() const;

    /// @brief Opens the window, making it visible for the first time.
    /// @param initialSize Initial size state (default AdjustToContent).
    void open(Window::FrameSize initialSize = Window::FrameSize::AdjustToContent); //to 'open' window (show it initially)
    /// @brief Opens the window modally, disabling interaction with all other windows.
    /// @param initialSize Initial size state (default AdjustToContent).
    void openModal(Window::FrameSize initialSize = Window::FrameSize::AdjustToContent); //to 'open' while disabling others

    /// @brief Closes and destroys this window.
    void close(); //close and destroy it

    /// @brief Returns the platform visual/display identifier for this window.
    /// @return Visual identifier as a UINT4.
    td::UINT4 getVisualID() const override;

    /// @brief Returns whether this window is a dialog.
    /// @return True if this is a dialog window.
    bool isDialog() const;
    /// @brief Returns fixed-size information for width and height.
    /// @return Tuple of (fixedWidth, fixedHeight, fixedBoth) booleans.
    std::tuple<bool, bool, bool> isFixedSize() const;
    /// @brief Returns the FixSizes policy for this window.
    /// @return Current Frame::FixSizes value.
    Frame::FixSizes getFixSizes() const;
    /// @brief Returns a const pointer to the attached toolbar.
    /// @return Const pointer to the ToolBar, or nullptr if none.
    const ToolBar* getToolBar() const;
    /// @brief Returns a pointer to the attached toolbar.
    /// @return Pointer to the ToolBar, or nullptr if none.
    ToolBar* getToolBar();
    /// @brief Returns whether the window has a native header/title bar.
    /// @return True if a header bar is present.
    bool hasHeaderBar() const;
    /// @brief Returns a pointer to the central content view.
    /// @return Pointer to the central BaseView, or nullptr if none is set.
    BaseView* getCentralView();
    /// @brief Returns a const pointer to the central content view.
    /// @return Const pointer to the central BaseView, or nullptr if none is set.
    const BaseView* getCentralView() const;
    /// @brief Returns a const pointer to the status bar.
    /// @return Const pointer to the StatusBar, or nullptr if none.
    const StatusBar* getStatusBar() const;
    /// @brief Returns whether this window is currently displayed modally.
    /// @return True if modal.
    bool isModal() const;
    /// @brief Returns a pointer to the parent window (non-const alias).
    /// @return Pointer to the parent Window, or nullptr.
    Window* getParentWindow();
    /// @brief Returns a const pointer to the parent window.
    /// @return Const pointer to the parent Window, or nullptr.
    const Window* getParentWindow() const;
    /// @brief Configures this window to float on top of its parent.
    void keepOnTopOfParent();
    /// @brief Returns whether this window is set to float on top of its parent.
    /// @return True if on-top behaviour is active.
    bool isOnTopOfParent() const;
    /// @brief Sets whether the user can resize this window.
    /// @param bResizable True to allow resizing, false to lock the window size.
    void setResizable(bool bResizable);
    /// @brief Returns whether the window can be resized by the user.
    /// @return True if resizable.
    bool isResizable() const;

    //event handlers
    /// @brief Dispatches a menu or toolbar action to the appropriate handler.
    /// @param aiDesc Descriptor containing the menu, submenu, and action IDs.
    /// @return True if the action was handled.
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;
    /// @brief Binds a lambda to a top-level menu action (first and last submenu = 0).
    /// @param menuID Menu identifier.
    /// @param actionID Action identifier within the menu.
    /// @param fnToCall Function to invoke when the action is triggered.
    void onActionItem(td::BYTE menuID, td::BYTE actionID, const std::function<void()>& fnToCall); //first and last submenu = 0
    /// @brief Binds a lambda to an action within a specific submenu range.
    /// @param menuID Top-level menu identifier.
    /// @param firstSubMenuID First submenu identifier in the range.
    /// @param actionID Action identifier.
    /// @param lastMenuID Last submenu identifier in the range.
    /// @param fnToCall Function to invoke when the action is triggered.
    void onActionItem(td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE actionID, td::BYTE lastMenuID, const std::function<void()>& fnToCall);

    /// @brief Freezes the window, deferring layout and redraw operations.
    void freeze();
    /// @brief Unfreezes the window, applying any deferred layout and redraw operations.
    void unFreeze();
    /// @brief Returns whether the window is currently frozen.
    /// @return True if frozen.
    bool isFrozen() const;

    /// @brief Returns whether a close request is in progress.
    /// @return True if the window is in the process of closing.
    bool isClosing() const;
    /// @brief Returns whether this is the application's main window.
    /// @return True if this window is the main window.
    bool isMain() const; //returns true if this window is the main window
    /// @brief Returns whether the window is currently maximized.
    /// @return True if maximized.
    bool isMaximized() const;

    /// @brief Returns the scale factor between logical (point) and physical (pixel) coordinates.
    /// @return Scale factor (e.g. 2.0 on a HiDPI display).
    float getLogicalToPhysicalPixelScale() const;
};

} //namespace gui

#else
//MU_NATGLXS defined
#include <glx/natGLX.h>
#include <gui/Types.h>
#include <gui/NatObject.h>

namespace glx
{
class View;
}//namespace glx

namespace gui
{
/// @brief Lightweight GLX window used when MU_NATGLXS is defined; wraps a native window handle.
class NATGLX_API Window
{
public:
    /// @brief Bitmask flags controlling native window style attributes (macOS NSWindowStyleMask values).
    enum class StyleMask : td::WORD {
        Borderless       = 0,               ///< Window has no border or title bar.
        Titled           = ( 1 << 0 ),      ///< Window has a title bar.
        Closable         = ( 1 << 1 ),      ///< Window has a close button.
        Miniaturizable   = ( 1 << 2 ),      ///< Window has a minimise button.
        Resizable        = ( 1 << 3 ),      ///< Window can be resized by the user.
        TexturedBackground = ( 1 << 8 ),    ///< Window uses a textured (metal) background.
        UnifiedTitleAndToolbar = ( 1 << 12 ), ///< Title bar and toolbar are visually unified.
        FullScreen       = ( 1 << 14 ),     ///< Window can enter full-screen mode.
        FullSizeContentView = ( 1 << 15 ),  ///< Content view extends under the title bar.
        UtilityWindow    = ( 1 << 4 ),      ///< Floating utility/panel window.
        DocModalWindow   = ( 1 << 6 ),      ///< Document-modal sheet window.
        NonactivatingPanel   = ( 1 << 7 )}; ///< Panel that does not activate the application.
protected:
    gui::Handle _handle = nullptr; ///< Native platform window handle.
private:
    Window() = delete;
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    void operator = (const Window&) = delete;
public:
    /// @brief Constructs a GLX window at a given size with the specified style masks.
    /// @param sz Desired window size.
    /// @param masks Initialiser list of StyleMask flags (default Closable | Titled).
    Window(const gui::Size& sz, const std::initializer_list<StyleMask>& masks={StyleMask::Closable, StyleMask::Titled});
    /// @brief Constructs a GLX window at a given geometry with the specified style masks.
    /// @param g Desired position and size.
    /// @param masks Initialiser list of StyleMask flags (default Closable | Titled).
    Window(const gui::Geometry& g, const std::initializer_list<StyleMask>& masks={StyleMask::Closable, StyleMask::Titled});
    /// @brief Sets the OpenGL view as the central content view.
    /// @param pView Pointer to the glx::View to display.
    void setCentralView(glx::View* pView);
    /// @brief Returns the native platform handle of this window.
    /// @return Native handle value.
    gui::Handle getHandle();
    /// @brief Sets the window title to the given C-string.
    /// @param title Null-terminated title string.
    void setTitle(const char* title);
    /// @brief Sets the window title to the given td::String.
    /// @param title Title string.
    void setTitle(const td::String& title);

};
} //namespace gui

#endif


