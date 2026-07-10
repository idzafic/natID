// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PropertyEditorSwitcher.h
    @brief View switcher managing multiple property editor panels. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once
#include <gui/ViewSwitcher.h>
#include  <initializer_list>
#include <gui/PropertyEditor.h>


namespace gui
{

/// @brief A ViewSwitcher that hosts several PropertyEditor instances and switches between them.
class NATGUI_API PropertyEditorSwitcher : public ViewSwitcher
{
    PropertyEditorSwitcher(const PropertyEditorSwitcher&) = delete;
    PropertyEditorSwitcher& operator =(const PropertyEditorSwitcher&) = delete;
    PropertyEditorSwitcher() = delete;
public:
    /// @brief Constructs a switcher pre-allocated for the given number of editors.
    /// @param nPropEditors Number of property editors to allocate.
    PropertyEditorSwitcher(size_t nPropEditors);

    /// @brief Destructor.
    ~PropertyEditorSwitcher();

    /// @brief Activates the editor at the specified position and populates it with property data.
    /// @param pos Zero-based index of the editor to show.
    /// @param propConsumer Property consumer that supplies and receives property values.
    /// @param bUpdateData If true, the editor values are refreshed from the consumer.
    void setCurrentEditor(int pos, IProperty* propConsumer, bool bUpdateData);

    /// @brief Associates a canvas with the editor at the specified position.
    /// @param pos Zero-based editor index.
    /// @param pCanvas Canvas that the editor should update on property changes.
    void setCanvas(int pos, Canvas* pCanvas);

    /// @brief Refreshes all property values in the currently visible editor.
    void updateValues();

    /// @brief Refreshes the property values in the editor at the specified position.
    /// @param pos Zero-based index of the editor to update.
    void updateValues(int pos);

    /// @brief Returns a pointer to the PropertyEditor at the specified position.
    /// @param pos Zero-based editor index.
    /// @return Pointer to the PropertyEditor, or nullptr if the index is out of range.
    PropertyEditor* getPropertyEditor(int pos);
};

} //namespace gui
