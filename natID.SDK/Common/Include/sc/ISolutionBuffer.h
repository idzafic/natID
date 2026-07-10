// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ISolutionBuffer.h
 *  @brief Interface for consuming intermediate and final solver solution data.
 */
#pragma once

namespace sc
{

/// @brief Abstract interface for a buffer that receives solver output at each reporting step.
class ISolutionBuffer
{
public:
    /// @brief Outcome status reported when the solver finalises a run.
    enum class Status {Error, UserStop, SuccessfullyCompleted, BatchCompleted};

    /// @brief Receive and store the current solution snapshot.
    virtual void put() = 0;
    /// @brief Signal the end of the solution process with the final status.
    /// @param status The completion status of the solver run.
    virtual void finalize(Status status) = 0;
};

} //namspace sc
