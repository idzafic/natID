//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/PasswordEdit.h>
#include <gui/TextEdit.h>
#include <gui/Button.h>
#include <gui/HorizontalLayout.h>
#include <gui/VerticalLayout.h>
#include <gui/GridLayout.h>
#include <gui/Image.h>
#include <gui/GridComposer.h>
#include <gui/Display.h>
#include <cnt/SafeFullVector.h>

//GridLayout in Vertical layout

class ViewVert : public gui::View
{
private:
protected:
    gui::Label _lblUN;
    gui::LineEdit _userName;
    gui::Label _lblPWD;
    gui::PasswordEdit _password;
    gui::Button _btnDestructive;
    gui::Button _btnConstructive;
    gui::Image _imgMenu;
    gui::Button _btnRoundWithMenu; //menu can be on any other type of button
    gui::Button _btnNormal;
    gui::Button _btnDefault;
    gui::Image _imgEye;
    gui::Button _btnDisplayInfo;
    
    //gui::Label _lbl3;
    gui::TextEdit _textEdit;
    gui::GridLayout _glUnAndPwd;
    gui::HorizontalLayout _hlPwd;
    gui::HorizontalLayout _hlButtons;
    gui::VerticalLayout _vl;
public:
    ViewVert()
    : _lblUN("User Name:")
    , _lblPWD("Password:")
    , _btnDestructive("Destructive")
    , _btnConstructive("Constructive")
    , _imgMenu(":menu")
    , _btnRoundWithMenu(&_imgMenu)
    //, _btnRoundWithMenu("+")
    , _btnNormal("Normal")
    , _btnDefault("Default")
    , _imgEye(":eye")
    , _btnDisplayInfo(&_imgEye)
    //, _lbl3("Label for text edit:")
    , _glUnAndPwd(2,2)
    , _hlPwd(2)
    , _hlButtons(7)
    , _vl(5)
    {
        _btnRoundWithMenu.setCircular();
        _btnRoundWithMenu.setMenuID(222); //use it with menu
        _btnRoundWithMenu.setFlat();

        _btnDisplayInfo.setFlat();
        //group password edit field with toogle button
        _hlPwd.setSpaceBetweenCells(0);
        _hlPwd.setMargins(0, 0);
        _hlPwd << _password << _btnDisplayInfo;
        
        //populate grid layout
        gui::GridComposer gc(_glUnAndPwd);
        gc.appendRow(_lblUN); gc.appendCol(_userName);
        gc.appendRow(_lblPWD); gc.appendCol(_hlPwd);
        
        //set button types and put them in _hlButtons (horizontal layout)
        _btnDestructive.setType(gui::Button::Type::Destructive);
        _btnConstructive.setType(gui::Button::Type::Constructive);
        _btnDefault.setType(gui::Button::Type::Default);
        _hlButtons << _btnDestructive << _btnConstructive;
        _hlButtons.appendSpacer();
        _hlButtons.append(_btnRoundWithMenu);
        _hlButtons.appendSpacer();
        _hlButtons << _btnNormal << _btnDefault;
        
        //populate vertical layout
        _vl << _glUnAndPwd << _textEdit << _hlButtons;
        
        setLayout(&_vl);
    }
    
    bool onClick(gui::Button* pBtn) override
    {
        if (pBtn == &_btnDisplayInfo)
        {
            cnt::SafeFullVector<gui::Display::Info> allDisplays;
            gui::Display::getInfoForAll(allDisplays);
            
            size_t nDisp = allDisplays.size();
            td::String str;
            str.format("Detected %zu displays...", nDisp);
            _textEdit.appendString(str);
            size_t i = 0;
            for (auto& d : allDisplays)
            {
                td::String strInfo;
                strInfo.format("Disp#%zu = Logical size (%.0fx%.0f)lpx, physical size(%.2fx%.2f)mm ", i,
                               d.logicalSize.width, d.logicalSize.height, d.physicalSize.width, d.physicalSize.height);
                _textEdit.appendString(strInfo);
                ++i;
            }
            
            //TL and center point of this button on display
            gui::Point tl;
            gui::Point center;
            _btnDisplayInfo.getTLInScreenCoordinates(tl);
            _btnDisplayInfo.getCenterInScreenCoordinates(center);
            td::String strInfo;
            strInfo.format(" ButtonTLOnScreen(%.0f,%.0f), ButtonCenterOnScreen(%.0f,%.0f) ", tl.x, tl.y, center.x, center.y);
            _textEdit.appendString(strInfo);
//            _password.toggleVisibility(); //not safe, keylogger issue, removed!!
            return true;
        }
        return false;
    }

    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        auto pAI = aiDesc.getActionItem();
        
        if (menuID == 222)
        {
            showAlert("Button menu option", "Ove of button menu options selected");
            return true;
        }
        return false;
    }
};
