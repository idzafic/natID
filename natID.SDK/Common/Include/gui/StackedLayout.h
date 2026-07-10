// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file StackedLayout.h
 * @brief Provides the StackedLayout class for stacking controls along one axis.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Layout.h"
#include <cnt/PushBackVector.h>

namespace gui
{
	/// @brief A layout that arranges controls in a sequential stack (base class for horizontal and vertical layouts).
	class NATGUI_API StackedLayout : public Layout
	{
    protected:
        cnt::PushBackVector<Cell> _cells; ///< Dynamic list of layout cells.
        td::BYTE _spaceBetweenCells; ///< Spacing between adjacent cells in pixels.
        td::BYTE _marginX; ///< Horizontal outer margin in pixels.
        td::BYTE _marginY; ///< Vertical outer margin in pixels.
    protected:
        /// @brief Returns the number of currently visible cells.
        /// @return Count of visible cells.
        size_t getNoOfVisibleCells() const;

        /// @brief Performs the initial measurement pass.
        /// @return True if measurement succeeded.
        bool initialMeasure() override;

        /// @brief Measures the initial size of a single cell.
        /// @param cell Cell info to populate.
        void initialMeasure(CellInfo&) override;

        /// @brief Measures the preferred size of a cell.
        /// @param cell Cell info to populate.
        void measure(CellInfo& cell) override;

        /// @brief Re-measures a cell after layout changes.
        /// @param cell Cell info to update.
        void reMeasure(CellInfo& cell) override;

        /// @brief Updates a specific cell's information.
        /// @param cell Cell info to update.
        /// @param ci Reference cell info.
        /// @param iPos Position index.
        virtual void updateCell(CellInfo& cell, const CellInfo& ci, int iPos) const;

        /// @brief Freezes the layout to prevent updates.
        /// @return True if freezing succeeded.
        bool freeze() override;

        /// @brief Unfreezes the layout to allow updates.
        void unFreeze() override;

        /// @brief Handles an action item event.
        /// @param aiDesc Action item descriptor.
        /// @return True if handled.
        bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;

        /// @brief Returns the horizontal margin.
        /// @return Margin in pixels.
        td::BYTE getXMargin() const;

        /// @brief Returns the vertical margin.
        /// @return Margin in pixels.
        td::BYTE getYMargin() const;
        //virtual Size getMinSize(const Cell& cell);

        StackedLayout() = delete;
        StackedLayout(const StackedLayout&) = delete;
        StackedLayout& operator =(const StackedLayout&) = delete;
    public:
        /// @brief Constructs a StackedLayout with an initial capacity.
        /// @param nElems Initial number of elements to reserve.
        StackedLayout(size_t nElems);

        /// @brief Sets the outer margins of the layout.
        /// @param marginX Horizontal margin.
        /// @param marginY Vertical margin.
        void setMargins(td::BYTE marginX, td::BYTE marginY);

        /// @brief Sets the spacing between cells.
        /// @param spaceBetweenCells Spacing in pixels.
        void setSpaceBetweenCells(td::BYTE spaceBetweenCells);

        /// @brief Appends a control to the layout.
        /// @param ctrl Control to append.
        /// @param hAlign Horizontal alignment.
        /// @param vAlign Vertical alignment.
        void append(Control& ctrl, td::HAlignment hAlign = td::HAlignment::Center, td::VAlignment vAlign = td::VAlignment::Center);

        /// @brief Appends a control using the stream operator.
        /// @param ctrl Control to append.
        /// @return Reference to this layout.
        StackedLayout& operator << (Control& ctrl);

        /// @brief Appends a nested layout.
        /// @param layout Layout to append.
        void appendLayout(Layout& layout);

        /// @brief Appends a nested layout using the stream operator.
        /// @param layout Layout to append.
        /// @return Reference to this layout.
        StackedLayout& operator << (Layout& layout);

        /// @brief Populates the parent view with the layout cells.
        /// @param pView Target view.
        /// @param cell Cell descriptor.
        /// @param szMargins Margin sizes.
        void populateView(View* pView, const Cell& cell, const Size& szMargins) override;

        /// @brief Returns the control at the given cell position.
        /// @param cellPosition Zero-based cell index.
        /// @return Pointer to the control, or nullptr.
        const Control* getControl(size_t cellPosition) const;

        /// @brief Returns the total number of cells in this layout.
        /// @return Cell count.
        size_t getNumberOfCells() const;
	};
}
