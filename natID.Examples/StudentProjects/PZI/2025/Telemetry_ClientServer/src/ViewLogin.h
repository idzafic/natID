#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/PasswordEdit.h>
#include <gui/GridLayout.h>

class ViewLogin : public gui::View //log in screen for establishing connection
{
private:
protected:
    gui::Label _lblUn;
    gui::LineEdit _editUN;
    gui::Label _lblPwd;
    gui::PasswordEdit _editPwd;
    gui::GridLayout _gl;
public:
    ViewLogin(const td::String& un)
        : _lblUn("User name:")
        , _lblPwd("Password:")
        , _gl(2, 2)
    {
        _editUN.setText(un);
        
        _gl.insert(0, 0, _lblUn);
        _gl.insert(0, 1, _editUN);
        _gl.insert(1, 0, _lblPwd);
        _gl.insert(1, 1, _editPwd);
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
