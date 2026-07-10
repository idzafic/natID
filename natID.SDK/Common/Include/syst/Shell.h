// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Shell.h
    @brief Provides shell-level operations such as opening URLs and launching processes. */
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

/// @brief Provides static methods for common desktop-shell operations.
class MAINUTILS_API Shell
{
public:
    // Open URL in default browser
    /// @brief Opens the specified URL in the system default web browser.
    /// @param url The URL string to open.
    /// @return true if the browser was launched successfully; false otherwise.
    static bool openUrl(const td::String& url);

    // Open default mail client
    /// @brief Composes an email using the system default mail client.
    /// @param to The recipient email address.
    /// @param subject The email subject line (optional, defaults to empty).
    /// @param body The email body text (optional, defaults to empty).
    /// @return true if the mail client was launched successfully; false otherwise.
    static bool sendEmail(const td::String& to,
        const td::String& subject = "",
        const td::String& body = "");

    // Run process with arguments
    /// @brief Launches an external executable with optional arguments.
    /// @param executable Path to the executable file to run.
    /// @param args Vector of arguments to pass to the executable.
    /// @param wait If true, blocks until the launched process completes.
    /// @return true if the process was started successfully; false otherwise.
    static bool runProcess(const td::String& executable,
        const cnt::SafeFullVector<td::String>& args = {},
        bool wait = false); //if wait is true -> wait until executable completes
};

}  //namespace gui
