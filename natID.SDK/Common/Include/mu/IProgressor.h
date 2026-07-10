// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IProgressor.h
 *  @brief Interface for reporting progress status during long-running operations.
 */
#pragma once
#include <td/Types.h>

namespace mu
{

/// @brief Abstract interface for progress reporting; implementors update a progress indicator.
class IProgressor
{
public:
    /// @brief Updates the progress indicator at the given location.
    /// @param val Normalised progress value (typically 0.0 to 1.0 or an absolute count).
    /// @param location Identifier for the progress bar or indicator to update.
    virtual void updateStatus(double val, td::BYTE location) = 0;
};

}


