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
#include <cnt/SafeFullVector.h>

namespace gui
{

//class DataCtrl;

class NATGUI_API PropertyValues
{
    class ValueDescriptor
    {
    public:
        //DataCtrl* pCtrl;
        td::Variant value;
        td::Variant shadowValue;
        td::UINT4 key;
        ValueDescriptor()
        //: pCtrl(nullptr)
        : value(td::DataType::TD_NONE)
        , shadowValue(td::DataType::TD_NONE)
        , key(0)
        {}
    };

protected:
    cnt::SafeFullVector<ValueDescriptor> _values;
    gui::Rect _rectBeforeUpdate;
    gui::Rect _rectAfterUpdate;
    td::INT2 _lastChangedValuePos = -1;
    td::BYTE _refreshCanvas = 0;
protected:
    int findPosition(td::UINT4 key) const;
public:
    PropertyValues();
    PropertyValues(td::UINT2 nProps);
    ~PropertyValues();
    void reserve(td::UINT2 nProps);
    void init(td::UINT2 position, td::UINT4 key, const td::Variant& value);
    //void setControl(td::UINT2 position, DataCtrl* pCtrl);
    void setValueByPosition(td::UINT2 pos, const td::Variant& value);
    void setValueByKey(td::UINT4 key, const td::Variant& value);
    void makeShadowCopy();
    void takeFromShadowCopy();
    const td::Variant& getValueByPosition(td::UINT2 pos) const;
    const td::Variant& getValueByKey(td::UINT4 key) const;
    const td::Variant& getShadowValueByPosition(td::UINT2 pos) const;
    td::UINT4 getKey(td::UINT2 pos) const;
    void resetCanvasUpdateRects();
    const gui::Rect getRectBeforeUpdate() const;
    const gui::Rect getRectAfterUpdate() const;
    void setRectBeforeUpdate(const gui::Rect& r);
    void setRectAfterUpdate(const gui::Rect& r);
    bool isCanvasUpdateRequiered() const;
    void setUpdateCanvas(bool bUpdate = true);
    void setLastChangedValuePos(td::INT2 pos);
    td::INT2 getLastChangedValuePos() const;
    const td::Variant& getLastChangedValue() const;
};

} //namepace gui

