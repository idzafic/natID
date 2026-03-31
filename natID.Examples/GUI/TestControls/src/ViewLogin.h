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
public:
    ViewLogin()
    : _lblUn("User name:")
    , _editUN(gui::LineEdit::Messages::Send)
    , _lblPwd("Password:")
    , _editPwd(gui::LineEdit::Messages::Send)
    //, _lbl3("Label for text edit:")
    , _gl(2,2)
    {
        _gl.insert(0,0, _lblUn);
        _gl.insert(0,1, _editUN);
        _gl.insert(1,0, _lblPwd);
        _gl.insert(1,1, _editPwd);
        
        setLayout(&_gl);
    }
    
    td::String getUserName() const
    {
        return _editUN.getText();
    }
    
    td::String getPassword() const
    {
        return _editPwd.getText();
    }
};
