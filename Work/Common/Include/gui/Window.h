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

class NATGUI_API Window : public Frame
{
    friend class Application;
    friend class WindowHelper;
    friend class Dialog;
protected:
public:
    enum class FrameSize : td::BYTE { Minimized = 0, UseSpecified, AdjustToContent, Maximized, FullScreen };
private:
    void addToParent(Window* pParent);
protected:
#ifdef MU_DEBUG
    td::String _name;
#endif
    Window* _parent = nullptr;
    ToolBar* _pToolBar = nullptr;
    BaseView* _pCentralView = nullptr;
    StatusBar* _pStatusBar = nullptr;
    BaseView* _pAuxView = nullptr;
    void* _menuActions = nullptr;
    td::Size<int> _initialSize;
    cnt::PushBackVector<NatObject*> _attachments;
    td::UINT4 _wndID = 0;
    Frame::FixSizes _fixSizes = Frame::FixSizes::No;
    td::BYTE _wndCloseFlag : 1;
    td::BYTE _isDialog : 1;
    td::BYTE _isModal : 1;
    td::BYTE _keepOnTop : 1;
    td::BYTE _resizable : 1;
    td::BYTE _isOpen : 1;
    td::BYTE _attachToParent : 1;
    td::BYTE _frozen : 1;
    td::BYTE _focusEvents : 1;
    td::BYTE _hasHeaderBar : 1;
    //    td::BYTE _bulkActionItemInsert : 1;

private:
    Window() = delete;
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    void operator = (const Window&) = delete;
    
    void adjustInitialGeometry(const gui::Size& sz, gui::Geometry& g);
    void attach(NatObject* pObj);
    bool removeAttachment(NatObject* pObj);
    void _release();
    bool canCloseAttachments();
    bool handleClosing(); //true if closed
    void destroy() override;
    void composeContent();
protected:
    inline void setDbgName(const char* name)
    {
#ifdef MU_DEBUG
        _name = name;
#endif
    }
    void setToolBar(ToolBar& pToolBar);
    void setCentralView(gui::BaseView* pView, Frame::FixSizes fixSizes = Frame::FixSizes::FixAuto);
    void setStatusBar(gui::StatusBar& sb);
    void setContextMenus(ContextMenus* pContextMenus);
    virtual void onInitialAppearance(); //will be called only once
    
    //onFocus & onFocusLost needs to enabled with enableFocusEvents
    void enableFocusEvents();
    virtual void onFocus(); //will be called when window obtains focus
    virtual void onFocusLost(); //will be called when window is not in focus anymore
    
    virtual bool shouldClose(); //window will not be closed if this method returns false
    //void invokeOnCloseEnvent();
    virtual void onClose(); //will be called only once
    virtual void systemColorModeChanged(bool bDarkMode);
    
    Window(Frame* parent, const gui::Size& size, td::UINT4 wndID); //for dialog
public:
    Window(const gui::Size& sz, Window* pParent = nullptr, td::UINT4 wndID = 0);
    Window(Window* pParent, const gui::Size* pSize = nullptr, td::UINT4 wndID = 0);
    Window(const gui::Geometry& g, Window* pParent = nullptr, td::UINT4 wndID = 0);
    virtual ~Window();
    gui::ObjType getObjType() const override { return ObjType::Window; }
    
    void setGeometry(const Geometry& g, bool redraw=true, Frame::Animation animation = Frame::Animation::SystemDefault);
    Size setSize(const Size& desiredSize, bool redraw = false, bool animate = false);
    void adjustSizeToContentSize(const Size& contentSize, bool animate = false);
    
//    const td::PointAndSize<int>& getInitialRequestedGeometry() const;
    const td::Size<int>& getInitialSize() const;
    
    Window* getParent() const;
    
    gui::Window* getAttachedWindow(td::UINT4 wndID);
    
    gui::FileDialog* getAttachedFileDialog(td::UINT4 wndID);
    
    template <typename TID>
    gui::Window* getAttachedWindow(TID wndID)
    {
        return getAttachedWindow((td::UINT4) wndID);
    }
    
    template <typename TID>
    gui::Dialog* getAttachedDialog(TID wndID)
    {
        gui::Window* pWnd = getAttachedWindow((td::UINT4) wndID);
        if (!pWnd)
            return nullptr;
        gui::Dialog* pDlg = reinterpret_cast<gui::Dialog*>(pWnd);
        return pDlg;
    }
    
    template <typename TID>
    gui::FileDialog* getAttachedFileDialog(TID wndID)
    {
        return getAttachedFileDialog((td::UINT4) wndID);
    }
    
    bool isOpen() const;
    
    void open(Window::FrameSize initialSize = Window::FrameSize::AdjustToContent); //to 'open' window (show it initially)
    void openModal(Window::FrameSize initialSize = Window::FrameSize::AdjustToContent); //to 'open' while disabling others
    
    void close(); //close and destroy it
    
    td::UINT4 getVisualID() const override;
    
    bool isDialog() const;
    std::tuple<bool, bool, bool> isFixedSize() const;
    Frame::FixSizes getFixSizes() const;
    const ToolBar* getToolBar() const;
    ToolBar* getToolBar();
    bool hasHeaderBar() const;
    BaseView* getCentralView();
    const BaseView* getCentralView() const;
    const StatusBar* getStatusBar() const;
    bool isModal() const;
    Window* getParentWindow();
    const Window* getParentWindow() const;
    void keepOnTopOfParent();
    bool isOnTopOfParent() const;
    void setResizable(bool bResizable);
    bool isResizable() const;
    
    //event handlers
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;
    void onActionItem(td::BYTE menuID, td::BYTE actionID, const std::function<void()>& fnToCall); //first and last submenu = 0
    void onActionItem(td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE actionID, td::BYTE lastMenuID, const std::function<void()>& fnToCall);
    
    void freeze();
    void unFreeze();
    bool isFrozen() const;
    
    bool isClosing() const;
    bool isMain() const; //returns true if this window is the main window
    bool isMaximized() const;

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
class NATGLX_API Window
{
public:
    enum class StyleMask : td::WORD {
        Borderless       = 0,
        Titled           = ( 1 << 0 ),
        Closable         = ( 1 << 1 ),
        Miniaturizable   = ( 1 << 2 ),
        Resizable        = ( 1 << 3 ),
        TexturedBackground = ( 1 << 8 ),
        UnifiedTitleAndToolbar = ( 1 << 12 ),
        FullScreen       = ( 1 << 14 ),
        FullSizeContentView = ( 1 << 15 ),
        UtilityWindow    = ( 1 << 4 ),
        DocModalWindow   = ( 1 << 6 ),
        NonactivatingPanel   = ( 1 << 7 )};
protected:
    gui::Handle _handle = nullptr;
private:
    Window() = delete;
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    void operator = (const Window&) = delete;
public:
    Window(const gui::Size& sz, const std::initializer_list<StyleMask>& masks={StyleMask::Closable, StyleMask::Titled});
    Window(const gui::Geometry& g, const std::initializer_list<StyleMask>& masks={StyleMask::Closable, StyleMask::Titled});
    void setCentralView(glx::View* pView);
    gui::Handle getHandle();
    void setTitle(const char* title);
    void setTitle(const td::String& title);
    
};
} //namespace gui

#endif


