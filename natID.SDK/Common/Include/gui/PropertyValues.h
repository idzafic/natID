// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PropertyValues.h
    @brief Container for key-indexed property values used by property editors. */
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

/// @brief Holds a collection of keyed property values with shadow-copy support for undo/revert.
class NATGUI_API PropertyValues
{
    /// @brief Internal descriptor for a single stored property value.
    class ValueDescriptor
    {
    public:
        //DataCtrl* pCtrl;
        td::Variant value;        ///< Current live value of the property.
        td::Variant shadowValue;  ///< Shadow copy of the value (for revert/undo support).
        td::UINT4 key;            ///< Application-assigned key that identifies this property.

        /// @brief Default constructor. Initialises value and shadowValue to TD_NONE.
        ValueDescriptor()
        //: pCtrl(nullptr)
        : value(td::DataType::TD_NONE)
        , shadowValue(td::DataType::TD_NONE)
        , key(0)
        {}
    };

protected:
    cnt::SafeFullVector<ValueDescriptor> _values; ///< Ordered storage of all property value descriptors.
    gui::Rect _rectBeforeUpdate;  ///< Canvas region that needs repainting before a value is changed.
    gui::Rect _rectAfterUpdate;   ///< Canvas region that needs repainting after a value is changed.
    td::INT2 _lastChangedValuePos = -1; ///< Position of the most recently changed value (-1 if none).
    td::BYTE _refreshCanvas = 0;  ///< Non-zero if a canvas refresh has been requested.
protected:
    /// @brief Searches for a value descriptor by key.
    /// @param key Key to search for.
    /// @return Zero-based position of the matching descriptor, or -1 if not found.
    int findPosition(td::UINT4 key) const;

    PropertyValues(const PropertyValues&) = delete;
    PropertyValues& operator =(const PropertyValues&) = delete;
public:
    /// @brief Default constructor. Creates an empty container.
    PropertyValues();

    /// @brief Constructs the container and pre-allocates storage for the given number of properties.
    /// @param nProps Number of properties to allocate.
    PropertyValues(td::UINT2 nProps);

    /// @brief Destructor.
    ~PropertyValues();

    /// @brief Reserves storage for the expected number of properties.
    /// @param nProps Number of entries to reserve.
    void reserve(td::UINT2 nProps);

    /// @brief Initialises the property at a given position with a key and an initial value.
    /// @param position Zero-based position within the container.
    /// @param key Application key that identifies this property.
    /// @param value Initial value to store.
    void init(td::UINT2 position, td::UINT4 key, const td::Variant& value);

    /// @brief Sets the value at a given position directly.
    /// @param pos Zero-based position.
    /// @param value New value to store.
    void setValueByPosition(td::UINT2 pos, const td::Variant& value);

    /// @brief Sets the value identified by the given key.
    /// @param key Property key.
    /// @param value New value to store.
    void setValueByKey(td::UINT4 key, const td::Variant& value);

    /// @brief Copies all current values into the shadow copy.
    void makeShadowCopy();

    /// @brief Restores all current values from the shadow copy.
    void takeFromShadowCopy();

    /// @brief Returns the current value at a given position.
    /// @param pos Zero-based position.
    /// @return Const reference to the stored value.
    const td::Variant& getValueByPosition(td::UINT2 pos) const;

    /// @brief Returns the current value for the given key.
    /// @param key Property key.
    /// @return Const reference to the stored value.
    const td::Variant& getValueByKey(td::UINT4 key) const;

    /// @brief Returns the shadow-copy value at a given position.
    /// @param pos Zero-based position.
    /// @return Const reference to the shadow value.
    const td::Variant& getShadowValueByPosition(td::UINT2 pos) const;

    /// @brief Returns the key associated with the property at the given position.
    /// @param pos Zero-based position.
    /// @return Property key value.
    td::UINT4 getKey(td::UINT2 pos) const;

    /// @brief Clears both the before- and after-update canvas rectangles.
    void resetCanvasUpdateRects();

    /// @brief Returns the canvas region that should be repainted before a value change.
    /// @return Rectangle defining the dirty region.
    const gui::Rect getRectBeforeUpdate() const;

    /// @brief Returns the canvas region that should be repainted after a value change.
    /// @return Rectangle defining the dirty region.
    const gui::Rect getRectAfterUpdate() const;

    /// @brief Sets the canvas region that must be redrawn before a value change is applied.
    /// @param r Rectangle to store.
    void setRectBeforeUpdate(const gui::Rect& r);

    /// @brief Sets the canvas region that must be redrawn after a value change is applied.
    /// @param r Rectangle to store.
    void setRectAfterUpdate(const gui::Rect& r);

    /// @brief Returns whether a canvas update has been requested.
    /// @return True if at least one canvas rect has been set.
    bool isCanvasUpdateRequiered() const;

    /// @brief Sets or clears the canvas-update-required flag.
    /// @param bUpdate True to request a canvas update.
    void setUpdateCanvas(bool bUpdate = true);

    /// @brief Records the position of the most recently changed property.
    /// @param pos Zero-based position of the changed value.
    void setLastChangedValuePos(td::INT2 pos);

    /// @brief Returns the position of the most recently changed property.
    /// @return Zero-based position, or -1 if no change has been recorded.
    td::INT2 getLastChangedValuePos() const;

    /// @brief Returns the most recently changed value.
    /// @return Const reference to the last changed value variant.
    const td::Variant& getLastChangedValue() const;
};

} //namepace gui
