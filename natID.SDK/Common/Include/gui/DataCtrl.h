// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DataCtrl.h
    @brief Declares the DataCtrl base class for data-bound GUI controls with value get/set semantics. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Control.h"
#include <td/Types.h>

namespace gui
{
    class DataCtrlHelper;

	/// @brief Base class for GUI controls that are bound to a typed data value.
	///
	/// DataCtrl extends Control with a data type, control identifier, messaging
	/// infrastructure, and decimal/formatting settings shared by all data-entry
	/// controls (line edits, combo boxes, sliders, date/time pickers, etc.).
	class NATGUI_API DataCtrl : public Control
	{
    public:
        /// @brief Orientation options for controls that can be rendered in multiple directions.
        enum class Orientation
        {
            Horizontal,     ///< Control is laid out horizontally.
            Vertical,       ///< Control is laid out vertically (bottom-up).
            VerticalTopDown ///< Control is laid out vertically, top-down.
        };
    private:
        friend class DataCtrlHelper;
    protected:
        td::UINT2 _ctrlID;             ///< Application-defined control identifier.
        td::UINT2 _maxLen;             ///< Maximum number of characters (or digits) accepted.
        td::DataType _dataType;        ///< Runtime data type this control is bound to.
        td::BYTE _tmpBlockMsg;         ///< Temporary flag to suppress outgoing messages.
        td::BYTE _nDec : 4;            ///< Number of decimal places for numeric display.
        td::BYTE _sendingMessagesIsActivated : 2; ///< Activation level for outgoing change messages.
        td::BYTE _showThSep : 1;       ///< Non-zero if thousands-separator should be shown.
        td::BYTE _disableMsg : 1;      ///< Non-zero if all messaging is permanently disabled.
        td::BYTE _popover : 1;         ///< Non-zero if this control is inside a popover.
        td::BYTE _vertical : 1;        ///< Non-zero if the control is oriented vertically.

    protected:
        DataCtrl(const DataCtrl&) = delete;
        DataCtrl& operator =(const DataCtrl&) = delete;

        /// @brief Constructs a DataCtrl bound to the specified data type.
        /// @param dataType  The td::DataType that this control will hold.
        DataCtrl(td::DataType dataType);

        /// @brief Marks this control as residing inside a popover.
        void setAsPopover();

        /// @brief Assigns the application-defined control identifier.
        /// @param ctrlID  Identifier to assign.
        void setControlID(td::UINT2 ctrlID);

        /// @brief Sends a "begin editing" notification to the registered message consumer.
        void sendBeginEditingMessage();

        /// @brief Sends a "content changed" notification to the registered message consumer.
        void sendChangedContentMessage();

        /// @brief Sends a "finished editing" notification to the registered message consumer.
        void sendFinishedEditingMessage();

        /// @brief Sends an "Enter key pressed" notification to the registered message consumer.
        /// @param bCmd  true if Cmd/Ctrl was held together with Enter.
        void sendEnterPressMessage(bool bCmd);

        /// @brief Sends a "selection changed" notification to the registered message consumer.
        void sendSelChangedMessage();

        /// @brief Sends a "check box toggled" notification to the registered message consumer.
        void sendChkBoxMessage();

        /// @brief Sends a "slider value changed" notification to the registered message consumer.
        void sendSliderMessage();

        /// @brief Sends a "date/time value changed" notification to the registered message consumer.
        void sendDateTimeMessage();

        /// @brief Sends a "colour changed" notification to the registered message consumer.
        void sendColorMessage();

        /// @brief Returns whether message sending is activated at the given level.
        /// @param level  Activation level to query (1 or 2).
        /// @return true if messaging is activated at that level.
        bool isSendingMessagesActivated(td::BYTE level) const;

        /// @brief Returns an optional trailing text string appended after the control's value.
        /// @return Pointer to the trailing text, or nullptr if none.
        virtual const char* getTrailingTxt() const;

    public:
        /// @brief Sets the control's current value from a variant.
        /// @param val          New value to assign.
        /// @param sendMessage  If true, fires the appropriate change notification.
        /// @return true if the value was accepted and applied.
        virtual bool setValue(const td::Variant& val, bool sendMessage=true);

        /// @brief Reads the control's current value into a variant.
        /// @param val        Receives the current value.
        /// @param checkType  If true, type compatibility is verified before assignment.
        /// @return true if a valid value was retrieved.
        virtual bool getValue(td::Variant& val, bool checkType = false) const;

        /// @brief Clears the control's value to an empty/default state.
        /// @param sendMessage  If true, fires the appropriate change notification.
        virtual void clean(bool sendMessage = false);

        /// @brief Returns the data type this control is bound to.
        /// @return The td::DataType of the control's value.
        td::DataType getDataType() const;

        /// @brief Sets the maximum input length (characters or digits) accepted by this control.
        /// @param maxLen  Maximum length value.
        void setMaxLen(td::WORD maxLen);

        /// @brief Returns the maximum input length accepted by this control.
        /// @return Maximum length, or 0 if unrestricted.
        size_t getMaxLen() const;

        /// @brief Permanently disables outgoing change messages for this control.
        void disableMessaging();

        /// @brief Re-enables outgoing change messages after disableMessaging() was called.
        void enableMessaging();

        /// @brief Returns whether this control is currently inside a popover.
        /// @return true if the control is in a popover context.
        bool isPopover() const;

        /// @brief Returns the application-defined control identifier.
        /// @return The control identifier assigned via setControlID().
        td::UINT2 getControlID() const;

        /// @brief Resets the control to its zero/empty value state.
        void toZero();

        /// @brief Returns whether this control is oriented vertically.
        /// @return true if the control is in a vertical orientation.
        bool isVertical() const;
	};
}
