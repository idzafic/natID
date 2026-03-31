//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/PasswordEdit.h>

#include <gui/GridLayout.h>

class ViewLogin : public gui::View
{
private:
protected:
    gui::Label _lblUn;
    gui::LineEdit _editUN;
    gui::Label _lblPwd;
    gui::PasswordEdit _editPwd;
    gui::GridLayout _gl;
    gui::Button _button;
public:
    ViewLogin()
    : _lblUn("User name:")
    , _lblPwd("Password:")
    , _button("Test")
    , _gl(3,2)
    {
        _editPwd.setToolTip("To pass enter 'Sifra'.");
        _gl.insert(0,0, _lblUn);
        _gl.insert(0,1, _editUN);
        _gl.insert(1,0, _lblPwd);
        _gl.insert(1,1, _editPwd);
        _gl.insert(2,0, _button);
        
        setLayout(&_gl);
        
//        _button.forwardMessagesTo(this);
    }
    
    td::String getUserName() const
    {
        return _editUN.getText();
    }
    
    td::String getPassword() const
    {
        return _editPwd.getText();
    }
    
    bool onClick(gui::Button* pBtn) override
    {
        int k=0;
        ++k;
        gui::Alert::show("Test Button", "Test button clicked and handled in ViewLogin");
        return true;
    }
};

//
//class ViewLoginForDlg : public gui::View
//{
//private:
//protected:
//    gui::Label _lblUn;
//    gui::LineEdit _editUN;
//    gui::Label _lblPwd;
//    gui::PasswordEdit _editPwd;
//    gui::GridLayout _gl;
//    gui::Button _button;
//public:
//    ViewLoginForDlg(int, double, char*)
//        : _lblUn("User name:")
//        , _editUN(gui::LineEdit::Messages::Send)
//        , _lblPwd("Password:")
//        , _editPwd(gui::LineEdit::Messages::Send)
//        , _button("Test")
//        , _gl(3, 2)
//    {
//        _editPwd.setToolTip("To pass enter 'Sifra'.");
//        _gl.insert(0, 0, _lblUn);
//        _gl.insert(0, 1, _editUN);
//        _gl.insert(1, 0, _lblPwd);
//        _gl.insert(1, 1, _editPwd);
//        _gl.insert(2, 0, _button);
//
//        setLayout(&_gl);
//    }
//
//    td::String getUserName() const
//    {
//        return _editUN.getText();
//    }
//
//    td::String getPassword() const
//    {
//        return _editPwd.getText();
//    }
//
//    //virtual td::UINT4 getID() const
//    //{
//    //    return typeid(decltype(*this)).hash_code();
//    //}
//
//     bool onDialogButton(gui::Dialog::Button::ID btnID, gui::Button* pButton)
//    {
//        if (btnID == gui::Dialog::Button::ID::OK)
//        {
//            td::String un = getUserName();
//            if (un.length() == 0)
//            {
//                showAlert("Enter UN", "Please enter user name!");
//                return false;
//            }
//            td::String strPwd = getPassword();
//            if (strPwd.cCompare("Sifra") == 0) //samo za primjer
//                return true;
//            showAlert("Pogresna sifra", "Ukucajte korektnu sifru!");
//            return false;
//        }
//        return true;
//    }
//
//    virtual bool onDialogClose(gui::Dialog::Button::ID btnID, gui::Dialog* pDlg) 
//    {
//        if (btnID == gui::Dialog::Button::ID::OK)
//        {
//             //do some actions after login
//             td::String info;
//             td::String userName = getUserName();
//             info.format("User '%s' managed to login!", userName.c_str());
//             gui::Alert::show("Success!!", info);
//        }
//        return true;
//    }
//
//    virtual bool onDialogClose(gui::Dialog* pDlg) 
//    {
//        if (pDlg->getClickedButtonID() == gui::Dialog::Button::ID::OK)
//        {
//            //do some actions after login
//            td::String info;
//            td::String userName = getUserName();
//            info.format("User '%s' managed to login!", userName.c_str());
//            gui::Alert::show("Success!!", info);
//        }
//        return true;
//    }
//
//};
