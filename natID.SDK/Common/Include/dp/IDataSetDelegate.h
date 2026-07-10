// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IDataSetDelegate.h @brief Interface for receiving callbacks during IDataSet loading and consumption. */
#pragma once
#include <dp/IDataSet.h>

namespace dp {
/// @brief Abstract delegate interface for customizing IDataSet row processing.
class IDataSetDelegate
{
public:
    /// @brief Called once when the data set is initialized.
    /// @param pDS Pointer to the data set being initialized.
    virtual void init(dp::IDataSet* pDS)
    {
    }
    /// @brief Called for each row when the data set is loading data.
    /// @param pDS Pointer to the data set.
    /// @param row Reference to the current row wrapper.
    /// @param iRow Zero-based index of the current row.
    /// @return true to continue loading, false to abort.
    virtual bool onLoad(dp::IDataSet* pDS, dp::RowWrapper& row, size_t iRow)
    {
        return false;
    }
    /// @brief Called for each row when the data set is consuming (processing) data.
    /// @param pDS Pointer to the data set.
    /// @param row Reference to the current row wrapper.
    /// @param iRow Zero-based index of the current row.
    /// @return true to continue consuming, false to abort.
    virtual bool onConsume(dp::IDataSet* pDS, dp::RowWrapper& row, size_t iRow)
    {
        return false;
    }
};
} //namespace dp
