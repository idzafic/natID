// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Alert.h
    @brief Utility class for displaying modal alert and yes/no question dialogs. */
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

/// @brief Provides static methods for showing informational alerts and yes/no question dialogs.
class NATGUI_API Alert
{
    Alert(const Alert&) = delete;
    Alert& operator =(const Alert&) = delete;
public:
    /// @brief Represents the user's answer to a yes/no dialog.
    enum class Answer : td::BYTE {No=0,    ///< User chose No.
                                   Yes,    ///< User chose Yes.
                                   Other,  ///< User chose an alternative option.
                                   Close}; ///< User closed the dialog without choosing.
    /// @brief Callback function type invoked after the user answers a question dialog.
    typedef std::function<void(Alert::Answer)> CallBack;

    /// @brief Shows a simple informational alert with a header and informative text.
    /// @param msgHead The heading text of the alert.
    /// @param msgInfo The informative body text of the alert.
    static void show(const td::String& msgHead, const td::String& msgInfo);

    //will be called on whole app
    /// @brief Shows an asynchronous yes/no question dialog at the application level and calls the provided callback with the user's answer.
    /// @param msgText The main question text.
    /// @param informativeText Additional informative text.
    /// @param strYes Label for the Yes button.
    /// @param strNo Label for the No button.
    /// @param callBackLambda Callback lambda invoked with the user's Answer when the dialog is dismissed.
    static void showYesNoQuestion(const td::String& msgText, const td::String& informativeText, const td::String& strYes, const td::String& strNo, Alert::CallBack callBackLambda);
};

} //namespace gui
