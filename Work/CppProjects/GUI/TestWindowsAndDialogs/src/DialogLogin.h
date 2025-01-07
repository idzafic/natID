//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Dialog.h>
#include "ViewLogin.h"

class DialogLogin : public gui::Dialog
{
protected:
    ViewLogin _viewLogin;
    
    bool onClick(Dialog::Button::ID btnID, gui::Button* pButton) override
    {
        if (btnID == Dialog::Button::ID::OK)
        {
            td::String un = _viewLogin.getUserName();
            if (un.length() == 0)
            {
                showAlert("Enter UN", "Please enter user name!");
                return false;
            }
            td::String strPwd = _viewLogin.getPassword();
            if (strPwd.cCompare("Sifra") == 0) //samo za primjer
                return true;
            showAlert("Pogresna sifra", "Ukucajte korektnu sifru!");
            return false;
        }
        return true;
    }
public:
    DialogLogin(gui::Control* pView, td::UINT4 wndID = 0)
    : gui::Dialog(pView, { {gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default},
                            {gui::Dialog::Button::ID::Cancel, tr("Cancel")}}, gui::Size(300, 100), wndID)
    {
        setCentralView(&_viewLogin);
//        composeContent();
    }
    
    td::String getUserName() const
    {
        return _viewLogin.getUserName();
    }
};
