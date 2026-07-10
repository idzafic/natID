// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file VerticalLayout.h
    @brief Stacked layout that arranges child views vertically from top to bottom. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "StackedLayout.h"
#include <cnt/PushBackVector.h>

namespace gui
{
	/// @brief Layout that stacks child views vertically, distributing available height among them.
	class NATGUI_API VerticalLayout : public StackedLayout
	{
    protected:
        /// @brief Updates a single cell descriptor during a layout pass.
        /// @param cell Output cell information to update.
        /// @param ci Reference cell information from the previous pass.
        /// @param iPos Zero-based position of this cell in the layout.
        void updateCell(CellInfo& cell, const CellInfo& ci, int iPos) const override;
        /// @brief Applies the given frame geometry to this layout and all its children.
        /// @param cellFrame Available bounding rectangle for the layout.
        /// @param cell Cell descriptor with alignment and sizing hints.
        void setGeometry(const Geometry& cellFrame, const Cell& cell) override;

        VerticalLayout() = delete;
        VerticalLayout(const VerticalLayout&) = delete;
        VerticalLayout& operator =(const VerticalLayout&) = delete;
    public:
        /// @brief Constructs a VerticalLayout pre-sized for the given number of elements.
        /// @param nElems Number of child views the layout will accommodate.
        VerticalLayout(size_t nElems);
        /// @brief Returns the GUI object type identifier.
        /// @return ObjType::VerticalLayout.
        gui::ObjType getObjType() const override { return ObjType::VerticalLayout;}
        /// @brief Appends a fixed-size vertical space after the last child.
        /// @param spaceSize Space size in pixels.
        /// @return Reference to this layout for method chaining.
        VerticalLayout& appendSpace(td::BYTE spaceSize);
        /// @brief Appends a flexible spacer that absorbs surplus vertical space.
        /// @param weight Relative weight used when distributing surplus space among multiple spacers (default 1).
        /// @return Reference to this layout for method chaining.
        VerticalLayout& appendSpacer(td::BYTE weight=1);
	};
}
