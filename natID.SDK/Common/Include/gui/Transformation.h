// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Transformation.h
    @brief 2D affine transformation (rotate, translate, scale) applied to the graphics context. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "NatObject.h"

namespace gui
{

/// @brief Represents a 2D affine transformation that can be composed and applied to the current graphics context.
class NATGUI_API Transformation : public NatObject
{
protected:
    Transformation(const Transformation&) = delete;
    Transformation& operator = (const Transformation&) = delete;
public:
    /// @brief Default constructor that initialises the transformation to identity.
    Transformation();
    /// @brief Move constructor.
    /// @param tr Source transformation to move from.
    Transformation(Transformation&& tr);
    /// @brief Move assignment operator.
    /// @param other Source transformation to move from.
    /// @return Reference to this transformation after assignment.
    Transformation& operator = (Transformation&& other);

    /// @brief Virtual destructor.
    virtual ~Transformation();

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::Transformation.
    gui::ObjType getObjType() const override { return ObjType::Transformation; }

    //add rotation
    /// @brief Appends a rotation by the given angle in degrees.
    /// @param angleDeg Rotation angle in degrees (positive = counter-clockwise).
    void rotateDeg(double angleDeg);
    /// @brief Appends a rotation by the given angle in radians.
    /// @param angleRad Rotation angle in radians (positive = counter-clockwise).
    void rotateRad(double angleRad);
    //add translation
    /// @brief Appends a translation by the given delta values.
    /// @param dx Horizontal displacement.
    /// @param dy Vertical displacement.
    void translate(CoordType dx, CoordType dy);
    /// @brief Appends a translation by the given point.
    /// @param pt Point whose x and y values define the displacement.
    void translate(const Point& pt);
    //add scaling
    /// @brief Appends a uniform scale (same factor on both axes).
    /// @param sxAndsy Scale factor applied to both x and y axes.
    void scale(CoordType sxAndsy);
    /// @brief Appends a non-uniform scale.
    /// @param sx Scale factor for the x axis.
    /// @param sy Scale factor for the y axis.
    void scale(CoordType sx, CoordType sy);
    //set to identity
    /// @brief Resets this transformation to the identity matrix.
    void identity();
    //put as last in transformation chai
    /// @brief Concatenates this transformation after the current context transformation matrix.
    void appendToContext() const;
    //set as current transfomatoin matrix -> previous transformations will not be included
    /// @brief Replaces the current context transformation matrix with this transformation, discarding any previous transforms.
    void setToContext() const;

    //save current transfomation (graphics) context
    //could be called multiple times
    /// @brief Saves the current graphics context state (including the transformation matrix) onto a stack.
    static void saveContext();

    //restore context previously saved with saveContext
    //paired with saveContext
    /// @brief Restores the graphics context state previously saved by saveContext().
    static void restoreContext();

    /// @brief Sets the clipping rectangle in the current graphics context.
    /// @param clippingRect Rectangle that defines the clipping region.
    static void setClip(const gui::Rect& clippingRect);
};

}
