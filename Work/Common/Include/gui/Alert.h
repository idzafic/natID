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
#include <gui/Types.h>
#include <td/Types.h>
#include <cnt/SafeFullVector.h>
#include <functional>

namespace gui
{

class NATGUI_API Alert
{
public:
    enum class Answer : td::BYTE {No=0, Yes, Other, Close};
    typedef std::function<void(Alert::Answer)> CallBack;
    
    static void show(const td::String& msgHead, const td::String& msgInfo);
    
    //will be called on whole app 
    static void showYesNoQuestion(const td::String& msgText, const td::String& informativeText, const td::String& strYes, const td::String& strNo, Alert::CallBack callBackLambda);
};

} //namespace gui
