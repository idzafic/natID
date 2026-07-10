// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Properties.h @brief Declares the Properties container that holds a list of Property descriptors. */
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

/// @brief Owns and manages an ordered collection of Property objects used by property editors.
class NATGUI_API Properties
{
protected:
    cnt::PushBackVector<Property> _properties; ///< Internal storage for all property descriptors.

    Properties(const Properties&) = delete;
    Properties& operator =(const Properties&) = delete;
public:
    /// @brief Default constructor.
    Properties();

    /// @brief Constructs a Properties container with pre-reserved capacity.
    /// @param nProps Initial capacity for property storage.
    Properties(size_t nProps);

    /// @brief Destructor.
    ~Properties();

    /// @brief Reserves storage capacity for the given number of properties.
    /// @param nProperties Desired capacity.
    void reserve(size_t nProperties);

    /// @brief Appends a copy of the given property.
    /// @param prop Property to append.
    void push_back(const Property& prop);

    /// @brief Appends a default-constructed property and returns a reference to it.
    /// @return Reference to the newly appended property.
    Property& push_back();

    /// @brief Returns a const pointer to the property at the given position.
    /// @param pos Zero-based index.
    /// @return Const pointer to the property, or nullptr if out of range.
    const Property* get(size_t pos) const;

    /// @brief Returns a pointer to the property at the given position.
    /// @param pos Zero-based index.
    /// @return Pointer to the property, or nullptr if out of range.
    Property* get(size_t pos);

    /// @brief Returns the number of properties stored.
    /// @return Property count.
    size_t size() const;

    /// @brief Returns a mutable reference to the underlying property vector.
    /// @return Reference to the internal PushBackVector.
    cnt::PushBackVector<Property>& getAll();

    /// @brief Returns a const reference to the underlying property vector.
    /// @return Const reference to the internal PushBackVector.
    const cnt::PushBackVector<Property>& getAll() const;

    /// @brief Returns counts of groups and plain properties.
    /// @return Tuple of (number of groups, number of plain properties).
    std::tuple<size_t, size_t> getNoOfGroupsAndProps() const;
};

} //namepace gui
