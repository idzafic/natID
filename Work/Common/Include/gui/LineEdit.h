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
#include "TextCtrl.h"
#include <gui/Key.h>

namespace gui
{
	class NATGUI_API LineEdit : public TextCtrl
	{
    protected:
        const char* _trailingTxt = nullptr; //must be kept at client
    private:
        std::function<void()> _onBeginEdit; //on first change
        std::function<void()> _onChangedContent;
        std::function<void()> _onActivate; //when user presses Enter
        std::function<void()> _onActivateCmd; //when user presses Cmd/Ctrl+Enter
        std::function<void()> _onFinishEdit; //on lost focus
    private:
        void checkMessageLevl(td::BYTE requiredLevel);
    public:
        enum class Messages : unsigned char {DoNotSend=0, Send, SendAll};
    protected:
        void measure(CellInfo&) override;
        void reMeasure(CellInfo&) override;
        const char* getTrailingTxt() const override;
        bool onKeyPressed(const gui::Key& key) override;
        LineEdit(td::DataType dataType);
    public:
        LineEdit(Messages sendMsg = Messages::DoNotSend);
        LineEdit(const td::String& toolTip, Messages sendMsg = Messages::DoNotSend);
        ~LineEdit();
        virtual gui::ObjType getObjType() const override { return ObjType::LineEdit;}
        void setTrailingTxt(const char* trailingTxt); //trailingTxt must be kept at client
        void setHAlignment(td::HAlignment textAlignment) const;
        void setTruncationMode(td::TextEllipsize ellips) const;
        
        const std::function<void()>& getBeginEditHandler() const;
        const std::function<void()>& getChangedContentHandler() const;
        const std::function<void()>& getActivateHandler() const;
        const std::function<void()>& getActivateCmdHandler() const;
        const std::function<void()>& getFinishEditHandler() const;
        
        void onBeginEdit(const std::function<void()>& fnToCall); //first change after obtaining focus
        void onChangedContent(const std::function<void()>& fnToCall);
        void onActivate(const std::function<void()>& fnToCall); //pressed enter
        void onActivateCmd(const std::function<void()>& fnToCall); //pressed Cmd/Ctrl + Enter
        void onFinishEdit(const std::function<void()>& fnToCall); //on lost focu
	};
}
