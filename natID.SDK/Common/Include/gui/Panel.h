// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Panel.h
    @brief Template helpers for creating and showing dialog panels with embedded views. */
#pragma once
#include <gui/Dialog.h>
#include <concepts>
#include <type_traits>

namespace gui
{

/// @brief Concept satisfied when T is derived from gui::Dialog::CloseChecker.
/// @tparam T The type to check.
template <typename T>
concept CloseCheckerType = std::derived_from<T, gui::Dialog::CloseChecker>;

/// @brief Concept satisfied when T exposes a static getDialogButtonAlignment() returning td::HAlignment.
/// @tparam T The type to check.
template<typename T>
concept HasStaticDialogButtonAlignment = requires
{
    { T::getDialogButtonAlignment() } -> std::same_as<td::HAlignment>;
};

namespace Panel
{

//without variable arguments (view doesn't require parameters in the constructor)

/// @brief Internal dialog wrapper for a view that requires no constructor arguments.
/// @tparam ViewDlg The view type to embed as the central control of the dialog.
template <class ViewDlg>
class __Panel : public gui::Dialog
{
protected:
    ViewDlg _view; ///< The embedded view that serves as the dialog's central control.
public:
    //simple: without lambda and variable arguments

    /// @brief Constructs the panel dialog and sets the embedded view as the central control.
    /// @param pParentFrame Pointer to the parent frame (window or dialog).
    /// @param dlgID Unique identifier for this dialog instance.
    /// @param title Title string displayed in the dialog title bar.
    /// @param buttons Initialiser list of button descriptors shown at the bottom of the dialog.
    /// @param sz Initial size of the dialog.
    __Panel(Frame* pParentFrame, td::UINT4 dlgID, const td::String& title, const std::initializer_list<gui::Dialog::ButtonDesc>& buttons, const gui::Size& sz)
    : gui::Dialog(pParentFrame, buttons, sz, dlgID)
    {
        setTitle(title);
        setCentralView(&_view);
    }
};

//with variable arguments that will be passed to the view's constructor (view requie parameters in the constructor)

/// @brief Internal dialog wrapper for a view that requires constructor arguments.
/// @tparam ViewDlg The view type to embed as the central control of the dialog.
/// @tparam Args Variadic argument types forwarded to ViewDlg's constructor.
template <class ViewDlg, typename... Args>
class _Panel : public gui::Dialog
{
protected:
    ViewDlg _view; ///< The embedded view that serves as the dialog's central control.
public:
    //with all possibilities

    /// @brief Constructs the panel dialog and forwards extra arguments to the embedded view.
    /// @param pParentFrame Pointer to the parent frame (window or dialog).
    /// @param title Title string displayed in the dialog title bar.
    /// @param sz Initial size of the dialog.
    /// @param dlgID Unique identifier for this dialog instance.
    /// @param buttons Initialiser list of button descriptors shown at the bottom.
    /// @param args Arguments forwarded to the ViewDlg constructor.
    _Panel(Frame* pParentFrame, const td::String& title, const gui::Size& sz, td::UINT4 dlgID, const std::initializer_list<gui::Dialog::ButtonDesc>& buttons, Args&&... args)
    : gui::Dialog(pParentFrame, buttons, sz, dlgID)
    , _view(std::forward<Args>(args)...)
    {
        setTitle(title);
        setCentralView(&_view);
    }

    /// @brief Constructs the panel dialog with an explicit button alignment and forwarded view arguments.
    /// @param pParentFrame Pointer to the parent frame (window or dialog).
    /// @param title Title string displayed in the dialog title bar.
    /// @param sz Initial size of the dialog.
    /// @param dlgID Unique identifier for this dialog instance.
    /// @param buttons Initialiser list of button descriptors shown at the bottom.
    /// @param buttonAlignment Horizontal alignment of the button row.
    /// @param args Arguments forwarded to the ViewDlg constructor.
    _Panel(Frame* pParentFrame, const td::String& title, const gui::Size& sz, td::UINT4 dlgID, const std::initializer_list<gui::Dialog::ButtonDesc>& buttons, td::HAlignment buttonAlignment, Args&&... args)
    : gui::Dialog(pParentFrame, buttons, buttonAlignment, sz, dlgID)
    , _view(std::forward<Args>(args)...)
    {
        setTitle(title);
        setCentralView(&_view);
    }

    /// @brief Returns a pointer to the embedded view.
    /// @return Pointer to the ViewDlg instance owned by this panel.
    ViewDlg* getView()
    {
        return &_view;
    }
};

#define MinDlgSize gui::Size(300, 100)

/// @brief Shows a non-modal dialog containing ViewDlg with no callback and no view constructor args.
/// @tparam ViewDlg The view type to embed.
/// @param pParentFrame Pointer to the parent frame.
/// @param dlgID Unique dialog identifier.
/// @param title Dialog title.
/// @param buttons Initialiser list of button descriptors.
//show dialog without handler
template <class ViewDlg>
inline void show(Frame* pParentFrame, td::UINT4 dlgID, const td::String& title, const std::initializer_list<Dialog::ButtonDesc>& buttons)
{
    //auto pApp = gui::getApplication();
    //auto wnd = pApp->getMainWnd();
    //gui::Window* pMainWnd = (gui::Window*) wnd;
    //assert(pMainWnd);
    //auto dlg = pMainWnd->getAttachedWindow(dlgID);
    //if (dlg)
    //{
    //    dlg->setFocus();
    //    return;
    //}
    gui::Window* pParentWnd = nullptr;
    auto frameType = pParentFrame->getObjType();
    if (frameType == gui::ObjType::Window)
        pParentWnd = dynamic_cast<gui::Window*>(pParentFrame);
    else
        pParentWnd = pParentFrame->getParentWindow();
    assert(pParentWnd);
    auto dlg = pParentWnd->getAttachedWindow(dlgID);
    if (dlg)
    {
        dlg->setFocus();
        return;
    }

    auto panel = new __Panel<ViewDlg>(pParentFrame, dlgID, title, buttons, MinDlgSize);
    panel->keepOnTopOfParent();
    panel->open();
}

/// @brief Shows a non-modal dialog with a single default OK button and no view constructor args.
/// @tparam ViewDlg The view type to embed.
/// @param pParentFrame Pointer to the parent frame.
/// @param dlgID Unique dialog identifier.
/// @param title Dialog title.
//show dialog without handler with default OK button, no args to the view
template <class ViewDlg>
inline void showOK(Frame* pParentFrame, td::UINT4 dlgID, const td::String& title)
{
    gui::Window* pParentWnd = nullptr;
    auto frameType = pParentFrame->getObjType();
    if (frameType == gui::ObjType::Window)
        pParentWnd = dynamic_cast<gui::Window*>(pParentFrame);
    else
        pParentWnd = pParentFrame->getParentWindow();
    assert(pParentWnd);
    auto dlg = pParentWnd->getAttachedWindow(dlgID);
    if (dlg)
    {
        dlg->setFocus();
        return;
    }

    auto panel = new __Panel<ViewDlg>(pParentFrame, dlgID, title, {{gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default}}, MinDlgSize);
    panel->keepOnTopOfParent();
    panel->open();
}

/// @brief Shows a non-modal OK dialog and invokes a callback when the dialog is dismissed.
/// @tparam ViewDlg The view type to embed.
/// @param pParentFrame Pointer to the parent frame.
/// @param dlgID Unique dialog identifier.
/// @param title Dialog title.
/// @param fnToCall Callback invoked with the closed dialog pointer when the dialog is dismissed.
template <class ViewDlg>
inline void showOK(Frame* pParentFrame, td::UINT4 dlgID, const td::String& title, std::function<void(gui::Dialog*)> fnToCall)
{
    gui::Window* pParentWnd = nullptr;
    auto frameType = pParentFrame->getObjType();
    if (frameType == gui::ObjType::Window)
        pParentWnd = dynamic_cast<gui::Window*>(pParentFrame);
    else
        pParentWnd = pParentFrame->getParentWindow();

    assert(pParentWnd);
    auto dlg = pParentWnd->getAttachedWindow(dlgID);
    if (dlg)
    {
        dlg->setFocus();
        return;
    }

    auto panel = new __Panel<ViewDlg>(pParentFrame, dlgID, title, {{gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default}}, MinDlgSize);
    panel->keepOnTopOfParent();
    panel->openNonModal(fnToCall);
}

/// @brief Shows a non-modal dialog with custom buttons and forwarded view constructor arguments.
/// @tparam ViewDlg The view type to embed.
/// @tparam Args Variadic argument types forwarded to ViewDlg's constructor.
/// @param pParentFrame Pointer to the parent frame.
/// @param dlgID Unique dialog identifier.
/// @param title Dialog title.
/// @param buttons Initialiser list of button descriptors.
/// @param args Arguments forwarded to the ViewDlg constructor.
//show dialog without handler with arguments to the view
template <class ViewDlg, typename... Args>
inline void show(Frame* pParentFrame, td::UINT4 dlgID, const td::String& title, const std::initializer_list<Dialog::ButtonDesc>& buttons, Args&&... args)
{
    gui::Window* pParentWnd = nullptr;
    auto frameType = pParentFrame->getObjType();
    if (frameType == gui::ObjType::Window)
        pParentWnd = dynamic_cast<gui::Window*>(pParentFrame);
    else
        pParentWnd = pParentFrame->getParentWindow();

    assert(pParentWnd);
    auto dlg = pParentWnd->getAttachedWindow(dlgID);
    if (dlg)
    {
        dlg->setFocus();
        return;
    }

    auto panel = new _Panel<ViewDlg, Args...>(pParentFrame, title, MinDlgSize, dlgID, buttons, std::forward<Args>(args)...);
    panel->keepOnTopOfParent();
    panel->open();
}

/// @brief Shows a non-modal OK dialog with forwarded view constructor arguments.
/// @tparam ViewDlg The view type to embed.
/// @tparam Args Variadic argument types forwarded to ViewDlg's constructor.
/// @param pParentFrame Pointer to the parent frame.
/// @param dlgID Unique dialog identifier.
/// @param title Dialog title.
/// @param args Arguments forwarded to the ViewDlg constructor.
//show dialog without handler with default OK button, and args to the view
template <class ViewDlg, typename... Args>
inline void showOK(Frame* pParentFrame, td::UINT4 dlgID, const td::String& title, Args&&... args)
{
    gui::Window* pParentWnd = nullptr;
    auto frameType = pParentFrame->getObjType();
    if (frameType == gui::ObjType::Window)
        pParentWnd = dynamic_cast<gui::Window*>(pParentFrame);
    else
        pParentWnd = pParentFrame->getParentWindow();

    assert(pParentWnd);
    auto dlg = pParentWnd->getAttachedWindow(dlgID);
    if (dlg)
    {
        dlg->setFocus();
        return;
    }

    auto panel = new _Panel<ViewDlg, Args...>(pParentFrame, title, MinDlgSize, dlgID, {{gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default}}, std::forward<Args>(args)...);
    panel->keepOnTopOfParent();
    panel->open();
}


/// @brief Shows a non-modal dialog with an explicit size, a dismiss callback, and forwarded view args.
/// @tparam ViewDlg The view type to embed.
/// @tparam Args Variadic argument types forwarded to ViewDlg's constructor.
/// @param pParentFrame Pointer to the parent frame.
/// @param title Dialog title.
/// @param sz Explicit initial size for the dialog.
/// @param dlgID Unique dialog identifier.
/// @param buttons Initialiser list of button descriptors.
/// @param fnToCall Callback invoked with the dialog pointer when the dialog is dismissed.
/// @param args Arguments forwarded to the ViewDlg constructor.
template <class ViewDlg, typename... Args>
inline void show(Frame* pParentFrame, const td::String& title, const gui::Size& sz, td::UINT4 dlgID, const std::initializer_list<Dialog::ButtonDesc>& buttons, std::function<void(gui::Dialog*)> fnToCall, Args&&... args)
{
    gui::Window* pParentWnd = nullptr;
    auto frameType = pParentFrame->getObjType();
    if (frameType == gui::ObjType::Window)
        pParentWnd = dynamic_cast<gui::Window*>(pParentFrame);
    else
        pParentWnd = pParentFrame->getParentWindow();
    assert(pParentWnd);
    auto dlg = pParentWnd->getAttachedWindow(dlgID);
    if (dlg)
    {
        dlg->setFocus();
        return;
    }
    auto panel = new _Panel<ViewDlg, Args...>(pParentFrame, title, sz, dlgID, buttons, std::forward<Args>(args)...);
    panel->keepOnTopOfParent();
    panel->openNonModal(fnToCall);
}

/// @brief Shows a non-modal dialog using the minimum size, a dismiss callback, and forwarded view args.
/// @tparam ViewDlg The view type to embed.
/// @tparam Args Variadic argument types forwarded to ViewDlg's constructor.
/// @param pParentFrame Pointer to the parent frame.
/// @param title Dialog title.
/// @param dlgID Unique dialog identifier.
/// @param buttons Initialiser list of button descriptors.
/// @param fnToCall Callback invoked with the dialog pointer when the dialog is dismissed.
/// @param args Arguments forwarded to the ViewDlg constructor.
template <class ViewDlg, typename... Args>
inline void show(Frame* pParentFrame, const td::String& title, td::UINT4 dlgID, const std::initializer_list<Dialog::ButtonDesc>& buttons, std::function<void(gui::Dialog*)> fnToCall, Args&&... args)
{
    gui::Window* pParentWnd = nullptr;
    auto frameType = pParentFrame->getObjType();
    if (frameType == gui::ObjType::Window)
        pParentWnd = dynamic_cast<gui::Window*>(pParentFrame);
    else
        pParentWnd = pParentFrame->getParentWindow();

    assert(pParentWnd);
    auto dlg = pParentWnd->getAttachedWindow(dlgID);
    if (dlg)
    {
        dlg->setFocus();
        return;
    }

    auto panel = new _Panel<ViewDlg, Args...>(pParentFrame, title, MinDlgSize, dlgID, buttons, std::forward<Args>(args)...);
    panel->keepOnTopOfParent();
    panel->openNonModal(fnToCall);
}

/// @brief Shows a non-modal dialog with an explicit size; callback receives the button ID and view pointer.
/// @tparam ViewDlg The view type to embed.
/// @tparam Args Variadic argument types forwarded to ViewDlg's constructor.
/// @param pParentFrame Pointer to the parent frame.
/// @param title Dialog title.
/// @param sz Explicit initial size for the dialog.
/// @param dlgID Unique dialog identifier.
/// @param buttons Initialiser list of button descriptors.
/// @param fnToCall Callback receiving the clicked button ID and a pointer to the embedded view.
/// @param args Arguments forwarded to the ViewDlg constructor.
template <class ViewDlg, typename... Args>
inline void show(Frame* pParentFrame, const td::String& title, const gui::Size& sz, td::UINT4 dlgID, const std::initializer_list<Dialog::ButtonDesc>& buttons, std::function<void(gui::Dialog::Button::ID, ViewDlg*)> fnToCall, Args&&... args)
{
    gui::Window* pParentWnd = nullptr;
    auto frameType = pParentFrame->getObjType();
    if (frameType == gui::ObjType::Window)
        pParentWnd = dynamic_cast<gui::Window*>(pParentFrame);
    else
        pParentWnd = pParentFrame->getParentWindow();
    assert(pParentWnd);
    auto dlg = pParentWnd->getAttachedWindow(dlgID);
    if (dlg)
    {
        dlg->setFocus();
        return;
    }

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

/// @brief Shows a non-modal dialog using the minimum size; callback receives the button ID and view pointer.
/// @tparam ViewDlg The view type to embed.
/// @tparam Args Variadic argument types forwarded to ViewDlg's constructor.
/// @param pParentFrame Pointer to the parent frame.
/// @param title Dialog title.
/// @param dlgID Unique dialog identifier.
/// @param buttons Initialiser list of button descriptors.
/// @param fnToCall Callback receiving the clicked button ID and a pointer to the embedded view.
/// @param args Arguments forwarded to the ViewDlg constructor.
template <class ViewDlg, typename... Args>
inline void show(Frame* pParentFrame, const td::String& title, td::UINT4 dlgID, const std::initializer_list<Dialog::ButtonDesc>& buttons, std::function<void(gui::Dialog::Button::ID, ViewDlg*)> fnToCall, Args&&... args)
{
    gui::Window* pParentWnd = nullptr;
    auto frameType = pParentFrame->getObjType();
    if (frameType == gui::ObjType::Window)
        pParentWnd = dynamic_cast<gui::Window*>(pParentFrame);
    else
        pParentWnd = pParentFrame->getParentWindow();
    assert(pParentWnd);
    auto dlg = pParentWnd->getAttachedWindow(dlgID);
    if (dlg)
    {
        dlg->setFocus();
        return;
    }

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

/// @brief Shows a button-less non-modal dialog with forwarded view constructor arguments.
/// @tparam ViewDlg The view type to embed.
/// @tparam Args Variadic argument types forwarded to ViewDlg's constructor.
/// @param pParentFrame Pointer to the parent frame.
/// @param title Dialog title.
/// @param dlgID Unique dialog identifier.
/// @param args Arguments forwarded to the ViewDlg constructor.
template <class ViewDlg, typename... Args>
inline void showWithoutButtons(Frame* pParentFrame, const td::String& title, td::UINT4 dlgID, Args&&... args)
{
    gui::Window* pParentWnd = nullptr;
    auto frameType = pParentFrame->getObjType();
    if (frameType == gui::ObjType::Window)
        pParentWnd = dynamic_cast<gui::Window*>(pParentFrame);
    else
        pParentWnd = pParentFrame->getParentWindow();
    assert(pParentWnd);
    auto dlg = pParentWnd->getAttachedWindow(dlgID);
    if (dlg)
    {
        dlg->setFocus();
        return;
    }
    auto panel = new _Panel<ViewDlg, Args...>(pParentFrame, title, MinDlgSize, dlgID, {}, std::forward<Args>(args)...);
    panel->keepOnTopOfParent();
    panel->openNonModal();
}


//Modal part
//No handler

/// @brief Shows a modal dialog with no dismiss callback and no view constructor arguments.
/// @tparam ViewDlg The view type to embed.
/// @param pParentFrame Pointer to the parent frame.
/// @param dlgID Unique dialog identifier.
/// @param title Dialog title.
/// @param buttons Initialiser list of button descriptors.
template <class ViewDlg>
inline void showModal(Frame* pParentFrame, td::UINT4 dlgID, const td::String& title, const std::initializer_list<Dialog::ButtonDesc>& buttons)
{
    //auto pApp = gui::getApplication();
    //auto wnd = pApp->getMainWnd();
    //gui::Window* pMainWnd = (gui::Window*) wnd;
    //assert(pMainWnd);
    //auto dlg = pMainWnd->getAttachedWindow(dlgID);
    //if (dlg)
    //{
    //    dlg->setFocus();
    //    return;
    //}
    gui::Window* pParentWnd = nullptr;
    auto frameType = pParentFrame->getObjType();
    if (frameType == gui::ObjType::Window)
        pParentWnd = dynamic_cast<gui::Window*>(pParentFrame);
    else
        pParentWnd = pParentFrame->getParentWindow();
    assert(pParentWnd);
    auto dlg = pParentWnd->getAttachedWindow(dlgID);
    if (dlg)
    {
        dlg->setFocus();
        return;
    }

    auto panel = new __Panel<ViewDlg>(pParentFrame, dlgID, title, buttons, MinDlgSize);
    //panel->keepOnTopOfParent();
    panel->openModal();
}

/// @brief Shows a modal dialog with both button-click and dialog-open callbacks plus forwarded view args.
/// @tparam ViewDlg The view type to embed.
/// @tparam Args Variadic argument types forwarded to ViewDlg's constructor.
/// @param pParentFrame Pointer to the parent frame.
/// @param title Dialog title.
/// @param dlgID Unique dialog identifier.
/// @param buttons Initialiser list of button descriptors.
/// @param fnToCallOnButtonClick Callback receiving the clicked button ID and embedded view pointer.
/// @param fnToCallOnDialogOpen Callback invoked with the embedded view pointer when the dialog opens.
/// @param args Arguments forwarded to the ViewDlg constructor.
/// @return Pointer to the embedded ViewDlg instance, or nullptr if already open and focused.
template <class ViewDlg, typename... Args>
inline ViewDlg* showModal(Frame* pParentFrame, const td::String& title, td::UINT4 dlgID,
    const std::initializer_list<Dialog::ButtonDesc>& buttons,
    std::function<void(gui::Dialog::Button::ID, ViewDlg*)> fnToCallOnButtonClick,
    std::function<void(ViewDlg*)> fnToCallOnDialogOpen,
    Args&&... args)
{
    gui::Window* pParentWnd = nullptr;
    auto frameType = pParentFrame->getObjType();
    if (frameType == gui::ObjType::Window)
        pParentWnd = dynamic_cast<gui::Window*>(pParentFrame);
    else
        pParentWnd = pParentFrame->getParentWindow();
    assert(pParentWnd);
    auto dlg = pParentWnd->getAttachedWindow(dlgID);
    if (dlg)
    {
        dlg->setFocus();
        return dynamic_cast<ViewDlg*>(dlg->getCentralView());
    }

    auto panel = new _Panel<ViewDlg, Args...>(pParentFrame, title, MinDlgSize, dlgID, buttons, std::forward<Args>(args)...);
    //panel->keepOnTopOfParent();
    panel->openModal([fnToCallOnButtonClick](gui::Dialog* pDlg)
    {
        gui::Control* ctrl = const_cast<gui::Control*>(pDlg->getCentralControl());
        ViewDlg* view = dynamic_cast<ViewDlg*>(ctrl);
        gui::Dialog::Button::ID clickedBtnID = pDlg->getClickedButtonID();
        fnToCallOnButtonClick(clickedBtnID, view);
    });
    return dynamic_cast<ViewDlg*>(panel->getCentralView());
}

/// @brief Shows a modal dialog with a button-click callback and forwarded view constructor arguments.
/// @tparam ViewDlg The view type to embed.
/// @tparam Args Variadic argument types forwarded to ViewDlg's constructor.
/// @param pParentFrame Pointer to the parent frame.
/// @param title Dialog title.
/// @param dlgID Unique dialog identifier.
/// @param buttons Initialiser list of button descriptors.
/// @param fnToCallOnButtonClick Callback receiving the clicked button ID and embedded view pointer.
/// @param args Arguments forwarded to the ViewDlg constructor.
/// @return Pointer to the embedded ViewDlg instance, or nullptr if the dialog could not be created.
template <class ViewDlg, typename... Args>
inline ViewDlg* showModal(Frame* pParentFrame, const td::String& title, td::UINT4 dlgID,
    const std::initializer_list<Dialog::ButtonDesc>& buttons,
    std::function<void(gui::Dialog::Button::ID, ViewDlg*)> fnToCallOnButtonClick,
    Args&&... args)
{
    gui::Window* pParentWnd = nullptr;
    auto frameType = pParentFrame->getObjType();
    if (frameType == gui::ObjType::Window)
        pParentWnd = dynamic_cast<gui::Window*>(pParentFrame);
    else
        pParentWnd = pParentFrame->getParentWindow();
    assert(pParentWnd);
    auto dlgWnd = pParentWnd->getAttachedWindow(dlgID);
    if (dlgWnd)
    {
        dlgWnd->setFocus();
        gui::Dialog* pDlg = dynamic_cast<gui::Dialog*>(dlgWnd);
        const gui::Control* pCentralCtrlC = pDlg->getCentralControl();
        if (pCentralCtrlC)
        {
            gui::Control* pCtrl = const_cast<gui::Control*>(pCentralCtrlC);
            return dynamic_cast<ViewDlg*>(pCtrl);
        }
        return nullptr;
    }

    td::HAlignment hButtonAlign = td::HAlignment::Right;

    if constexpr (HasStaticDialogButtonAlignment<ViewDlg>)
    {
        hButtonAlign = ViewDlg::getDialogButtonAlignment();
    }

    auto panel = new _Panel<ViewDlg, Args...>(pParentFrame, title, MinDlgSize, dlgID, buttons, hButtonAlign, std::forward<Args>(args)...);

    //panel->keepOnTopOfParent();
    panel->openModal([fnToCallOnButtonClick](gui::Dialog* pDlg)
    {
        gui::Control* ctrl = const_cast<gui::Control*>(pDlg->getCentralControl());
        ViewDlg* view = dynamic_cast<ViewDlg*>(ctrl);
        gui::Dialog::Button::ID clickedBtnID = pDlg->getClickedButtonID();
        fnToCallOnButtonClick(clickedBtnID, view);
    });

    ViewDlg* pView = panel->getView();

    if constexpr (CloseCheckerType<ViewDlg>)
    {
        //set close button checker it it's available
        auto* checker = static_cast<gui::Dialog::CloseChecker*>(pView);
        panel->setButtonCloseChecker(checker);
    }

    return pView;
//    const gui::Control* pCentralCtrlC = panel->getCentralControl();
//    if (pCentralCtrlC)
//    {
//        gui::Control* pCtrl = const_cast<gui::Control*>(pCentralCtrlC);
//        ViewDlg* pView = dynamic_cast<ViewDlg*>(pCtrl);
//        if constexpr (CloseCheckerType<ViewDlg>)
//        {
//            //set close button checker it it's available
//            auto* checker = static_cast<gui::Dialog::CloseChecker*>(pView);
//            panel->setButtonCloseChecker(checker);
//        }
//
//        return pView;
//    }
//
//    return nullptr;
}


} //namespace Panel

} //namespace gui
