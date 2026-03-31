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
#include <mu/muLib.h>
#include <cnt/SafeFullVector.h>
#include <td/String.h>


namespace syst
{

class MAINUTILS_API Shell
{
public:
    // Open URL in default browser
    static bool openUrl(const td::String& url);

    // Open default mail client
    static bool sendEmail(const td::String& to,
        const td::String& subject = "",
        const td::String& body = "");

    // Run process with arguments
    static bool runProcess(const td::String& executable,
        const cnt::SafeFullVector<td::String>& args = {},
        bool wait = false); //if wait is true -> wait until executable completes
};

}  //namespace gui
