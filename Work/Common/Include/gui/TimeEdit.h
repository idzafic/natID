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
#include "LineEdit.h"
#include <td/DateTime.h>

namespace gui
{
template <class TBASE>
class LineEditHelper;
class TimeEditHelper;

class NATGUI_API TimeEdit : public LineEdit
{
    template <class TBASE>
    friend class LineEditHelper;
    friend class TimeEditHelper;
private:
    td::Time _val;
    td::Time _minTime;
    td::Time _maxTime;
    std::function<void()> _onChangedValue;
private:
    void updateValue(td::Time val);
protected:
    const char* adjustFinalString(const char* pStrIn);
    void clip(td::Time& time) const;
    static bool _putLedingZeros;
public:
    TimeEdit();
    TimeEdit(const td::String& toolTip);
    ~TimeEdit();
    gui::ObjType getObjType() const override { return ObjType::TimeEdit;}
    bool setValue(const td::Variant& val, bool sendMessage=true) override;
    bool getValue(td::Variant& val, bool checkType = false) const override;
    td::Time getValue() const;
    void setValue(const td::Time& t, bool sendMessage = false);
    static void putLeadingZeros(bool bPutLeadingZeros);

    const std::function<void()>& getChangedValueHandler() const;
    void onChangedValue(const std::function<void()>& fnToCall);
};

}
