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
class PopoverView;
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

class NATGUI_API Consumer : public NatObject, protected gui::IMessageConsumer
{
protected:
    gui::IMessageConsumer* _pConsumer = nullptr;
protected:
    Consumer();

    //IMessageConsumer interface
    virtual bool onSpecialChar(gui::Object* pSender, char key);
    virtual bool _processFrameworkMessage(gui::Message& msg);
    virtual bool isReadyForMessage() const;
    virtual bool reMeasure();
    
    //user overrides
    
    //for menus and toolbars
    virtual bool onActionItem(gui::ActionItemDescriptor& aiDesc);
    virtual bool onContextMenuUpdate(td::BYTE menuID, gui::ContextMenu* pMenu);
    //for button clicks
    virtual bool onClick(gui::Button* pBtn);
    virtual bool onClick(gui::DrawableButton* pDrawableBtn);
    virtual bool onClick(gui::DrawableButtonIntern* pDrawableBtnIntern);
    virtual bool onClick(gui::DrawableButtonWithDelegate* pDrawableBtnWDlg);
    virtual bool onClick(gui::CheckBox* pBtn);
    virtual bool onDblClick(gui::TableEdit* pTE);
    virtual bool onClick(gui::Dialog* pDlg, td::UINT4 dlgID); //clicked a closing button on Dialog
    virtual bool onClick(gui::FileDialog* pDlg, td::UINT4 dlgID); //clicked Open/Save or cancel status button on a file dialog
    //for Alerts
    virtual bool onAnswer(td::UINT4 questionID, gui::Alert::Answer answer); //clicked Yes/No/Other on Alert dialog
    //for line edits
    virtual bool onBeginEdit(gui::LineEdit* pCtrl);
    virtual bool onChangedContent(gui::LineEdit* pCtrl);
    virtual bool onActivate(gui::LineEdit* pCtrl); //pressed enter
    virtual bool onActivateCmd(gui::LineEdit* pCtrl); //pressed Cmd/Ctrl+Enter
    virtual bool onFinishEdit(gui::LineEdit* pCtrl);
    //for selection changes
    virtual bool onChangedSelection(gui::ComboBox* pCmb);
    virtual bool onChangedSelection(gui::DBComboBox* pCmb);
    virtual bool onChangedSelection(gui::TableEdit* pTE);
    virtual bool onChangedSelection(gui::Navigator* pNavigator);
    virtual bool onChangedSelection(gui::TabView* pNavigator);
    virtual bool onChangedSelection(gui::PopoverButton* pBtn);
    virtual bool onChangedSelection(gui::TextEdit* pTE);
    virtual bool onChangedSelection(gui::StandardTabView* pBtn, int newPos);
    //for value changes
    virtual bool onChangedValue(gui::Slider* pSlider);
    virtual bool onChangedValue(gui::DateEdit* pDE);
    virtual bool onChangedValue(gui::TimeEdit* pTE);
    virtual bool onChangedValue(gui::ColorPicker* pCP);
    virtual bool onChangedValue(gui::LinePatternPicker* pLPP);
    virtual bool onModified(gui::TextEdit* pTE);
    //Popover on toolbar
    virtual bool onToolbarsPopoverSelectionChange(gui::PopoverView* pPOView, td::UINT2 ctrlID, td::UINT2 selection);
    virtual bool onToolbarsComboBoxSelectionChange(gui::ComboBox* pCmb, td::UINT2 ctrlID, int selection);
    virtual bool onToolbarsComboBoxSelectionChange(gui::DBComboBox* pCmb, td::UINT2 ctrlID, int selection);
    //Timers
    virtual bool onTimer(gui::Timer* pTimer);
    //Sockets
    virtual bool onServerSocket(const gui::no::Notification& notification);
    virtual bool onClientSocket(const gui::no::Notification& notification);
    //sockets
//    virtual bool onWebSocket(td::LUINT8 connID, const td::String& webMsg);
    
public:
    void forwardMessagesTo(gui::IMessageConsumer* pConsumer);
    gui::IMessageConsumer* getMessageConsumer();
    gui::IMessageConsumer* getMessageForwardedDestination();
    //user events
    virtual bool handleUserEvent(td::UINT4 eventID, const td::Variant& userParam = td::Variant(td::TD_NONE));
};

} //namespace gui

