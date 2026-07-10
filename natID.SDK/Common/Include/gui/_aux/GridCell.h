// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file GridCell.h
 * @brief Extends Cell with original sizing and span fields used for grid layout serialization.
 */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/_aux/Cell.h>
#include <fstream>

namespace gui
{

/// @brief A Cell that additionally stores the original (pre-resize) sizing and span values for grid layout persistence.
class NATGUI_API GridCell : public Cell
{
public:
    td::UINT2 minHorOrig;   ///< Original minimum horizontal size before any runtime resize.
    td::UINT2 minVerOrig;   ///< Original minimum vertical size before any runtime resize.
    td::BYTE nResHorOrig;   ///< Original horizontal resize unit count before any runtime resize.
    td::BYTE nResVerOrig;   ///< Original vertical resize unit count before any runtime resize.
    td::BYTE rowSpan;       ///< Number of rows this cell spans.
    td::BYTE colSpan;       ///< Number of columns this cell spans.
protected:

public:
    /// @brief Resets the grid cell to its default state, clearing all fields.
    void clean();
    /// @brief Serializes the grid cell layout data to an output file stream.
    /// @param f The output file stream to write to.
    void serialize(std::ofstream& f) const;
};

}  //namespace gui
