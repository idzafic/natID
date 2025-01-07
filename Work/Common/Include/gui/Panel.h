// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <gui/Dialog.h>

namespace gui
{

namespace Panel
{

//without variable arguments (view doesn't require parameters in the constructor)
template <class ViewDlg>
class __Panel : public gui::Dialog
{
    protected:
    ViewDlg _view;
    public:
    //simple: without lambda and variable arguments
    __Panel(Frame* pParentFrame, td::UINT4 dlgID, const td::String& title, const std::initializer_list<gui::Dialog::ButtonDesc>& buttons, const gui::Size& sz)
    : gui::Dialog(pParentFrame, buttons, sz, dlgID)
    {
        setTitle(title);
        setCentralView(&_view);
    }
};

//with variable arguments that will be passed to the view's constructor (view requie parameters in the constructor)
template <class ViewDlg, typename... Args>
class _Panel : public gui::Dialog
{
protected:
    ViewDlg _view;
public:
    //with all possibilities
    _Panel(Frame* pParentFrame, const td::String& title, const gui::Size& sz, td::UINT4 dlgID, const std::initializer_list<gui::Dialog::ButtonDesc>& buttons, Args&&... args)
    : gui::Dialog(pParentFrame, buttons, sz, dlgID)
    , _view(std::forward<Args>(args)...)
    {
        setTitle(title);
        setCentralView(&_view);
    }
};

#define MinDlgSize gui::Size(300, 100)

//show dialog without handler
template <class ViewDlg>
inline void show(Frame* pParentFrame, td::UINT4 dlgID, const td::String& title, const std::initializer_list<Dialog::ButtonDesc>& buttons)
{
    auto pApp = gui::getApplication();
    auto wnd = pApp->getMainWnd();
    gui::Window* pMainWnd = (gui::Window*) wnd;
    assert(pMainWnd);
    auto dlg = pMainWnd->getAttachedWindow(dlgID);
    if (dlg)
    {
        dlg->setFocus();
        return;
    }
    
    auto panel = new __Panel<ViewDlg>(pParentFrame, dlgID, title, buttons, MinDlgSize);
    panel->keepOnTopOfParent();
    panel->open();
}

//show dialog without handler with default OK button, no args to the view
template <class ViewDlg>
inline void showOK(Frame* pParentFrame, td::UINT4 dlgID, const td::String& title)
{
    auto pApp = gui::getApplication();
    auto wnd = pApp->getMainWnd();
    gui::Window* pMainWnd = (gui::Window*) wnd;
    assert(pMainWnd);
    auto dlg = pMainWnd->getAttachedWindow(dlgID);
    if (dlg)
    {
        dlg->setFocus();
        return;
    }
    
    auto panel = new __Panel<ViewDlg>(pParentFrame, dlgID, title, {{gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default}}, MinDlgSize);
    panel->keepOnTopOfParent();
    panel->open();
}

//show dialog without handler with arguments to the view
template <class ViewDlg, typename... Args>
inline void show(Frame* pParentFrame, td::UINT4 dlgID, const td::String& title, const std::initializer_list<Dialog::ButtonDesc>& buttons, Args&&... args)
{
    auto pApp = gui::getApplication();
    auto wnd = pApp->getMainWnd();
    gui::Window* pMainWnd = (gui::Window*) wnd;
    assert(pMainWnd);
    auto dlg = pMainWnd->getAttachedWindow(dlgID);
    if (dlg)
    {
        dlg->setFocus();
        return;
    }
    
    auto panel = new _Panel<ViewDlg, Args...>(pParentFrame, title, MinDlgSize, dlgID, buttons, std::forward<Args>(args)...);
    panel->keepOnTopOfParent();
    panel->open();
}

//show dialog without handler with default OK button, and args to the view
template <class ViewDlg, typename... Args>
inline void showOK(Frame* pParentFrame, td::UINT4 dlgID, const td::String& title, Args&&... args)
{
    auto pApp = gui::getApplication();
    auto wnd = pApp->getMainWnd();
    gui::Window* pMainWnd = (gui::Window*) wnd;
    assert(pMainWnd);
    auto dlg = pMainWnd->getAttachedWindow(dlgID);
    if (dlg)
    {
        dlg->setFocus();
        return;
    }
    
    auto panel = new _Panel<ViewDlg, Args...>(pParentFrame, title, MinDlgSize, dlgID, {{gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default}}, std::forward<Args>(args)...);
    panel->keepOnTopOfParent();
    panel->open();
}


template <class ViewDlg, typename... Args>
inline void show(Frame* pParentFrame, const td::String& title, const gui::Size& sz, td::UINT4 dlgID, const std::initializer_list<Dialog::ButtonDesc>& buttons, std::function<void(gui::Dialog*)> fnToCall, Args&&... args)
{
    auto panel = new _Panel<ViewDlg, Args...>(pParentFrame, title, sz, dlgID, buttons, std::forward<Args>(args)...);
    panel->keepOnTopOfParent();
    panel->openNonModal(fnToCall);
}

template <class ViewDlg, typename... Args>
inline void show(Frame* pParentFrame, const td::String& title, td::UINT4 dlgID, const std::initializer_list<Dialog::ButtonDesc>& buttons, std::function<void(gui::Dialog*)> fnToCall, Args&&... args)
{
    auto panel = new _Panel<ViewDlg, Args...>(pParentFrame, title, MinDlgSize, dlgID, buttons, std::forward<Args>(args)...);
    panel->keepOnTopOfParent();
    panel->openNonModal(fnToCall);
}

template <class ViewDlg, typename... Args>
inline void show(Frame* pParentFrame, const td::String& title, const gui::Size& sz, td::UINT4 dlgID, const std::initializer_list<Dialog::ButtonDesc>& buttons, std::function<void(gui::Dialog::Button::ID, ViewDlg*)> fnToCall, Args&&... args)
{
    auto panel = new _Panel<ViewDlg, Args...>(pParentFrame, title, sz, dlgID, buttons, std::forward<Args>(args)...);
    panel->keepOnTopOfParent();
    panel->openNonModal([fnToCall](gui::Dialog* pDlg)
    {
        gui::Control* ctrl = const_cast<gui::Control*>(pDlg->getCentralControl());
        ViewDlg* view = dynamic_cast<ViewDlg*>(ctrl);
        gui::Dialog::Button::ID clickedBtnID = pDlg->getClickedButtonID();
        fnToCall(clickedBtnID, view);
    });
}

template <class ViewDlg, typename... Args>
inline void show(Frame* pParentFrame, const td::String& title, td::UINT4 dlgID, const std::initializer_list<Dialog::ButtonDesc>& buttons, std::function<void(gui::Dialog::Button::ID, ViewDlg*)> fnToCall, Args&&... args)
{
    auto panel = new _Panel<ViewDlg, Args...>(pParentFrame, title, MinDlgSize, dlgID, buttons, std::forward<Args>(args)...);
    panel->keepOnTopOfParent();
    panel->openNonModal([fnToCall](gui::Dialog* pDlg)
    {
        gui::Control* ctrl = const_cast<gui::Control*>(pDlg->getCentralControl());
        ViewDlg* view = dynamic_cast<ViewDlg*>(ctrl);
        gui::Dialog::Button::ID clickedBtnID = pDlg->getClickedButtonID();
        fnToCall(clickedBtnID, view);
    });
}


} //namespace Panel

} //namespace gui
