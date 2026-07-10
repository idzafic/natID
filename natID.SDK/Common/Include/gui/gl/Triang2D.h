// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Triang2D.h
    @brief 2D polygon triangulation utilities for converting contours into renderable triangle meshes. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/natGL.h>
#include <td/Types.h>
#include <gui/gl/GL.h>
#include <cnt/PushBackVector.h>
#include <tuple>

namespace gui
{
namespace gl
{

class Buffer;
class Command;

/// @brief Performs 2D polygon triangulation, converting vertex contours into triangle primitives
///        suitable for OpenGL rendering.
class NATGL_API Triang2D
{
    void* _triangulator = nullptr; ///< Opaque pointer to the internal triangulation engine.

    Triang2D(const Triang2D&) = delete;
    Triang2D& operator =(const Triang2D&) = delete;
    Triang2D() = delete;
public:
//    enum class Type {Fan=0, Strip, Triangles, Error};
    /// @brief Constructs the triangulator with a pre-allocated vertex capacity.
    /// @param nMaxVertices Maximum number of input vertices the triangulator can handle.
    Triang2D(td::UINT4 nMaxVertices);
    /// @brief Destructor. Frees the internal triangulation engine.
    ~Triang2D();

    /// @brief Triangulates a polygon defined by a raw vertex array.
    /// @param verticesIn Pointer to the input vertex array.
    /// @param nVerticesIn Number of input vertices.
    /// @param verticesOut Output vector that receives the triangulated vertices.
    /// @param indicesOut Output vector that receives the triangle index list.
    /// @return A tuple containing the resulting primitive type and the number of output indices.
    std::tuple<gui::gl::Primitive, td::UINT4> triangulate(const gui::gl::Point2d* verticesIn, td::UINT4 nVerticesIn, cnt::PushBackVector<gui::gl::Point2d>& verticesOut, cnt::PushBackVector<td::UINT4>& indicesOut);

    /// @brief Triangulates a polygon defined by a vector of vertices.
    /// @param verticesIn Input vector of polygon vertices.
    /// @param verticesOut Output vector that receives the triangulated vertices.
    /// @param indicesOut Output vector that receives the triangle index list.
    /// @return A tuple containing the resulting primitive type and the number of output indices.
    std::tuple<gui::gl::Primitive, td::UINT4> triangulate(const cnt::PushBackVector<gui::gl::Point2d>& verticesIn, cnt::PushBackVector<gui::gl::Point2d>& verticesOut, cnt::PushBackVector<td::UINT4>& indicesOut);

    /// @brief Triangulates a polygon and stores the result directly into a GPU buffer.
    /// @param pBuffer Target GL buffer to receive the triangulated geometry.
    /// @param verticesIn Pointer to the input vertex array.
    /// @param nVerticesIn Number of input vertices.
    /// @return Pointer to a Command object describing the resulting draw call, or nullptr on failure.
    gui::gl::Command* triangulate(gui::gl::Buffer* pBuffer, const gui::gl::Point2d* verticesIn, td::UINT4 nVerticesIn);

    /// @brief Triangulates a polygon from a vector and stores the result into a GPU buffer.
    /// @param pBuffer Target GL buffer to receive the triangulated geometry.
    /// @param verticesIn Input vector of polygon vertices.
    /// @return Pointer to a Command object describing the resulting draw call, or nullptr on failure.
    gui::gl::Command* triangulate(gui::gl::Buffer* pBuffer, const cnt::PushBackVector<gui::gl::Point2d>& verticesIn);

    //used to find indices over previously triangulated polygon (used to draw polygon border)
    /// @brief Creates a line command by recycling index data from a previously triangulated polygon, useful for drawing the polygon border.
    /// @param pBuffer Target GL buffer for the line geometry.
    /// @param primitive The primitive type of the previously triangulated polygon.
    /// @param verticesToSearch Pointer to the array of vertices to search within.
    /// @param startingVertex Index of the first vertex in the search range.
    /// @param nVerticesToSearch Number of vertices to search.
    /// @return Pointer to a Command object for the border draw call, or nullptr on failure.
    gui::gl::Command* createRecycledLine(gui::gl::Buffer* pBuffer, gui::gl::Primitive, const gui::gl::Point2d* verticesToSearch, td::UINT4 startingVertex, td::UINT4 nVerticesToSearch);

    //returns true if ok and number of inserted vertices and indices
    /// @brief Triangulates a polyline with a given width into a filled strip, optionally closing it.
    /// @param verticesIn Pointer to the centerline vertex array.
    /// @param nVerticesIn Number of centerline vertices.
    /// @param width Width of the generated line strip.
    /// @param bClosed True to close the polyline into a loop.
    /// @param verticesOut Output vector receiving the generated strip vertices.
    /// @param indicesOut Output vector receiving the generated index list.
    /// @return A tuple of the primitive type, the number of output vertices, and the number of output indices.
    static std::tuple<gui::gl::Primitive, td::UINT4, td::UINT4> triangulateLine(const gui::gl::Point2d* verticesIn, td::UINT4 nVerticesIn, float width, bool bClosed, cnt::PushBackVector<gui::gl::Point2d>& verticesOut, cnt::PushBackVector<td::UINT4>& indicesOut);

    /// @brief Triangulates a polyline into a GPU buffer with the given width, optionally closing it.
    /// @param pBuffer Target GL buffer for the generated strip geometry.
    /// @param verticesIn Pointer to the centerline vertex array.
    /// @param nVerticesIn Number of centerline vertices.
    /// @param width Width of the generated line strip.
    /// @param bClosed True to close the polyline into a loop.
    /// @return Pointer to a Command object for the strip draw call, or nullptr on failure.
    gui::gl::Command* triangulateLine(gui::gl::Buffer* pBuffer, const gui::gl::Point2d* verticesIn, td::UINT4 nVerticesIn, float width, bool bClosed);
};

//Generates triangulated symmetric polygons in form of TRIANGL_FAN structure
/// @brief Generates a triangulated symmetric polygon (fan structure) centered on a circle.
/// @param c The circle defining the center and radius of the polygon.
/// @param nNodes Number of nodes (sides) of the polygon.
/// @param vertices Output vector receiving the generated fan vertices.
/// @param firstPtAngle Starting angle in radians for the first vertex (default 0).
/// @param pTextureCoords Optional output vector for corresponding texture coordinates.
NATGL_API void generateSymPolygon(const gui::gl::Circle& c, size_t nNodes, cnt::PushBackVector<gui::gl::Point2d>& vertices, float firstPtAngle = 0, cnt::PushBackVector<gui::gl::Point2d>* pTextureCoords = nullptr);

} //namespace gl

} //namespace gui
