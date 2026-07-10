// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ICameraLimiter.h
 *  @brief Interface for constraining camera movement in a 3-D OpenGL scene. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/natGL.h>
#include <gui/gl/GL.h>

namespace gui
{
namespace gl
{

/// @brief Abstract interface that constrains camera movement to enforce scene-specific limits.
class ICameraLimiter
{
public:
    /// @brief Result of a camera movement evaluation.
    enum class Move : td::BYTE {No=0,     ///< The desired move is completely rejected; camera stays at initialPos.
                                Limited,  ///< The move is partially allowed; finalePosition contains the clamped result.
                                Desired   ///< The desired position is accepted without modification.
                               };

    /// @brief Evaluates a requested camera move and optionally clamps it.
    /// @param initialPos The camera position before the move.
    /// @param desiredPosition The position the camera wants to reach.
    /// @param finalePosition Output: the position the camera should actually reach.
    /// @return A Move value indicating whether the move is allowed, limited, or rejected.
    virtual Move makeMove(const glm::vec3& initialPos, const glm::vec3& desiredPosition, glm::vec3& finalePosition) = 0;
};

} //namespace gl
} //namespace gui
