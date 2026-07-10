// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PropertyLayout.h
    @brief Two-column layout that pairs labels with property editor controls. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Layout.h"
//#include <cnt/PBVector.h>
#include <cnt/PushBackVector.h>
#include <gui/Control.h>
#include <gui/Label.h>
#include <gui/Properties.h>
#include <gui/_aux/PropertyGroupCell.h>

namespace gui
{
    /// @brief Layout that arranges label–control pairs in two aligned columns.
    class NATGUI_API PropertyLayout : public Layout
    {
        /// @brief Classifies a visual row as either a group header or a label/control entry.
        enum class Type : td::BYTE {Group, Entry};

        /// @brief Internal structure describing a single visual row in the layout.
        struct VisualPropery
        {
            Cell labelCell; ///< Cell occupied by the label (or group header).
            Cell ctrlCell;  ///< Cell occupied by the editor control.
            Type type;      ///< Whether this row is a group header or a property entry.
            //td::BYTE collapsed;
        };

    protected:
        cnt::PushBackVector<VisualPropery> _visualProperties; ///< Ordered list of all visual rows.
        td::UINT2 _maxGroupWidth;  ///< Maximum width required by group header rows.
        td::UINT2 _maxLabelWidth;  ///< Maximum width required by label cells.
        td::UINT2 _maxCtrlWidth;   ///< Maximum width required by control cells.

        td::UINT2 _currMinWidth;   ///< Current computed minimum total width.
        td::UINT2 _currMinHeight;  ///< Current computed minimum total height.

        td::BYTE _spaceBetweenRows;  ///< Vertical spacing between consecutive property rows.
        td::BYTE _spaceBeweenCols;   ///< Horizontal spacing between the label and control columns.
        td::HAlignment _labelHAlignment; ///< Horizontal alignment applied to all label cells.

    protected:
        /// @brief Performs an initial complete measurement pass.
        /// @return True if the measurement changed any sizes.
        bool initialMeasure() override;

        /// @brief Fills a CellInfo structure during the initial measurement.
        /// @param cell CellInfo to populate.
        void initialMeasure(CellInfo&) override;

        /// @brief Measures the layout given current cell information.
        /// @param cell CellInfo describing available space.
        void measure(CellInfo& cell) override;

        /// @brief Re-measures the layout after a size change.
        /// @param cell CellInfo describing the new available space.
        void reMeasure(CellInfo& cell) override;

        /// @brief Applies geometry to all contained controls.
        /// @param cellFrame Available geometry.
        /// @param cell Cell descriptor.
        void setGeometry(const Geometry& cellFrame, const Cell& cell) override;

        /// @brief Destroys and removes all visual controls from the layout.
        void deleteVisualControls();

        PropertyLayout(const PropertyLayout&) = delete;
        PropertyLayout& operator =(const PropertyLayout&) = delete;
    public:
        /// @brief Constructs a PropertyLayout pre-allocated for the given number of properties.
        /// @param nProps Estimated number of property rows.
        /// @param labelAlignment Horizontal alignment for label cells.
        PropertyLayout(size_t nProps, td::HAlignment labelAlignment = td::HAlignment::Left);

        /// @brief Constructs a PropertyLayout with default capacity.
        /// @param labelAlignment Horizontal alignment for label cells.
        PropertyLayout(td::HAlignment labelAlignment = td::HAlignment::Left);

        /// @brief Destructor.
        ~PropertyLayout();

        /// @brief Returns the GUI object type identifier.
        /// @return ObjType::PropertyLayout.
        virtual gui::ObjType getObjType() const override { return ObjType::PropertyLayout;}

        /// @brief Reserves storage for the expected number of properties.
        /// @param nProps Number of rows to reserve.
        void reserve(size_t nProps);

        /// @brief Appends a group header row to the layout.
        /// @param pPropertyGroupCell Pointer to the group header cell widget.
        void addGroup(gui::PropertyGroupCell* pPropertyGroupCell);

        /// @brief Appends a label–control pair as a new property row.
        /// @param pLabel Pointer to the label widget.
        /// @param pControl Pointer to the editor control widget.
        void addProperty(gui::Label* pLabel, gui::Control* pControl);

        /// @brief Populates the parent view with all contained controls.
        /// @param pView Parent view to receive the controls.
        /// @param cell Cell descriptor within the parent view.
        /// @param szMargins Margin sizes to apply around the layout.
        void populateView(View* pView, const Cell& cell, const Size& szMargins) override;

        /// @brief Sets the horizontal alignment applied to all label cells.
        /// @param hAlign Desired horizontal alignment.
        void setLabelAlignment(td::HAlignment hAlign);

        /// @brief Returns the current computed minimum height of the layout.
        /// @return Minimum height in pixels.
        td::UINT2 getCurrMinHeight() const;

        /// @brief Returns the zero-based position of a control within the layout.
        /// @param pControl Pointer to the control to locate.
        /// @return Position index, or -1 if the control is not found.
        int getControlPosition(const Control* pControl) const;

        /// @brief Returns the control at the specified position.
        /// @param pos Zero-based position index.
        /// @return Pointer to the control, or nullptr if the index is out of range.
        Control* getControlAtPosition(int pos);
    };
}
