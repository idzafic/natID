//
//  Created by Izudin Dzafic on 1 Dec 2022.
//  Copyright © 2022 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/NumericEdit.h>
#include <gui/TextEdit.h>
#include <gui/Button.h>
#include <gui/HorizontalLayout.h>
#include <gui/GridLayout.h>
#include <cnt/StringBuilder.h>
#include <gui/GridComposer.h>
#include "DialogLogin.h"
#include "WndID.h"
#include <gui/FileDialog.h>
#include <fo/FileOperations.h>
#include "WindowTableEdit.h"
#include <gui/FileDialog.h>
#include <gui/Panel.h>
#include "ViewProgress.h"
#include "ViewProgress2.h"

//#define USE_CALLBACKS

class MainView : public gui::View
{
protected:
    gui::Label _lblName;
    gui::LineEdit _editName;
    gui::Label _lblQuantity;
    gui::NumericEdit _neQuantity;
    gui::Label _lblPrice;
    gui::NumericEdit _nePrice;
    gui::Label _lblValue;
    gui::NumericEdit _neValue;
    gui::Button _btnDelete;
    gui::Button _btnAddWindow;
    gui::Button _btnShowWindow;
//    gui::Button _btnShowLoginDlg;
    gui::Button _btnShowModalLoginDlg;
    gui::Button _btnShowModalLoginDlg2;
    gui::Button _btnShowModalProgressDlg;
    gui::Button _btnShowModalProgressDlg2;
//    gui::Button _btnOpenFileDlgModal;
//    gui::Button _btnSaveFileDlgModal;
    gui::Button _btnAdd;
    gui::TextEdit _textEdit;
    gui::HorizontalLayout _hlButtons;
    gui::GridLayout _gl;
public:
    MainView()
    : _lblName(tr("name"))
    , _lblQuantity(tr("quant"))
    , _neQuantity(td::decimal3, gui::LineEdit::Messages::Send)
    , _lblPrice(tr("price"))
    , _nePrice(td::decimal2, gui::LineEdit::Messages::Send)
    , _lblValue(tr("value"))
    , _neValue(td::decimal2)
    , _btnDelete(tr("delete"))
    , _btnAddWindow("Window (id=0)...")
    , _btnShowWindow("Window (id>0)...")
//    , _btnShowLoginDlg("Login dlg (nonModal)...")
    , _btnShowModalLoginDlg("Login dlg (Modal)...")
    , _btnShowModalLoginDlg2("Login - view only")
    , _btnShowModalProgressDlg("Modal progress")
    , _btnShowModalProgressDlg2("Modal progress 2")
//    , _btnOpenFileDlgModal("Open (Modal)...")
//    , _btnSaveFileDlgModal("Save (Modal)..")
    , _btnAdd(tr("add"))
    , _hlButtons(10)
    , _gl(6,2)
    {
        //set as read only
        _neValue.setAsReadOnly();
        _btnDelete.setType(gui::Button::Type::Destructive);
        _btnAdd.setAsDefault();
        
        _btnShowModalLoginDlg.setType(gui::Button::Type::Constructive);
        _btnShowModalLoginDlg2.setType(gui::Button::Type::Critical);

        //populate horizontal layout with buttons
        _hlButtons << _btnDelete << _btnAddWindow << _btnShowWindow << _btnShowModalLoginDlg << _btnShowModalLoginDlg2 << _btnShowModalProgressDlg << _btnShowModalProgressDlg2;
        _hlButtons.appendSpacer();
        _hlButtons.append(_btnAdd);
        

        //grid can be populated on two ways
        if (false) //change true -> false -> true (same GUI)
        {
            //1. using insert
            _gl.insert(0,0, _lblName); _gl.insert(0,1,_editName);
            _gl.insert(1,0, _lblQuantity); _gl.insert(1,1,_neQuantity);
            _gl.insert(2,0, _lblPrice); _gl.insert(2,1,_nePrice);
            _gl.insert(3,0, _lblValue); _gl.insert(3,1,_neValue);
            _gl.insert(4,0, _hlButtons, -1);
            _gl.insert(5,0, _textEdit, -1);
        }
        else
        {
            //using GridComposer (appendRow & appendCol)
            gui::GridComposer gc(_gl);
            gc.appendRow(_lblName); gc.appendCol(_editName);
            gc.appendRow(_lblQuantity); gc.appendCol(_neQuantity);
            gc.appendRow(_lblPrice); gc.appendCol(_nePrice);
            gc.appendRow(_lblValue); gc.appendCol(_neValue);
            gc.appendRow(_hlButtons, 0); //span columns until the last column
            gc.appendRow(_textEdit, 0); //span columns until the last column
        }
        
        setLayout(&_gl); //set the layout of this view
    }
    
    bool onFinishEdit(gui::LineEdit* pCtrl) override
    {
        if ( (pCtrl == &_neQuantity) || (pCtrl == &_nePrice) )
        {
            td::Variant quant = _neQuantity.getValue();
            td::Variant price = _nePrice.getValue();
            auto value = quant * price;
            td::Decimal2 decVal(value.r8Val());
            _neValue.setValue(decVal);
            return true;
        }
        return false;
    }
    
    void openFile(gui::FileDialog* pFD)
    {
        auto status = pFD->getStatus();
        if (status == gui::FileDialog::Status::OK)
        {
            td::String strFileName = pFD->getFileName();
            td::String strContent;
            if (fo::loadFileContent(strFileName, strContent))
                _textEdit.setText(strContent);
        }
    }
    
    void saveFile(gui::FileDialog* pFD)
    {
        auto status = pFD->getStatus();
        if (status == gui::FileDialog::Status::OK)
        {
            td::String strFileName = pFD->getFileName();
            td::String strContent = _textEdit.getText();
            fo::OutFile f;
            if (fo::createTextFile(f, strFileName))
                f << strContent;
        }            
    }
    
    bool onClick(gui::Button* pBtn) override
    {
        if (pBtn == &_btnAdd)
        {
            cnt::StringBuilderSmall sb;
            sb.appendString(_editName.getText());
            sb.appendCString(", Quantity=");
            sb.appendString(_neQuantity.getText());
            sb.appendCString(", Value=");
            sb.appendString(_neValue.getText());
            sb.appendCString(" ");
            td::String str = sb.toString();
            _textEdit.appendString(str);
            return true;
        }
        else if (pBtn == &_btnDelete)
        {
//            gui::Alert::Answer answ = showYesNoQuestion("Delete text", "Are you sure you want to delete text?", "Yes", "No");
//            if (answ == gui::Alert::Answer::Yes)
//                _textEdit.clean();
            _textEdit.clean();
            return true;
        }
        else if (pBtn == &_btnAddWindow)
        {
            //example: add window without wndID
            gui::Window* pParentWnd = getParentWindow();
            auto pWnd = new WindowTableEdit(pParentWnd);
            pWnd->keepOnTopOfParent();
            pWnd->open();
            return true;
        }
        else if (pBtn == &_btnShowWindow)
        {
            //example: add window with wndID
            //check if already open
            auto pWnd = getAttachedWindow((td::UINT4) WndID::SingleWnd);
            if (pWnd)
                pWnd->setFocus();
            else
            {
                gui::Window* pParentWnd = getParentWindow();
                auto pWnd = new WindowTableEdit(pParentWnd, (td::UINT4) WndID::SingleWnd);
                pWnd->open();
            }
            return true;
        }
        else if (pBtn == &_btnShowModalLoginDlg)
        {
            DialogLogin* dlg = new DialogLogin(pBtn); //centrira se u odnosu na button
            //dlg->openModal(WndID::LoginDlg, this); //Uncomment this line and comment out next one to obtain onClick message
            dlg->openModal([](gui::Dialog* pDlg)
            {
                if (pDlg->getClickedButtonID() == gui::Dialog::Button::ID::OK)
                {
                    DialogLogin* pDlgLogin = (DialogLogin*) pDlg;
                    //do some actions after login
                    td::String info;
                    td::String userName = pDlgLogin->getUserName();
                    info.format("User '%s' managed to login!", userName.c_str());
                    gui::Alert::show("Success!!", info);
                }
            });
            return true;
        }
        else if (pBtn == &_btnShowModalLoginDlg2)
        {
            //gui::showPanel<ViewLoginForDlg>([](gui::Dialog* pDlg)
            //    {
            //        if (pDlg->getClickedButtonID() == gui::Dialog::Button::ID::OK)
            //        {
            //            DialogLogin* pDlgLogin = (DialogLogin*) pDlg;
            //            //do some actions after login
            //            td::String info;
            //            td::String userName = pDlgLogin->getUserName();
            //            info.format("User '%s' managed to login!", userName.c_str());
            //            gui::Alert::show("Success!!", info);
            //        }
            //    },1, 2, nullptr, pBtn);
            //ili ovako:
            td::UINT4 loginDlgID = 2599;
            gui::Panel::show<ViewLogin>(this, tr("Login"), loginDlgID, gui::Dialog::OkCancel, [this](gui::Dialog::Button::ID clickedButtonID, ViewLogin* pViewLogin)
            {
                if (clickedButtonID == gui::Dialog::Button::ID::OK)
                {
                    td::String info;
                    td::String userName = pViewLogin->getUserName();
                    info.format("User '%s' managed to login!", userName.c_str());
                    gui::Alert::show("Success!!", info);
                }
            });
            return true;
        }
        else if (pBtn == &_btnShowModalProgressDlg)
        {
            //Demonstration of Modal dialog with progress indicator that is updated from a thread
            //In this version thread is started outside of the dialog view
            td::UINT4 loginDlgID = 2610;
            auto bCancelled = std::make_shared<std::atomic<bool>>(false);
            
            auto fnBtnClick = [this, bCancelled](gui::Dialog::Button::ID clickedButtonID, ViewProgress* pViewProgress)
            {
                if (clickedButtonID == gui::Dialog::Button::ID::Cancel)
                {
                    bCancelled->store(true);
                }
            };
            
            
            ViewProgress* pProgView = gui::Panel::showModal<ViewProgress>(this, tr("Progress"), loginDlgID, {{gui::Dialog::Button::ID::Cancel, tr("Cancel")}}, fnBtnClick);
            //start thread that counts up to 100, each time sends a message to to pProgView. When it reaches 1, it should invoce close
            
            // Lambda that will run in the background thread
            auto progressTask = [pProgView, bCancelled]() noexcept
            {
                try
                {
                    for (int i = 1; i <= 100; ++i)
                    {
                        if (bCancelled->load(std::memory_order_relaxed))
                            break;

                        const double val = static_cast<double>(i) / 100.0;
                        const bool shouldClose = (i == 100);

                        // <<< CRITICAL: Post to GUI thread! >>>
                        pProgView->setValue(val, shouldClose); 

                        if (shouldClose)
                            break;

                        std::this_thread::sleep_for(std::chrono::milliseconds(80)); // ~80ms for ~8 seconds total
                    }
                }
                catch (...)
                {
                    // log error if desired
                }
            };

            // Start the thread
            auto thread = std::make_shared<std::thread>(progressTask);

            // Optional: detach if you don't want to join (be careful with lifetime)
            thread->detach();
            return true;
        }
        else if (pBtn == &_btnShowModalProgressDlg2)
        {
            //Demonstration of Modal dialog with progress indicator that is updated from a thread
            //In this version thread is started outside of the dialog view
            td::UINT4 loginDlgID = 2615;
            auto fnBtnClick = [this](gui::Dialog::Button::ID clickedButtonID, ViewProgress2* pViewProgress)
            {
                if (clickedButtonID == gui::Dialog::Button::ID::Cancel)
                {
                    pViewProgress->stop();
                }
            };
            
            
            ViewProgress2* pProgView = gui::Panel::showModal<ViewProgress2>(this, tr("Progress"), loginDlgID, {{gui::Dialog::Button::ID::Cancel, tr("Cancel")}}, fnBtnClick);
            //start thread that counts up to 100, each time sends a message to to pProgView. When it reaches 1, it should invoce close
            
            pProgView->start();
            
            return true;
        }
        return false;
    }
    
    
    //check dialog clicks
    bool onClick(gui::Dialog* pDlg, td::UINT4 dlgID) override
    {
        if (dlgID == (td::UINT4) WndID::LoginDlg)
        {
            gui::Dialog::Button::ID btnID = pDlg->getClickedButtonID();
            if (btnID == gui::Dialog::Button::ID::OK)
            {
                DialogLogin* pDlgLogin = (DialogLogin*) pDlg;
                //do some actions after login
                td::String info;
                td::String userName = pDlgLogin->getUserName();
                info.format("User '%s' managed to login!", userName.c_str());
                showAlert("Success!!", info);
            }
            else
            {
                showAlert("Error!", "User didn't login!");
            }
            return true;
        }
        return false;
    }
    
    gui::TextEdit* getTextEdit()
    {
        return &_textEdit;
    }
    
};
