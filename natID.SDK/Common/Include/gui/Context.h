// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Context.h
    @brief Declares the Context RAII helper that saves and restores the graphics transformation context. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Transformation.h>

namespace gui
{

/// @brief RAII guard that saves the current graphics transformation context on construction
///        and restores it on destruction.
///
/// Wrap drawing code in a Context object to ensure that any transformation changes are
/// automatically rolled back when the scope exits, regardless of how it exits.
class Context
{
    Context(const Context&) = delete;
    Context& operator =(const Context&) = delete;
public:
    /// @brief Saves the current transformation context.
    Context()
    {
        gui::Transformation::saveContext();
    }

    /// @brief Restores the previously saved transformation context.
    ~Context()
    {
        gui::Transformation::restoreContext();
    }
};

} //namespace gui
