// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Cell.h
 @brief Represents a GUI layout cell that holds a control and its geometry information. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "CellInfo.h"


namespace gui
{

class Control;

/// @brief Extends CellInfo to associate a GUI control with a cell and provide geometry adjustment utilities.
class NATGUI_API Cell : public CellInfo
{
public:
    Control*  pCtrl; ///< Pointer to the GUI control occupying this cell.
protected:

public:
    /// @brief Resets the cell to its default state, clearing control pointer and layout info.
    void clean();
    /// @brief Assigns layout information from a CellInfo object to this cell.
    /// @param ci The CellInfo to copy layout properties from.
    void operator = (const CellInfo& ci);
    /// @brief Adjusts the vertical geometry of the associated control to fit within the cell.
    /// @param ctrlGeometry The geometry structure to be adjusted vertically.
    void adjustCtrlVerticalGeometry(Geometry& ctrlGeometry) const;
    /// @brief Adjusts the horizontal geometry of the associated control to fit within the cell.
    /// @param ctrlGeometry The geometry structure to be adjusted horizontally.
    void adjustCtrlHorizontalGeometry(Geometry& ctrlGeometry) const;
    /// @brief Adjusts both horizontal and vertical geometry of the associated control to fit within the cell.
    /// @param ctrlGeometry The geometry structure to be fully adjusted.
    void adjustCtrlGeometry(Geometry& ctrlGeometry) const;
};

}  //namespace gui
