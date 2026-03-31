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
#include <gui/Types.h>
#include <td/Variant.h>
#include <td/String.h>
#include <tuple>

namespace gui
{

class NATGUI_API Property
{
public:
    enum class Type : td::BYTE  {Group=0, ReadOnly, Normal};
    enum class Presentation : td::BYTE {Default=0, Slider, Combo};
protected:
    td::String _lbl;
    td::Variant _value;
    td::String _ctrlToolTip;
    union
    {
        double _minValue;
        const td::String* _pStringList;
    };
    union
    {
        double _maxValue;
        size_t _nStrings;
    };
    
    td::UINT4 _key;
    Type _type;
    Presentation _presentation;
    char _noDec: 4;
    td::BYTE _thSeparator : 1;
    td::BYTE _useCallbackOnValueChange : 1;
protected:
    
public:
    Property();
    ~Property();
    void set(td::UINT4 key, const td::String& lbl, const td::Variant& val, const td::String& ctrlToolTip="");
    void setReadOnly();
    void setDecimalPointsAndThSep(int noDec, bool showThSep=true);
    void setPresentation(Presentation presentation);
    void setGroup(const td::String& lbl);
    td::UINT4 getKey() const;
    const td::Variant& getValue() const;
    td::Variant& getValue();
    Property::Type getType() const;
    Property::Presentation getPresentation() const;
    bool showThSeparator() const;
    int getNoDec() const;
    const td::String& getLabel() const;
    const td::String& getCtrlToolTip() const;
    void setMinValue(double minValue);
    void setMaxValue(double maxValue);
    double getMinValue() const;
    double getMaxValue() const;
    
    void setStringList(const td::String* pStrings, size_t nStrings);
    std::tuple<const td::String*, size_t> getStringList() const;
};

} //namepace gui

