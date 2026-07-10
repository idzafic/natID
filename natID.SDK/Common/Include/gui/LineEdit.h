// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file LineEdit.h
    @brief Single-line text input control with support for callbacks and activation events. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "TextCtrl.h"
#include <gui/Key.h>

namespace gui
{
	/// @brief Single-line text editing control that fires callbacks on various editing events.
	class NATGUI_API LineEdit : public TextCtrl
	{
    protected:
        const char* _trailingTxt = nullptr; //must be kept at client ///< Optional suffix text appended after the editable content; must remain valid for the lifetime of the control.
    private:
        std::function<void()> _onBeginEdit; //on first change ///< Callback invoked on the first content change after focus is gained.
        std::function<void()> _onChangedContent; ///< Callback invoked whenever the text content changes.
        std::function<void()> _onActivate; //when user presses Enter ///< Callback invoked when the user presses the Enter key.
        std::function<void()> _onActivateCmd; //when user presses Cmd/Ctrl+Enter ///< Callback invoked when the user presses Cmd/Ctrl+Enter.
        std::function<void()> _onFinishEdit; //on lost focus ///< Callback invoked when the control loses keyboard focus.

    public:
        /// @brief Controls whether content-change and focus messages are sent.
        enum class Messages : unsigned char
        {
            DoNotSend=0, ///< Do not send any messages for content changes or focus events.
            Send,        ///< Send messages for content changes and focus events.
            SendAll      ///< Send all available messages including intermediate edits.
        };
    protected:
        /// @brief Measures the preferred size of this control.
        /// @param cellInfo Cell info to populate with size data.
        void measure(CellInfo&) override;

        /// @brief Re-measures the preferred size of this control after a change.
        /// @param cellInfo Cell info to update with new size data.
        void reMeasure(CellInfo&) override;

        /// @brief Returns the trailing text suffix displayed after the editable content.
        /// @return Pointer to the null-terminated trailing text string.
        const char* getTrailingTxt() const override;

        /// @brief Handles key-press events to implement special key behaviour.
        /// @param key The key that was pressed.
        /// @return True if the key event was consumed.
        bool onKeyPressed(const gui::Key& key) override;

        /// @brief Protected constructor for use by subclasses with a specific data type.
        /// @param dataType The data type this control accepts.
        LineEdit(td::DataType dataType);
        LineEdit(const LineEdit&) = delete;
        LineEdit& operator =(const LineEdit&) = delete;
    public:
        /// @brief Constructs a LineEdit control.
        /// @param sendMsg Message sending mode for content and focus events.
        LineEdit(Messages sendMsg = Messages::DoNotSend);

        /// @brief Constructs a LineEdit control with a tooltip.
        /// @param toolTip Tooltip string displayed on hover.
        /// @param sendMsg Message sending mode for content and focus events.
        LineEdit(const td::String& toolTip, Messages sendMsg = Messages::DoNotSend);

        /// @brief Destructor.
        ~LineEdit();

        /// @brief Returns the object type identifier for this control.
        /// @return ObjType::LineEdit.
        virtual gui::ObjType getObjType() const override { return ObjType::LineEdit;}

        /// @brief Sets the trailing suffix text shown after the editable area.
        /// @param trailingTxt Pointer to a null-terminated string. Must remain valid for the lifetime of the control.
        void setTrailingTxt(const char* trailingTxt); //trailingTxt must be kept at client

        /// @brief Sets the horizontal alignment of text within the control.
        /// @param textAlignment Desired horizontal alignment.
        void setHAlignment(td::HAlignment textAlignment) const;

        /// @brief Sets the text truncation/ellipsis mode when text overflows the control.
        /// @param ellips The truncation mode to apply.
        void setTruncationMode(td::TextEllipsize ellips) const;

        /// @brief Returns the begin-edit callback handler.
        /// @return Const reference to the begin-edit function.
        const std::function<void()>& getBeginEditHandler() const;

        /// @brief Returns the changed-content callback handler.
        /// @return Const reference to the changed-content function.
        const std::function<void()>& getChangedContentHandler() const;

        /// @brief Returns the activate (Enter key) callback handler.
        /// @return Const reference to the activate function.
        const std::function<void()>& getActivateHandler() const;

        /// @brief Returns the activate-command (Cmd/Ctrl+Enter) callback handler.
        /// @return Const reference to the activate-command function.
        const std::function<void()>& getActivateCmdHandler() const;

        /// @brief Returns the finish-edit (focus lost) callback handler.
        /// @return Const reference to the finish-edit function.
        const std::function<void()>& getFinishEditHandler() const;

        /// @brief Assignment operator that sets the control's text content.
        /// @param txt New text value to display.
        void operator = (const td::String& txt)
        {
            setText(txt);
        }

        //for old-style event handling
        /// @brief Enables legacy message-based content-change and focus event handling.
        void activateContentChangeAndFocusEvents();

        /// @brief Sets the callback invoked on the first content change after focus is gained.
        /// @param fnToCall Function to call when editing begins.
        void onBeginEdit(const std::function<void()>& fnToCall); //first change after obtaining focus

        /// @brief Sets the callback invoked whenever the text content changes.
        /// @param fnToCall Function to call on each content change.
        void onChangedContent(const std::function<void()>& fnToCall);

        /// @brief Sets the callback invoked when the user presses the Enter key.
        /// @param fnToCall Function to call on Enter key activation.
        void onActivate(const std::function<void()>& fnToCall); //pressed enter

        /// @brief Sets the callback invoked when the user presses Cmd/Ctrl+Enter.
        /// @param fnToCall Function to call on command activation.
        void onActivateCmd(const std::function<void()>& fnToCall); //pressed Cmd/Ctrl + Enter

        /// @brief Sets the callback invoked when the control loses keyboard focus.
        /// @param fnToCall Function to call when editing finishes.
        void onFinishEdit(const std::function<void()>& fnToCall); //on lost focu
	};
}
