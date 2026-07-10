// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PropertyViewScroller.h
    @brief Scrollable container for a property editor view. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/_aux/PropertyView.h>

namespace gui
{
class PropertyEditor;

/// @brief A ViewScroller that wraps a PropertyView, providing scrolling for property editors.
class NATGUI_API PropertyViewScroller : public ViewScroller
{
    friend PropertyEditor;

    PropertyView _propView; ///< The scrollable property view hosted inside this scroller.
protected:
    /// @brief Replaces the properties displayed in the embedded property view.
    /// @param properties Pointer to the new Properties object to display.
    void setProperties(Properties* properties);

    /// @brief Returns a pointer to the embedded property view.
    /// @return Pointer to the PropertyView.
    PropertyView* getPropertyView();

    PropertyViewScroller(const PropertyViewScroller&) = delete;
    PropertyViewScroller& operator =(const PropertyViewScroller&) = delete;
public:
    /// @brief Constructs a scroller backed by the given properties.
    /// @param properties Pointer to the initial Properties to display.
    /// @param autoUpdate If true, the view refreshes automatically when property values change.
    /// @param labelAlignment Horizontal alignment applied to property labels.
    PropertyViewScroller(Properties* properties, bool autoUpdate = false, td::HAlignment labelAlignment = td::HAlignment::Left);

    /// @brief Constructs a scroller without initial properties.
    /// @param autoUpdate If true, the view refreshes automatically when property values change.
    /// @param labelAlignment Horizontal alignment applied to property labels.
    PropertyViewScroller(bool autoUpdate = false, td::HAlignment labelAlignment = td::HAlignment::Left);

    /// @brief Destructor.
    ~PropertyViewScroller();

    /// @brief Propagates the parent frame assignment to the embedded property view.
    /// @param pFrame New parent frame.
    void setParentFrame(Frame* pFrame) override;

    /// @brief Sets the horizontal alignment for label cells in the embedded view.
    /// @param hAlign Desired horizontal alignment.
    void setLabelAlignment(td::HAlignment hAlign);

    /// @brief Connects the scroller to a property consumer and optionally triggers an update.
    /// @param propConsumer Property consumer that supplies and receives values.
    /// @param bUpdate If true, all values are refreshed immediately.
    void setPropertyConsumer(gui::IProperty* propConsumer, bool bUpdate);

    /// @brief Associates a canvas with the property view so it can be invalidated on changes.
    /// @param pCanvas Canvas to link.
    void setCanvas(Canvas* pCanvas);

    /// @brief Refreshes all displayed property values from the current consumer.
    void updateValues();
};

} //namespace gui
