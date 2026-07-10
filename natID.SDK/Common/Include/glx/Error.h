// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
/** @file Error.h
 * @brief Encapsulates an error code, domain, and human-readable descriptions from GPU operations.
 */
#pragma once
#include <glx/natGLX.h>
#include <glx/Types.h>
#include <td/String.h>

//namespace MTL
//{
//class Error;
//} //namespace MTL

namespace glx
{
/// @brief Holds error information returned by GPU API calls, including code, domain, and descriptions.
class NATGLX_API Error
{
    td::LINT8 _code = 0;                          ///< Numeric error code.
    td::String _domain;                           ///< Domain string identifying the error source.
    td::String _localizedDescription;             ///< Human-readable description of the error.
    td::String _localizedRecoverySuggestion;      ///< Suggested action to recover from the error.
    td::String _localizedFailureReason;           ///< Localized reason explaining why the operation failed.
private:
//    MTL::Error* _pError;
protected:
//    Error(MTL::Error* pError);
public:
    /// @brief Default constructor; initializes an empty error with code zero.
    Error();

    /// @brief Assignment operator.
    /// @param error The error to assign from.
    void operator = (const Error& error);

    /// @brief Returns the numeric error code.
    /// @return The error code.
    td::LINT8 code() const;

    /// @brief Returns the domain string identifying the source of this error.
    /// @return The domain string.
    td::String domain() const;

    //class Dictionary* userInfo() const;

    /// @brief Returns a localized, human-readable description of the error.
    /// @return The localized description string.
    td::String localizedDescription() const;

//    class Array*      localizedRecoveryOptions() const;

    /// @brief Returns a localized suggestion for recovering from the error.
    /// @return The recovery suggestion string.
    td::String localizedRecoverySuggestion() const;

    /// @brief Returns a localized string explaining why the failure occurred.
    /// @return The failure reason string.
    td::String localizedFailureReason() const;

//    void release();
};

} //namespace glx


