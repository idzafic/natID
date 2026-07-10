// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file HorizontalLayout.h
    @brief Horizontal stacked layout that arranges controls side by side in a single row. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "StackedLayout.h"
#include <cnt/PushBackVector.h>

namespace gui
{
	/// @brief Layout that arranges child controls horizontally, left to right, in a single row.
	class NATGUI_API HorizontalLayout : public StackedLayout
	{
    protected:
        /// @brief Updates an individual cell's layout information during a layout pass.
        /// @param cell The cell info to update.
        /// @param ci Reference cell info to copy from.
        /// @param iPos Position index of the cell within the horizontal sequence.
        void updateCell(CellInfo& cell, const CellInfo& ci, int iPos) const override;

        /// @brief Applies the final geometry to the horizontal layout.
        /// @param cellFrame The available geometry rectangle.
        /// @param cell The cell within which the layout resides.
        void setGeometry(const Geometry& cellFrame, const Cell& cell) override;

        HorizontalLayout() = delete;
        HorizontalLayout(const HorizontalLayout&) = delete;
        HorizontalLayout& operator =(const HorizontalLayout&) = delete;
    public:
        /// @brief Constructs a HorizontalLayout with a pre-allocated number of elements.
        /// @param nElems Expected number of child controls.
        HorizontalLayout(size_t nElems);

        /// @brief Returns the object type identifier for this layout.
        /// @return ObjType::VerticalLayout (used for horizontal layout internally).
        virtual gui::ObjType getObjType() const override { return ObjType::VerticalLayout;};

        /// @brief Appends a fixed-size space between controls.
        /// @param spaceSize Size of the space in pixels.
        /// @return Reference to this layout for chaining.
        HorizontalLayout& appendSpace(td::BYTE spaceSize);

        /// @brief Appends a flexible spacer that expands to fill available space.
        /// @param weight Relative weight of the spacer for space distribution.
        /// @return Reference to this layout for chaining.
        HorizontalLayout& appendSpacer(td::BYTE weight=1);
	};
}
