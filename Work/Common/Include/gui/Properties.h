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
#include <gui/Property.h>
//#include <cnt/PBVector.h>
#include <cnt/PushBackVector.h>
#include <tuple>

namespace gui
{

class NATGUI_API Properties
{
protected:
    //cnt::PBVector<Property> _properties;
    cnt::PushBackVector<Property> _properties;
public:
    Properties();
    Properties(size_t nProps);
    ~Properties();
    
    void reserve(size_t nProperties);
    void push_back(const Property& prop);
    Property& push_back();
    
    const Property* get(size_t pos) const;
    Property* get(size_t pos);
    size_t size() const;
    cnt::PushBackVector<Property>& getAll();
    const cnt::PushBackVector<Property>& getAll() const;
    std::tuple<size_t, size_t> getNoOfGroupsAndProps() const;
};

} //namepace gui

