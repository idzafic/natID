// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PropertyEditor.h
    @brief Provides a combined title and scrollable property view for editing properties. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/PropertyViewScroller.h>
#include <gui/_aux/PropertyEditorTitle.h>
#include <gui/VerticalLayout.h>

namespace gui
{

/// @brief A view combining a title bar with a scrollable property list for property editing.
class NATGUI_API PropertyEditor : public View
{
    PropertyEditorTitle _title;          ///< Title bar displayed above the property list
    PropertyViewScroller _propViewScroller; ///< Scrollable container for the property view
    gui::VerticalLayout _layout;         ///< Vertical layout arranging title and property scroller

protected:
    /// @brief Sets the properties collection to display in this editor.
    /// @param properties Pointer to the properties to display.
    void setProperties(Properties* properties);
    /// @brief Handles click events from drawable symbol buttons.
    /// @param pSymbBtn Pointer to the clicked button.
    /// @return True if the event was handled.
    bool onClick(gui::DrawableButtonIntern* pSymbBtn) override;
    /// @brief Returns the internal property view.
    /// @return Pointer to the property view.
    PropertyView* getPropertyView();

    /// @brief Deleted copy constructor.
    PropertyEditor(const PropertyEditor&) = delete;
    /// @brief Deleted copy assignment operator.
    PropertyEditor& operator =(const PropertyEditor&) = delete;
public:
    /// @brief Constructs a PropertyEditor with a title and initial set of properties.
    /// @param title Title string displayed above the property list.
    /// @param properties Pointer to properties to display initially.
    /// @param autoUpdate If true, the editor auto-updates when values change.
    /// @param labelAlignment Horizontal alignment of property labels.
    PropertyEditor(const td::String& title, Properties* properties, bool autoUpdate = false, td::HAlignment labelAlignment = td::HAlignment::Right);
    /// @brief Constructs a PropertyEditor with a title but no initial properties.
    /// @param title Title string displayed above the property list.
    /// @param autoUpdate If true, the editor auto-updates when values change.
    /// @param labelAlignment Horizontal alignment of property labels.
    PropertyEditor(const td::String& title, bool autoUpdate = false, td::HAlignment labelAlignment = td::HAlignment::Right);

    /// @brief Destructor. Releases editor resources.
    ~PropertyEditor();
    /// @brief Sets the parent frame for this property editor.
    /// @param pFrame Pointer to the parent frame.
    void setParentFrame(Frame* pFrame) override;
    /// @brief Sets the horizontal alignment for property labels.
    /// @param hAlign The desired horizontal alignment.
    void setLabelAlignment(td::HAlignment hAlign);
    /// @brief Sets the horizontal alignment of the title text.
    /// @param hAlign The desired horizontal alignment.
    void setTitleAlignment(td::HAlignment hAlign);
    /// @brief Assigns an IProperty consumer that receives property change notifications.
    /// @param propConsumer Pointer to the consumer implementation.
    /// @param updateData If true, immediately updates the displayed values from the consumer.
    void setPropertyConsumer(gui::IProperty* propConsumer, bool updateData);
    /// @brief Associates a canvas to be refreshed when property values change.
    /// @param pCanvas Pointer to the canvas.
    void setCanvas(Canvas* pCanvas);
    /// @brief Refreshes all displayed property values from the current data source.
    void updateValues();

    //void setGeometry(const Geometry& cellFrame, const Cell& cell);
};

} //namespace gui
