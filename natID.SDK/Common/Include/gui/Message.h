// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Message.h
    @brief GUI message structure carrying event type, source, and associated data values. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once
#include <td/Types.h>
#include <td/Variant.h>

namespace gui
{
class NatObject;
class ContextData;

/// @brief Encapsulates a GUI event message, including its type, originating source, and payload variants.
class Message
{
public:
    /// @brief Identifies the category of GUI event carried by this message.
    enum class Type : unsigned char
    {
        MT_NA=0,                             ///< No message or uninitialised.
        MT_FocusIn,                          ///< A control gained keyboard focus.
        MT_FocusOut,                         ///< A control lost keyboard focus.
        MT_Enter,                            ///< The Enter key was pressed in a control.
        MT_CmdEnter,                         ///< Command+Enter (or Ctrl+Enter) was pressed.
        MT_ComboSelectionChanged,            ///< The selection in a combo box changed.
        MT_TableEditSelectionChanged,        ///< The selection in an editable table changed.
        MT_TableDblClick,                    ///< A row in a table was double-clicked.
        MT_DataViewSelectionChanged,         ///< The selection in a data view changed.
        MT_DataViewDblClick,                 ///< An item in a data view was double-clicked.
        MT_BtnClick,                         ///< A standard button was clicked.
        MT_CheckBtnClick,                    ///< A check button was clicked.
        MT_DrawableBtnClick,                 ///< A drawable (custom-drawn) button was clicked.
        MT_DlgBtnClick,                      ///< A dialog button was clicked.
        MT_FileDlgClick,                     ///< A file dialog button was clicked.
        MT_AlertClick,                       ///< An alert dialog button was clicked.
        MT_MenuActivate,                     ///< A menu item was activated.
        MT_ToolbarActivate,                  ///< A toolbar item was activated.
        MT_ToolbarPopoverChange,             ///< A toolbar popover selection changed.
        MT_PopoverButtonSelChange,           ///< A popover button selection changed.
        MT_ToolbarComboSelectionChanged,     ///< A toolbar combo box selection changed.
        MT_NavigatorSelectionChanged,        ///< The selection in a Navigator control changed.
        MT_StandardTabViewSelectionChanged,  ///< The tab selection in a standard tab view changed.
        MT_TextEditSelectionChaged,          ///< The selection or cursor in a text editor changed.
        MT_CodeViewSelectionChaged,          ///< The selection or cursor in a code view changed.
        MT_TextEdit_Modified,                ///< The content of a text editor was modified.
        MT_TabViewSelectionChanged,          ///< The tab selection in a custom tab view changed.
        MT_TabViewShowView,                  ///< A tab view is about to show a view.
        MT_BeginEdit,                        ///< An in-place editing session started.
        MT_ChangedContent,                   ///< The content of an editable control changed.
        MT_ReMeasure,                        ///< A layout re-measurement was requested.
        MT_Slider_ValChange,                 ///< A slider value changed.
        MT_DateTime_ValChange,               ///< A date/time control value changed.
        MT_Color_ValChange,                  ///< A color picker value changed.
        MT_LinePattern_ValChange,            ///< A line pattern picker value changed.
        MT_Timer,                            ///< A timer fired.
        MT_MouseLClick,                      ///< The primary (left) mouse button was clicked.
        MT_MouseDblClick,                    ///< A double-click with the primary mouse button occurred.
        MT_MouseRClick,                      ///< The secondary (right) mouse button was clicked.
        MT_Dialog,                           ///< A dialog-related event occurred.
        MT_Data,                             ///< A generic data event carrying a payload variant.
        MT_ServerSocket,                     ///< A server socket event occurred.
        MT_ClientSocket,                     ///< A client socket event occurred.
        MT_Navigator,                        ///< A Navigator-specific event occurred.
        MT_Initial,                          ///< Initial population of a control's value.
        MT_ValueSet,                         ///< A value was programmatically set on a control.
        MT_TBClick,                          ///< A toolbar button was clicked.
        MT_PropertyGroupClick,               ///< A property group header was clicked.
        MT_CtxMenuUpdate,                    ///< A context menu is about to be displayed and needs updating.
        MT_User1,                            ///< Application-defined user message 1.
        MT_User2,                            ///< Application-defined user message 2.
        MT_User3,                            ///< Application-defined user message 3.
        MT_User4                             ///< Application-defined user message 4.
    };

    /// @brief Identifies what triggered or originated the message.
    enum class Source : unsigned char
    {
        MS_NA=0,              ///< No source or uninitialised.
        MS_KeyboardFocusNext, ///< Focus moved forward via keyboard (e.g. Tab).
        MS_KeyboardFocusPrev, ///< Focus moved backward via keyboard (e.g. Shift+Tab).
        MS_KeyboardOther,     ///< Another keyboard interaction caused the event.
        MS_MouseBtnLeft,      ///< The left mouse button originated the event.
        MS_MouseBtnRight,     ///< The right mouse button originated the event.
        MS_MouseWheel,        ///< The mouse wheel originated the event.
        MS_Dialog,            ///< A dialog interaction originated the event.
        MS_RetFromQuery,      ///< The event originated from a return from an async query.
        MS_Other              ///< Some other unclassified source.
    };

    NatObject* pSource = nullptr;             ///< Pointer to the GUI object that generated the message.
    const ContextData* pContextData = nullptr; ///< Optional pointer to additional context data.

    /// @brief Bundles the message type and source into a single descriptor.
    class Desc
    {
    public:
        Type type;   ///< The event type of the message.
        Source source; ///< The input source that triggered the message.

        /// @brief Constructs a Desc with a given type and source.
        /// @param t The message type.
        /// @param s The message source.
        Desc(Type t, Source s)
            : type(t)
            , source(s)
        {}
    };

    Desc desc; ///< Combined type and source descriptor for this message.

    //int acceptStatus=0;

    td::Variant inValue;  ///< Primary input value payload carried by the message.
    td::Variant supValue; ///< Supplementary value payload for additional data.

    /// @brief Returns the event type of this message.
    /// @return The Message::Type of this message.
    inline Type getType() const
    {
        return desc.type;
    }

    /// @brief Returns a const reference to the primary input value.
    /// @return Const reference to the inValue variant.
    const td::Variant& getInValue() const
    {
        return inValue;
    }

    /// @brief Sets the supplementary value payload from a typed value.
    /// @tparam T Type of the value to store.
    /// @param supValueIn The value to store as the supplementary payload.
    template <typename T>
    void setSupValue(const T& supValueIn)
    {
        supValue = supValueIn;
    }
//    const td::Variant& getRetValue() const
//    {
//        return retValue;
//    }

    /// @brief Returns true if this message represents a data-activation event.
    /// @return True for focus, selection-changed, value-set, initial, or button-click events.
    inline bool isDataActivated() const
    {
        return ((desc.type == Type::MT_FocusIn) || (desc.type == Type::MT_FocusOut) || (desc.type == Type::MT_ComboSelectionChanged) ||
            (desc.type == Type::MT_ValueSet) || (desc.type == Type::MT_Initial) || (desc.type == Type::MT_BtnClick));
    }

    /// @brief Returns the source that generated this message.
    /// @return The Message::Source value.
    inline Source getSource() const
    {
        return desc.source;
    }

//    bool isAccepted() const
//    {
//        return (acceptStatus > 0);
//    }

    /// @brief Returns whether the given source corresponds to a keyboard event.
    /// @param source The source to classify.
    /// @return True if the source is keyboard-originated.
    bool isKeyboard(Source source) const
    {
        return (source >= Source::MS_KeyboardFocusNext) && (source <= Source::MS_KeyboardOther);
    }

    /// @brief Returns whether the given source corresponds to a mouse event.
    /// @param source The source to classify.
    /// @return True if the source is mouse-originated.
    bool isMouse(Source source) const
    {
        return ((source >= Source::MS_MouseBtnLeft) && (source <= Source::MS_MouseWheel));
    }

    /// @brief Constructs a Message with an optional source object and descriptor.
    /// @param pMsgField Pointer to the originating NatObject (may be nullptr).
    /// @param msgType Descriptor combining type and source (defaults to MT_FocusOut / MS_Other).
    Message(NatObject* pMsgField = nullptr, Desc msgType = Desc(Type::MT_FocusOut, Source::MS_Other))
        : pSource(pMsgField)
        , desc(msgType)
//        , acceptStatus(0)
    {}

    /// @brief Constructs a Message carrying a variant data payload.
    /// @param val The variant value to carry as the input payload.
    /// @param pMsgField Pointer to the originating NatObject (may be nullptr).
    /// @param msgType Descriptor combining type and source (defaults to MT_Data / MS_Other).
    Message(const td::Variant& val, NatObject* pMsgField = nullptr, Desc msgType = Desc(Type::MT_Data, Source::MS_Other))
        : inValue(val)
        , pSource(pMsgField)
        , desc(msgType)
//        , acceptStatus(0)
    {}

};

} //gui
