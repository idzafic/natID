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
#include "Control.h"
#include <td/Types.h>

namespace gui
{
    class DataCtrlHelper;
	class NATGUI_API DataCtrl : public Control
	{
        friend class DataCtrlHelper;
    protected:
        td::DataType _dataType;
        td::UINT2 _ctrlID;
        td::UINT2 _maxLen;
        td::BYTE _tmpBlockMsg;
        td::BYTE _nDec : 4;
        td::BYTE _sendingMessagesIsActivated : 2;
        td::BYTE _showThSep : 1;
        td::BYTE _disableMsg : 1;
        td::BYTE _popover : 1;
        
    protected:
        DataCtrl(td::DataType dataType);
        void setAsPopover();
        void setControlID(td::UINT2 ctrlID);
        
        void sendBeginEditingMessage();
        void sendChangedContentMessage();
        void sendFinishedEditingMessage();
        void sendEnterPressMessage(bool bCmd);
        void sendSelChangedMessage();
        void sendChkBoxMessage();
        void sendSliderMessage();
        void sendDateTimeMessage();
        void sendColorMessage();
        bool isSendingMessagesActivated(td::BYTE level) const;
        virtual const char* getTrailingTxt() const;
    public:
        virtual bool setValue(const td::Variant& val, bool sendMessage=true);
        virtual bool getValue(td::Variant& val, bool checkType = false) const;
        
        virtual void clean(bool sendMessage = false);
        td::DataType getDataType() const;
        void setMaxLen(td::WORD maxLen);
        size_t getMaxLen() const;
        void disableMessaging();
        void enableMessaging();
        bool isPopover() const;
        td::UINT2 getControlID() const;
        void toZero();
        
	};
}
