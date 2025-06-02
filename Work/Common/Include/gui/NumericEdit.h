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
#include <tuple>
#include <td/Concepts.h>

namespace gui
{
template <class TBASE>
class LineEditHelper;

class NATGUI_API NumericEdit : public LineEdit
{
    template <class TBASE>
    friend class LineEditHelper;
    double _minValue;
    double _maxValue;
    td::FormatFloat _format = td::FormatFloat::Decimal;
protected:
    std::tuple<bool, const char*> adjustWorkingString(const char* pStrIn, char* buff, size_t buffLen);
    const char* adjustFinalString(const char* pStrIn);
    NumericEdit();
public:
    NumericEdit(td::DataType dataType, LineEdit::Messages sendMsg = LineEdit::Messages::DoNotSend, bool bShowThSep = true, const td::String& toolTip = "", int nDec = -1);
    ~NumericEdit();
    bool getValue(td::Variant& val, bool checkType = false) const override;
    
    td::Variant getValue() const;
    
    template <td::conc::NumericNotBool T>
    void getValue(T& val) const
    {
        td::Variant varVal = getValue();
        varVal.getValue(val);
    }
    
    gui::ObjType getObjType() const override { return ObjType::NumericEdit;}
    void setNumberOfDigitsAfterDecimalPoint(td::BYTE nDecPlace);
    td::BYTE getNumberOfDigitsAfterDecimalPoint() const;
    void showThSep(bool bShowThSep);
    bool isThSepVisible() const;
    void setMinValue(double minVal);
    void setMaxValue(double maxVal);
    double getMinValue() const;
    double getMaxValue() const;
    void setFormat(td::FormatFloat fmt);
    td::FormatFloat getFormat() const;
};

} //namespace gui
