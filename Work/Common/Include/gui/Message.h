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
#include <td/Types.h>
#include <td/Variant.h>

namespace gui
{
class NatObject;
class ContextData;

class Message
{
public:
    enum class Type : unsigned char {MT_NA=0, MT_FocusIn, MT_FocusOut, MT_Enter, MT_CmdEnter, MT_ComboSelectionChanged, MT_TableEditSelectionChanged, MT_TableDblClick,
        MT_BtnClick, MT_CheckBtnClick, MT_DrawableBtnClick, MT_DlgBtnClick, MT_FileDlgClick, MT_AlertClick, MT_MenuActivate,
        MT_ToolbarActivate, MT_ToolbarPopoverChange, MT_PopoverButtonSelChange, MT_ToolbarComboSelectionChanged,
        MT_NavigatorSelectionChanged, MT_StandardTabViewSelectionChanged, MT_TextEditSelectionChaged, MT_CodeViewSelectionChaged,
        MT_TextEdit_Modified,
        MT_TabViewSelectionChanged, MT_TabViewShowView,
        MT_BeginEdit, MT_ChangedContent, MT_ReMeasure, MT_Slider_ValChange, MT_DateTime_ValChange, MT_Color_ValChange, MT_LinePattern_ValChange,
        MT_Timer,
        MT_MouseLClick,  MT_MouseDblClick, MT_MouseRClick, MT_Dialog, MT_Data,
        MT_ServerSocket, MT_ClientSocket,
        MT_Navigator, MT_Initial, MT_ValueSet, MT_TBClick, MT_PropertyGroupClick, MT_CtxMenuUpdate,
        MT_User1, MT_User2, MT_User3, MT_User4};

    enum class Source : unsigned char { MS_NA=0, MS_KeyboardFocusNext, MS_KeyboardFocusPrev, MS_KeyboardOther, MS_MouseBtnLeft, MS_MouseBtnRight, MS_MouseWheel, MS_Dialog, MS_RetFromQuery, MS_Other};

    NatObject* pSource = nullptr;
    const ContextData* pContextData = nullptr;
    class Desc
    {
    public:
        Type type;
        Source source;
        Desc(Type t, Source s)
            : type(t)
            , source(s)
        {}
    };
    
    Desc desc;

    //int acceptStatus=0;

    td::Variant inValue;
    td::Variant supValue;

    inline Type getType() const
    {
        return desc.type;
    }

    const td::Variant& getInValue() const
    {
        return inValue;
    }

    template <typename T>
    void setSupValue(const T& supValueIn)
    {
        supValue = supValueIn;
    }
//    const td::Variant& getRetValue() const
//    {
//        return retValue;
//    }

    inline bool isDataActivated() const
    {
        return ((desc.type == Type::MT_FocusIn) || (desc.type == Type::MT_FocusOut) || (desc.type == Type::MT_ComboSelectionChanged) ||
            (desc.type == Type::MT_ValueSet) || (desc.type == Type::MT_Initial) || (desc.type == Type::MT_BtnClick));
    }

    inline Source getSource() const
    {
        return desc.source;
    }

//    bool isAccepted() const
//    {
//        return (acceptStatus > 0);
//    }

    bool isKeyboard(Source source) const
    {
        return (source >= Source::MS_KeyboardFocusNext) && (source <= Source::MS_KeyboardOther);
    }

    bool isMouse(Source source) const
    {
        return ((source >= Source::MS_MouseBtnLeft) && (source <= Source::MS_MouseWheel));
    }

    Message(NatObject* pMsgField = nullptr, Desc msgType = Desc(Type::MT_FocusOut, Source::MS_Other))
        : pSource(pMsgField)
        , desc(msgType)
//        , acceptStatus(0)
    {}

    Message(const td::Variant& val, NatObject* pMsgField = nullptr, Desc msgType = Desc(Type::MT_Data, Source::MS_Other))
        : inValue(val)
        , pSource(pMsgField)
        , desc(msgType)
//        , acceptStatus(0)
    {}

};

} //gui
