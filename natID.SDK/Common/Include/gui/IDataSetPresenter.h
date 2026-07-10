// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IDataSetPresenter.h
    @brief Interface for objects that can present and detach a data set. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once

#include <td/Types.h>
#include <dp/IDataSet.h>

namespace gui
{

/// @brief Abstract interface for UI components that display data from a dp::IDataSet.
class IDataSetPresenter
{
public:
    /// @brief Displays the given data set in the implementing presenter.
    /// @param pDS Pointer to the data set to display. May be null to clear the view.
    virtual void show(dp::IDataSet* pDS) = 0;

    /// @brief Clears the presenter and releases its reference to the current data set.
    virtual void cleanAndDetachDataSet() = 0;
};

} //namespace gui
