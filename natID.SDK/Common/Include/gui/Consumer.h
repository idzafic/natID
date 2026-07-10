// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Consumer.h
    @brief Declares the Consumer base class that handles GUI events and messages for controls and windows. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "NatObject.h"
#include "IMessageConsumer.h"
#include <gui/Alert.h>
#include <gui/ActionItemDescriptor.h>
#include <gui/no/natNet.h>

namespace gui
{

class ActionItem;
class Button;
class Control;
class LineEdit;
class DataCtrl;
class ComboBox;
class DBComboBox;
class CheckBox;
class Slider;
//class DateTimeEdit;
class DateEdit;
class TimeEdit;
class ColorPicker;
class TableEdit;
class Navigator;
class TabView;
class PopoverButton;
class PopoverCanvas;
class LinePatternPicker;
class DrawableButton;
class DrawableButtonIntern;
class DrawableButtonWithDelegate;

class StandardTabView;
class Timer;
class Dialog;
class FileDialog;
class ContextMenu;
class TextEdit;
class DataView;

/// @brief Base class for objects that consume and dispatch GUI events and messages.
///
/// Consumer bridges the NatObject base with the IMessageConsumer interface, providing
/// virtual handlers for all common GUI interactions such as button clicks, edit field
/// changes, selection changes, timers, and socket notifications. Derived classes
/// override only the handlers they need.
class NATGUI_API Consumer : public NatObject, protected gui::IMessageConsumer
{
protected:
    gui::IMessageConsumer* _pConsumer = nullptr; ///< Optional forwarding destination for messages.

    Consumer(const Consumer&) = delete;
    Consumer& operator =(const Consumer&) = delete;
protected:
    /// @brief Default constructor; initialises the consumer with no forwarding target.
    Consumer();

    //IMessageConsumer interface
    /// @brief Called when a special (non-printable) key is pressed.
    /// @param pSender  The GUI object that received the key event.
    /// @param key      The special character code.
    /// @return true if the event was handled, false to allow further processing.
    virtual bool onSpecialChar(gui::Object* pSender, char key);

    /// @brief Internal framework message dispatch; processes low-level framework messages.
    /// @param msg  The message to process.
    /// @return true if the message was handled.
    virtual bool _processFrameworkMessage(gui::Message& msg);

    /// @brief Indicates whether this consumer is ready to process the next message.
    /// @return true if the consumer can accept messages.
    virtual bool isReadyForMessage() const;

    /// @brief Requests a re-measure pass for this consumer's associated layout.
    /// @return true if re-measuring was triggered.
    virtual bool reMeasure();

    //user overrides

    //for menus and toolbars
    /// @brief Called when a menu or toolbar action item is activated.
    /// @param aiDesc  Descriptor identifying the activated action item.
    /// @return true if the action was handled.
    virtual bool onActionItem(gui::ActionItemDescriptor& aiDesc);

    /// @brief Called before a context menu is shown, allowing dynamic menu updates.
    /// @param menuID  Identifier of the context menu about to be shown.
    /// @param pMenu   Pointer to the context menu to update.
    /// @return true if the menu was modified.
    virtual bool onContextMenuUpdate(td::BYTE menuID, gui::ContextMenu* pMenu);

    //for button clicks
    /// @brief Called when a standard Button is clicked.
    /// @param pBtn  Pointer to the clicked button.
    /// @return true if the click was handled.
    virtual bool onClick(gui::Button* pBtn);

    /// @brief Called when a DrawableButton is clicked.
    /// @param pDrawableBtn  Pointer to the clicked drawable button.
    /// @return true if the click was handled.
    virtual bool onClick(gui::DrawableButton* pDrawableBtn);

    /// @brief Called when an internal DrawableButton is clicked.
    /// @param pDrawableBtnIntern  Pointer to the clicked internal drawable button.
    /// @return true if the click was handled.
    virtual bool onClick(gui::DrawableButtonIntern* pDrawableBtnIntern);

    /// @brief Called when a DrawableButtonWithDelegate is clicked.
    /// @param pDrawableBtnWDlg  Pointer to the clicked button with delegate.
    /// @return true if the click was handled.
    virtual bool onClick(gui::DrawableButtonWithDelegate* pDrawableBtnWDlg);

    /// @brief Called when a CheckBox is clicked (toggled).
    /// @param pBtn  Pointer to the clicked check box.
    /// @return true if the click was handled.
    virtual bool onClick(gui::CheckBox* pBtn);

    /// @brief Called when a row in a TableEdit is double-clicked.
    /// @param pTE  Pointer to the table edit that received the double-click.
    /// @return true if the event was handled.
    virtual bool onDblClick(gui::TableEdit* pTE);

    /// @brief Called when a row in a DataView is double-clicked.
    /// @param pDV  Pointer to the data view that received the double-click.
    /// @return true if the event was handled.
    virtual bool onDblClick(gui::DataView* pDV);

    /// @brief Called when a closing button on a Dialog is clicked.
    /// @param pDlg   Pointer to the dialog that was closed.
    /// @param dlgID  Identifier of the dialog.
    /// @return true if the event was handled.
    virtual bool onClick(gui::Dialog* pDlg, td::UINT4 dlgID); //clicked a closing button on Dialog

    /// @brief Called when the Open/Save or Cancel button on a FileDialog is clicked.
    /// @param pDlg   Pointer to the file dialog.
    /// @param dlgID  Identifier of the file dialog.
    /// @return true if the event was handled.
    virtual bool onClick(gui::FileDialog* pDlg, td::UINT4 dlgID); //clicked Open/Save or cancel status button on a file dialog

    //for Alerts
    /// @brief Called when the user answers a Yes/No/Other question on an Alert dialog.
    /// @param questionID  Application-defined question identifier.
    /// @param answer      The answer chosen by the user.
    /// @return true if the answer was handled.
    virtual bool onAnswer(td::UINT4 questionID, gui::Alert::Answer answer); //clicked Yes/No/Other on Alert dialog

    //for line edits
    /// @brief Called when the user begins editing a LineEdit control.
    /// @param pCtrl  Pointer to the line edit control.
    /// @return true if the event was handled.
    virtual bool onBeginEdit(gui::LineEdit* pCtrl);

    /// @brief Called when the content of a LineEdit control changes.
    /// @param pCtrl  Pointer to the line edit control.
    /// @return true if the event was handled.
    virtual bool onChangedContent(gui::LineEdit* pCtrl);

    /// @brief Called when the user presses Enter in a LineEdit control.
    /// @param pCtrl  Pointer to the line edit control.
    /// @return true if the event was handled.
    virtual bool onActivate(gui::LineEdit* pCtrl); //pressed enter

    /// @brief Called when the user presses Cmd/Ctrl+Enter in a LineEdit control.
    /// @param pCtrl  Pointer to the line edit control.
    /// @return true if the event was handled.
    virtual bool onActivateCmd(gui::LineEdit* pCtrl); //pressed Cmd/Ctrl+Enter

    /// @brief Called when the user finishes editing a LineEdit control (loses focus).
    /// @param pCtrl  Pointer to the line edit control.
    /// @return true if the event was handled.
    virtual bool onFinishEdit(gui::LineEdit* pCtrl);

    //for selection changes
    /// @brief Called when the selection in a ComboBox changes.
    /// @param pCmb  Pointer to the combo box.
    /// @return true if the event was handled.
    virtual bool onChangedSelection(gui::ComboBox* pCmb);

    /// @brief Called when the selection in a DBComboBox changes.
    /// @param pCmb  Pointer to the database combo box.
    /// @return true if the event was handled.
    virtual bool onChangedSelection(gui::DBComboBox* pCmb);

    /// @brief Called when the selected row in a TableEdit changes.
    /// @param pTE  Pointer to the table edit control.
    /// @return true if the event was handled.
    virtual bool onChangedSelection(gui::TableEdit* pTE);

    /// @brief Called when the selected row in a DataView changes.
    /// @param pDV  Pointer to the data view control.
    /// @return true if the event was handled.
    virtual bool onChangedSelection(gui::DataView* pDV);

    /// @brief Called when the selection in a Navigator changes.
    /// @param pNavigator  Pointer to the navigator control.
    /// @return true if the event was handled.
    virtual bool onChangedSelection(gui::Navigator* pNavigator);

    /// @brief Called when the active tab in a TabView changes.
    /// @param pNavigator  Pointer to the tab view control.
    /// @return true if the event was handled.
    virtual bool onChangedSelection(gui::TabView* pNavigator);

    /// @brief Called when the selection in a PopoverButton changes.
    /// @param pBtn  Pointer to the popover button.
    /// @return true if the event was handled.
    virtual bool onChangedSelection(gui::PopoverButton* pBtn);

    /// @brief Called when the selection in a TextEdit changes.
    /// @param pTE  Pointer to the text edit control.
    /// @return true if the event was handled.
    virtual bool onChangedSelection(gui::TextEdit* pTE);

    /// @brief Called when the active page of a StandardTabView changes.
    /// @param pBtn    Pointer to the standard tab view.
    /// @param newPos  Zero-based index of the newly selected tab.
    /// @return true if the event was handled.
    virtual bool onChangedSelection(gui::StandardTabView* pBtn, int newPos);

    //for value changes
    /// @brief Called when the value of a Slider changes.
    /// @param pSlider  Pointer to the slider control.
    /// @return true if the event was handled.
    virtual bool onChangedValue(gui::Slider* pSlider);

    /// @brief Called when the value of a DateEdit changes.
    /// @param pDE  Pointer to the date edit control.
    /// @return true if the event was handled.
    virtual bool onChangedValue(gui::DateEdit* pDE);

    /// @brief Called when the value of a TimeEdit changes.
    /// @param pTE  Pointer to the time edit control.
    /// @return true if the event was handled.
    virtual bool onChangedValue(gui::TimeEdit* pTE);

    /// @brief Called when the selected colour in a ColorPicker changes.
    /// @param pCP  Pointer to the colour picker control.
    /// @return true if the event was handled.
    virtual bool onChangedValue(gui::ColorPicker* pCP);

    /// @brief Called when the selected line pattern in a LinePatternPicker changes.
    /// @param pLPP  Pointer to the line pattern picker control.
    /// @return true if the event was handled.
    virtual bool onChangedValue(gui::LinePatternPicker* pLPP);

    /// @brief Called when the text content of a TextEdit is modified.
    /// @param pTE  Pointer to the text edit control.
    /// @return true if the event was handled.
    virtual bool onModified(gui::TextEdit* pTE);

    //Popover on toolbar
    /// @brief Called when the selection in a toolbar popover canvas changes.
    /// @param pPOView   Pointer to the popover canvas.
    /// @param ctrlID    Control identifier within the toolbar.
    /// @param selection New selection index.
    /// @return true if the event was handled.
    virtual bool onToolbarsPopoverSelectionChange(gui::PopoverCanvas* pPOView, td::UINT2 ctrlID, td::UINT2 selection);

    /// @brief Called when the selection in a toolbar ComboBox changes.
    /// @param pCmb      Pointer to the combo box on the toolbar.
    /// @param ctrlID    Control identifier within the toolbar.
    /// @param selection New selection index.
    /// @return true if the event was handled.
    virtual bool onToolbarsComboBoxSelectionChange(gui::ComboBox* pCmb, td::UINT2 ctrlID, int selection);

    /// @brief Called when the selection in a toolbar DBComboBox changes.
    /// @param pCmb      Pointer to the database combo box on the toolbar.
    /// @param ctrlID    Control identifier within the toolbar.
    /// @param selection New selection index.
    /// @return true if the event was handled.
    virtual bool onToolbarsComboBoxSelectionChange(gui::DBComboBox* pCmb, td::UINT2 ctrlID, int selection);

    //Timers
    /// @brief Called when a Timer fires.
    /// @param pTimer  Pointer to the timer that fired.
    /// @return true if the event was handled.
    virtual bool onTimer(gui::Timer* pTimer);

    //Sockets
    /// @brief Called when a server-side socket event occurs.
    /// @param notification  Network notification details.
    /// @return true if the event was handled.
    virtual bool onServerSocket(const gui::no::Notification& notification);

    /// @brief Called when a client-side socket event occurs.
    /// @param notification  Network notification details.
    /// @return true if the event was handled.
    virtual bool onClientSocket(const gui::no::Notification& notification);
    //sockets
//    virtual bool onWebSocket(td::LUINT8 connID, const td::String& webMsg);

public:
    /// @brief Forwards all incoming messages to the specified consumer.
    /// @param pConsumer  Destination consumer that will receive forwarded messages.
    void forwardMessagesTo(gui::IMessageConsumer* pConsumer);

    /// @brief Returns the primary message consumer associated with this object.
    /// @return Pointer to the associated IMessageConsumer, or nullptr.
    gui::IMessageConsumer* getMessageConsumer();

    /// @brief Returns the consumer to which messages are currently being forwarded.
    /// @return Pointer to the forwarding destination, or nullptr if none is set.
    gui::IMessageConsumer* getMessageForwardedDestination();

    //user events
    /// @brief Handles an application-defined user event.
    /// @param eventID     Application-defined event identifier.
    /// @param userParam   Optional variant parameter accompanying the event.
    /// @return true if the event was handled.
    virtual bool handleUserEvent(td::UINT4 eventID, const td::Variant& userParam = td::Variant(td::TD_NONE));
};

} //namespace gui
