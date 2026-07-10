// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file TextCtrl.h
    @brief Base class for text-input controls with read/write and clipboard support. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "DataCtrl.h"

namespace gui
{
    /// @brief Abstract base for single-line and multi-line text controls.
    class NATGUI_API TextCtrl : public DataCtrl
    {
    protected:
        /// @brief Constructs a TextCtrl with the specified underlying data type.
        /// @param dataType Data type associated with the text value (default: string8).
        TextCtrl(td::DataType dataType = td::string8);
        TextCtrl(const TextCtrl&) = delete;
        TextCtrl& operator =(const TextCtrl&) = delete;
    public:
        /// @brief Sets whether the control is read-only.
        /// @param readOnly True to prevent user editing.
        void setAsReadOnly(bool readOnly=true);

        /// @brief Sets the control's text from a C-string.
        /// @param val Null-terminated string to display.
        void setText(const char* val);

        /// @brief Sets the control's text from a String object.
        /// @param val String value to display.
        /// @param bSendMessage If true, a change notification is sent.
        void setText(const td::String& val, bool bSendMessage = true);

        /// @brief Returns the current text content.
        /// @return The current text as a td::String.
        td::String getText() const;

        /// @brief Returns whether the control contains no text.
        /// @return True if the text is empty.
        bool isEmpty() const;

        /// @brief Returns whether the control allows user editing.
        /// @return True if the control is editable.
        bool isEditable() const override;

        /// @brief Selects all text in the control.
        void selectAll();

        /// @brief Copies the selected text to the clipboard.
        void copy() const;

        /// @brief Cuts the selected text to the clipboard.
        void cut();

        /// @brief Pastes text from the clipboard at the current insertion point.
        void paste();
    };
}
