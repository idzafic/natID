// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SplitterLayout.h
 * @brief Provides the SplitterLayout class for splitting a view area into two resizable panes.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Layout.h>
#include <gui/_aux/SplitterCell.h>
#include <gui/ISwitch.h>

namespace gui
{
	/// @brief A layout that divides its area into two panes separated by a draggable splitter.
	class NATGUI_API SplitterLayout : public Layout, public ISwitch
	{
        friend class SplitterCell;
    public:
        /// @brief Orientation of the splitter divider.
        enum class Orientation : td::BYTE {Horizontal=0, Vertical};
        /// @brief Identifies which cell is the auxiliary (collapsible) one.
        enum class AuxiliaryCell : td::BYTE {First=0, Second};
        /// @brief Position of the minimized auxiliary cell.
        enum class MinimizedCellPosition : td::BYTE {TopOrLeft, Center, BottomOrDown};
    protected:
        SplitterCell _splitterCell; ///< The draggable splitter handle cell.
        Geometry _lastGeometry; ///< Last recorded geometry for change detection.
        Cell _cells[3]; ///< The three layout cells: first pane, splitter, second pane.
        CoordType _sizeOfAuxiliaryCell; ///< Current size of the auxiliary cell.
        CoordType _lastVisibleAuxiliarySize; ///< Last visible size of the auxiliary cell.
        Orientation _orientation; ///< Layout orientation.
        AuxiliaryCell _posOfAuxiliaryCell; ///< Position of the auxiliary cell.
        td::BYTE _spaceOfSplitterCell; ///< Size of the splitter handle in pixels.
    private:
        /// @brief Restores the auxiliary view to its last visible size.
        /// @param viewGeometry Current view geometry.
        void unhideAuxliaryView(const Geometry& viewGeometry);
    protected:
        /// @brief Returns the maximum allowed size for the auxiliary cell.
        /// @return Maximum size in coordinate units.
        gui::CoordType getMaxSizeForAuxiliaryCell() const;

        /// @brief Returns the minimum size for the auxiliary cell.
        /// @param sz Output size structure.
        void getMinSizeForAuxiliaryCell(Size& sz) const;

        /// @brief Returns the number of currently visible cells.
        /// @return Count of visible cells.
        size_t getNoOfVisibleCells() const;

        /// @brief Performs the initial size measurement pass.
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

        /// @brief Freezes the layout to prevent updates.
        /// @return True if freezing succeeded.
        bool freeze() override;

        /// @brief Unfreezes the layout to allow updates.
        void unFreeze() override;

        /// @brief Handles an action item event.
        /// @param aiDesc Action item descriptor.
        /// @return True if the event was handled.
        bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;

        /// @brief Sets the geometry of a cell within the layout frame.
        /// @param cellFrame Frame geometry.
        /// @param cell Target cell.
        void setGeometry(const Geometry& cellFrame, const Cell& cell) override;

        /// @brief Returns the horizontal margin.
        /// @return Margin in pixels.
        td::BYTE getXMargin() const;

        /// @brief Returns the vertical margin.
        /// @return Margin in pixels.
        td::BYTE getYMargin() const;

        /// @brief Returns the current splitter orientation.
        /// @return Orientation enum value.
        Orientation getOrientation() const;

        /// @brief Updates cell geometry based on the new view geometry.
        /// @param viewGeometry New view geometry.
        void updateGeometry(const Geometry& viewGeometry);

        /// @brief Called when the user drags the splitter cell.
        void onSplitterCellMove();
        //ISwitch
        /// @brief Toggles the auxiliary cell between visible and hidden.
        /// @return New switch state.
        ISwitch::State toggle() override;

        /// @brief Sets the auxiliary cell state directly.
        /// @param state Desired state.
        void setState(ISwitch::State state) override;

        /// @brief Sets the updater interface for switch state notifications.
        /// @param pUpdater Pointer to the updater.
        void setUpdater(ISwitchState* pUpdater) override;

        SplitterLayout() = delete;
        SplitterLayout(const SplitterLayout&) = delete;
        SplitterLayout& operator =(const SplitterLayout&) = delete;
    public:
        /// @brief Constructs a SplitterLayout with given orientation and auxiliary cell position.
        /// @param orientation Layout orientation.
        /// @param auxCellPos Which cell is auxiliary.
        SplitterLayout(SplitterLayout::Orientation orientation, SplitterLayout::AuxiliaryCell auxCellPos = SplitterLayout::AuxiliaryCell::Second);

        /// @brief Returns the GUI object type identifier.
        /// @return ObjType::SplitterLayout.
        gui::ObjType getObjType() const override { return ObjType::SplitterLayout;}

        /// @brief Sets the layout margins.
        /// @param marginX Horizontal margin.
        /// @param marginY Vertical margin.
        void setMargins(td::BYTE marginX, td::BYTE marginY);

        /// @brief Sets the space between cells (splitter handle size).
        /// @param spaceBetweenCells Space in pixels.
        void setSpaceBetweenCells(td::BYTE spaceBetweenCells);

        /// @brief Assigns two controls to the two panes.
        /// @param ctrl1 First pane control.
        /// @param ctrl2 Second pane control.
        void setContent(Control& ctrl1, Control& ctrl2);

        /// @brief Sets a control at a specific pane position.
        /// @param pos Pane index (0 or 1).
        /// @param ctrl Control to place.
        /// @param hAlign Horizontal alignment.
        /// @param vAlign Vertical alignment.
        void setView(size_t pos, Control& ctrl, td::HAlignment hAlign = td::HAlignment::Left, td::VAlignment vAlign = td::VAlignment::Top);

        /// @brief Sets a layout at a specific pane position.
        /// @param pos Pane index (0 or 1).
        /// @param layout Layout to place.
        void setLayout(size_t pos, Layout& layout);

        /// @brief Populates the view with layout cells and margins.
        /// @param pView Target view.
        /// @param cell Cell descriptor.
        /// @param szMargins Margin sizes.
        void populateView(View* pView, const Cell& cell, const Size& szMargins) override;

        /// @brief Sets the tooltip shown when the auxiliary cell is minimized.
        /// @param str Tooltip text.
        void setToolTipForMinimizedAux(const td::String& str);
	};
}
