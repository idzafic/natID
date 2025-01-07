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
#include <gui/PropertyValues.h>

namespace gui
{

class IProperty
{
public:
//    virtual void onPropertyChange(size_t propPos) = 0;
//    virtual void applyChanges() = 0;
    virtual void getValues(PropertyValues& propValues) const = 0;
    virtual void setValues(PropertyValues& propValues) = 0;
    virtual void setValue(td::UINT4 key, PropertyValues& propValues) = 0;
};

} //namepace gui

