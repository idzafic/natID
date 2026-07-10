// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PropertyView.h
 *  @brief View that displays and manages a set of editable properties. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Properties.h>
#include <gui/ViewScroller.h>
#include <gui/View.h>
#include <gui/PropertyLayout.h>
#include <gui/IProperty.h>
#include <gui/_aux/PropertyView.h>
#include <gui/PropertyValues.h>

namespace gui
{

class PropertyViewScroller;
class ViewScroller;

/// @brief View that renders a collection of Properties and synchronises their values with an IProperty consumer.
class NATGUI_API PropertyView : public View
{
    friend class PropertyViewScroller;

protected:
    IProperty* _pIProp;              ///< Pointer to the property consumer that receives value changes.
    Properties* _properties;         ///< The collection of property descriptors displayed in this view.
    PropertyLayout _layout;          ///< Layout manager that arranges the property controls.
    ViewScroller* _pScroller = nullptr; ///< Optional scroller wrapping this view.
    Canvas* _pCanvas = nullptr;      ///< Optional canvas used for graphical property previews.
    PropertyValues _values;          ///< Shadow copy of the current property values.
    td::BYTE _autoUpdate;            ///< Non-zero if the view should automatically push changes to the consumer.
private:
    /// @brief Reads the current value from a data control and stores it in the values collection.
    /// @param pCtrl Pointer to the data control.
    /// @return True if a value was successfully taken from the control.
    bool takeValueFromDataCtrl(DataCtrl* pCtrl);

    /// @brief Pushes all stored property values back to the corresponding UI controls.
    void putValuesToCtrls();
protected:
    //virtual void measure(CellInfo& cell);
    //virtual void reMeasure(CellInfo& cell);

    /// @brief Handles internal framework messages for the property view.
    /// @param msg The framework message to process.
    /// @return True if the message was consumed.
    bool _processFrameworkMessage(gui::Message& msg) override;

    /// @brief Assigns a new property consumer and optionally updates the displayed data.
    /// @param pPropConsumer Pointer to the new property consumer.
    /// @param bUpdateData If true the controls are refreshed from the consumer immediately.
    void setPropertyConsumer(IProperty* pPropConsumer, bool bUpdateData);

    /// @brief Reads current values from the consumer and refreshes the UI controls.
    /// @param bMakeShadowCopy If true a shadow copy of the values is created for cancel/restore support.
    void updateValues(bool bMakeShadowCopy);

    /// @brief Sets the Properties collection and rebuilds the control layout.
    /// @param pProperties Pointer to the new Properties descriptor.
    void setProperties(Properties* pProperties);

    /// @brief Constructs a PropertyView without a properties collection.
    /// @param autoUpdate True to enable automatic consumer updates on every control change.
    /// @param labelAlignment Horizontal alignment for the property labels.
    PropertyView(bool autoUpdate = false, td::HAlignment labelAlignment = td::HAlignment::Left);

    /// @brief Assigns an external scroller that wraps this view.
    /// @param pScroller Pointer to the scroller.
    void setScroller(ViewScroller* pScroller) override;

    /// @brief Returns the currently assigned scroller.
    /// @return Pointer to the ViewScroller, or nullptr if none is set.
    ViewScroller* getScroller();

    /// @brief Returns the total model (content) size needed to display all properties.
    /// @param modelSize Output size in model coordinates.
    /// @return True if the size was computed successfully.
    bool getModelSize(gui::Size& modelSize) const override;

    /// @brief Called after all child cells have been measured; finalises layout metrics.
    /// @param ci Cell layout information to update.
    void finishedChildrenMeasuring(CellInfo& ci) override;

    /// @brief Assigns the canvas used for property preview rendering.
    /// @param pCanvas Pointer to the canvas.
    void setCanvas(Canvas* pCanvas);

    /// @brief Initialises refresh tracking for the associated canvas.
    void initCanvasRefreshInfo();

    /// @brief Processes pending canvas refresh requests.
    void processCanvasRefresh();

    //for selection changes
    /// @brief Called when editing of a LineEdit control finishes.
    /// @param pCtrl The LineEdit that finished editing.
    /// @return True if the event was handled.
    bool onFinishEdit(gui::LineEdit* pCtrl) override;

    /// @brief Called when a LineEdit control is activated (focused).
    /// @param pCtrl The activated LineEdit.
    /// @return True if the event was handled.
    bool onActivate(gui::LineEdit* pCtrl) override;

    /// @brief Called when a CheckBox is clicked.
    /// @param pBtn The CheckBox that was clicked.
    /// @return True if the event was handled.
    bool onClick(gui::CheckBox* pBtn) override;

    /// @brief Called when the selection in a ComboBox changes.
    /// @param pCmb The ComboBox whose selection changed.
    /// @return True if the event was handled.
    bool onChangedSelection(gui::ComboBox* pCmb) override;

    /// @brief Called when the selection in a DBComboBox changes.
    /// @param pCmb The DBComboBox whose selection changed.
    /// @return True if the event was handled.
    bool onChangedSelection(gui::DBComboBox* pCmb) override;

    /// @brief Called when the selection in a PopoverButton changes.
    /// @param pBtn The PopoverButton whose selection changed.
    /// @return True if the event was handled.
    bool onChangedSelection(gui::PopoverButton* pBtn) override;

    /// @brief Called when a Slider value changes.
    /// @param pSlider The Slider whose value changed.
    /// @return True if the event was handled.
    bool onChangedValue(gui::Slider* pSlider) override;

    //virtual bool onChangedValue(gui::DateTimeEdit* pDTE);
    /// @brief Called when a DateEdit value changes.
    /// @param pDE The DateEdit whose value changed.
    /// @return True if the event was handled.
    bool onChangedValue(gui::DateEdit* pDE) override;

    /// @brief Called when a TimeEdit value changes.
    /// @param pTE The TimeEdit whose value changed.
    /// @return True if the event was handled.
    bool onChangedValue(gui::TimeEdit* pTE) override;

    /// @brief Called when a ColorPicker value changes.
    /// @param pCP The ColorPicker whose value changed.
    /// @return True if the event was handled.
    bool onChangedValue(gui::ColorPicker* pCP) override;

    /// @brief Called when a LinePatternPicker value changes.
    /// @param pLPP The LinePatternPicker whose value changed.
    /// @return True if the event was handled.
    bool onChangedValue(gui::LinePatternPicker* pLPP) override;
public:
    /// @brief Constructs a PropertyView that immediately displays the given Properties collection.
    /// @param properties Pointer to the properties descriptor to display.
    /// @param autoUpdate True to enable automatic consumer updates on every control change.
    /// @param labelAlignment Horizontal alignment for the property labels.
    PropertyView(Properties* properties, bool autoUpdate = false, td::HAlignment labelAlignment = td::HAlignment::Left);

    /// @brief Destructor.
    ~PropertyView();

    /// @brief Sets the horizontal alignment for all property labels.
    /// @param hAlign The desired horizontal alignment.
    void setLabelAlignment(td::HAlignment hAlign);

    /// @brief Applies all current control values to the property consumer.
    void apply();

    /// @brief Restores all controls to the shadow copy values, discarding pending edits.
    void restore();
};

} //namespace gui
