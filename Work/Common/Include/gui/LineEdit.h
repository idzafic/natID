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

namespace gui
{
	class NATGUI_API LineEdit : public TextCtrl
	{
    protected:
        const char* _trailingTxt = nullptr; //must be kept at client
    private:
        std::function<void()> _onBeginEdit;
        std::function<void()> _onActivate;
        std::function<void()> _onFinishEdit;
    public:
        enum class Messages : unsigned char {DoNotSend=0, Send};
    protected:
        void measure(CellInfo&) override;
        void reMeasure(CellInfo&) override;
        const char* getTrailingTxt() const override;
        LineEdit(td::DataType dataType);
    public:
        LineEdit(Messages sendMsg = Messages::DoNotSend);
        LineEdit(const td::String& toolTip, Messages sendMsg = Messages::DoNotSend);
        ~LineEdit();
        virtual gui::ObjType getObjType() const override { return ObjType::LineEdit;}
        bool isEmpty() const;
        void setTrailingTxt(const char* trailingTxt); //trailingTxt must be kept at client
        void setHAlignment(td::HAlignment textAlignment) const;
        void setTruncationMode(td::TextEllipsize ellips) const;
        const std::function<void()>& getBeginEditHandler() const;
        const std::function<void()>& getActivateHandler() const;
        const std::function<void()>& getFinishEditHandler() const;
        
        void onBeginEdit(const std::function<void()>& fnToCall);
        void onActivate(const std::function<void()>& fnToCall); //pressed enter
        void onFinishEdit(const std::function<void()>& fnToCall);
        
 
	};
}
