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
class DateEditHelper;

class NATGUI_API DateEdit : public LineEdit
{
    template <class TBASE>
    friend class LineEditHelper;
    friend class DateEditHelper;
private:
    td::Date _val;
    td::Date _minDate;
    td::Date _maxDate;
    std::function<void()> _onChangedValue;
private:
    void updateValue(td::Date dateValue);
protected:
    const char* adjustFinalString(const char* pStrIn);
    void clip(td::Date& date) const;
    static bool _putLedingZeros;
public:
    DateEdit();
    DateEdit(const td::String& toolTip);
    ~DateEdit();
    gui::ObjType getObjType() const override { return ObjType::DateEdit;}
    bool setValue(const td::Variant& val, bool sendMessage=true) override;
    bool getValue(td::Variant& val, bool checkType = false) const override;
    td::Date getValue() const;
    void setValue(const td::Date& d, bool sendMessage = false);
    static void putLeadingZeros(bool bPutLeadingZeros);

    const std::function<void()>& getChangedValueHandler() const;
    void onChangedValue(const std::function<void()>& fnToCall);
};

}
