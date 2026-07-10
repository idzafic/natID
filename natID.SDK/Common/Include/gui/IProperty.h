// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IProperty.h
    @brief Interface for objects that expose a set of editable properties. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once
#include <gui/PropertyValues.h>

namespace gui
{

/// @brief Abstract interface for objects whose properties can be read and written through a PropertyValues container.
class IProperty
{
public:
//    virtual void onPropertyChange(size_t propPos) = 0;
//    virtual void applyChanges() = 0;

    /// @brief Populates a PropertyValues container with the current property values.
    /// @param propValues Container to fill with the current property values.
    virtual void getValues(PropertyValues& propValues) const = 0;

    /// @brief Applies all property values from a PropertyValues container to this object.
    /// @param propValues Container holding the new property values to apply.
    virtual void setValues(PropertyValues& propValues) = 0;

    /// @brief Sets a single property identified by key from a PropertyValues container.
    /// @param key Identifier of the property to set.
    /// @param propValues Container from which the value is read.
    virtual void setValue(td::UINT4 key, PropertyValues& propValues) = 0;
};

} //namepace gui
