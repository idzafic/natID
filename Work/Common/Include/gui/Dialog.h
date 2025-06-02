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
#include <gui/Window.h>
#include <gui/_aux/DialogView.h>
#include <gui/Button.h>
#include <gui/Application.h>

namespace gui
{
class WindowHelper;
class NATGUI_API Dialog : public Window
{
    friend class DialogView;
    friend class WindowHelper;
    enum class EventHandlerType : td::BYTE { Consumer=0, FunctionForSingleButtonWithoutBtnID, FunctionForAllButtons, FunctionForSingleButton, NA};
public:
    class Button
    {
    public:
        enum class ID : td::BYTE {Cancel=0, OK, Apply, Delete, Close, User0, User1, User2, User3, User4, User5, User6, User7, User8, User9, NA};
        gui::Button button;
        Dialog::Button::ID btnID;
        Button()
        : button("")
        , btnID(Button::ID::NA)
        {}
    };
    
    class ButtonDesc
    {
    public:
        td::String title;
        Dialog::Button::ID btnID;
        gui::Button::Type btnType = gui::Button::Type::Normal;
        ButtonDesc()
        : btnID(Button::ID::NA)
        {
        }
        
        ButtonDesc(Button::ID buttonID, const td::String& buttonTitle, gui::Button::Type buttonType = gui::Button::Type::Normal)
        : title(buttonTitle)
        , btnID(buttonID)
        , btnType(buttonType)
        {
        }
    };
    
    static const std::initializer_list<Dialog::ButtonDesc> OkCancel;
protected:
    DialogView _mainView;
    VerticalLayout _vl;
    HorizontalLayout _hlButtons;
    cnt::SafeFullVector<Dialog::Button> _buttons;
    std::function<void(gui::Dialog::Button::ID, gui::Dialog*)> _fnToCallForAllButtons;
    std::function<void(gui::Dialog*)> _fnToCallForSingleButtons;
    Dialog::Button::ID _clickedButtonID = Dialog::Button::ID::NA;
    EventHandlerType _eventHandlerType = EventHandlerType::Consumer;
    Dialog::Button::ID _buttonIDToHandle = Dialog::Button::ID::OK;
    td::BYTE _modal = 0;
    td::BYTE _closing = 0;
private:
    //this is for internal use
    bool shouldClose() override final;
    void onClose() override final;
    void onLostFocus() override final;
    
    bool handleClick(gui::Button* pButton);
    
    size_t appendButton(Dialog::Button::ID btnID, const std::initializer_list<Dialog::ButtonDesc>& buttons);
    void initButtons(const std::initializer_list<Dialog::ButtonDesc>& buttons);
    void composeContent();
    void _setVisualID(td::UINT4 wndID);
protected:
    //users should override this one
    virtual bool onClick(Dialog::Button::ID btnID, gui::Button* pButton);
    
    void closeModal(Dialog::Button::ID closingBtnID);
    void focusButton(Dialog::Button::ID btnID);
private:
    Dialog() = delete;
    Dialog(const Dialog&) = delete;
    Dialog(Dialog&&) = delete;
    void operator = (const Dialog&) = delete;
public:
    Dialog(Frame* pParentFrame, const std::initializer_list<Dialog::ButtonDesc>& buttons, const gui::Size& sz, td::UINT4 dlgID = 0);
    // virtual ~Dialog();
    
    const gui::DialogView& getCentralViewWithButtons() const;
    const Control* getCentralControl() const;
    
    template <class TVIEW>
    TVIEW* getView() const
    {
        const Control* pConstCtrl = getCentralControl();
        
        if (!pConstCtrl)
            return nullptr;
        Control* pCtrl = const_cast<Control*>(pConstCtrl);

        return reinterpret_cast<TVIEW*>(pCtrl);
    }
    
    //set visual ID (wndID) if it has not been already set in constructor, otherwise if will complain
    template <class TINT>
    void setVisualID(TINT wndID)
    {
        _setVisualID((td::UINT4)wndID);
    }
    
    //The consumer will be prompted after closing the dialog (with any button)
    void openModal(gui::IMessageConsumer* pConsumer = nullptr);

    //Function (fnToCal) will be called after closing the dialog (for any button)
    void openModal(std::function<void(gui::Dialog*)> fnToCall);
    
    //Function (fnToCal) will be called after closing the dialog (for any button). Called function has to have two parameters
    void openModal(std::function<void(gui::Dialog::Button::ID, gui::Dialog*)> fnToCall);

    //Function (fnToCal) will be called after closing if the dialog was closed with the specified button ID
    void openModal(gui::Dialog::Button::ID, std::function<void(gui::Dialog*)> fnToCall);
    
    void openNonModal(std::function<void(gui::Dialog*)> fnToCall);
    void openNonModal();

    //get butto id that closed the dialog 
    Dialog::Button::ID getClickedButtonID() const;
    
    template <class Dlg>
    static void show(gui::Frame* parent, bool resizible=true, bool keepOnParent = true)
    {
        auto pDlg = new Dlg(parent);
        if (!resizible)
            pDlg->setResizable(false);
        if (keepOnParent)
            pDlg->keepOnTopOfParent();
        pDlg->open();
    }

    template <class Dlg, typename DLGID>
    static void show(gui::Frame* parent, DLGID dlgID, bool resizible=true, bool keepOnParent = true)
    {
        auto pApp = gui::getApplication();

        gui::Window* pWnd = parent->getParentWindow();
        if (!pWnd)
            pWnd = pApp->getInitialWindow();

        assert(pWnd);
        auto dlg = pWnd->getAttachedWindow(dlgID);
        if (dlg)
        {
            dlg->setFocus();
            return;
        }

        auto pDlg = new Dlg(parent, (td::UINT4) dlgID);
        if (!resizible)
            pDlg->setResizable(false);
        if (keepOnParent)
            pDlg->keepOnTopOfParent();
        pDlg->open();
    }
    
    template <class Dlg, typename DLGID>
    static void show(gui::Frame* parent, const td::String& title, DLGID dlgID,  bool resizible=true, bool keepOnParent = true)
    {
        auto pApp = gui::getApplication();

        gui::Window* pWnd = parent->getParentWindow();
        if (!pWnd)
            pWnd = pApp->getInitialWindow();

        assert(pWnd);
        auto dlg = pWnd->getAttachedWindow(dlgID);
        if (dlg)
        {
            dlg->setFocus();
            return;
        }
        
        auto pDlg = new Dlg(parent, (td::UINT4) dlgID);
        pDlg->setTitle(title);
        if (!resizible)
            pDlg->setResizable(false);
        if (keepOnParent)
            pDlg->keepOnTopOfParent();
        pDlg->open();
    }
};

} //namespace gui
