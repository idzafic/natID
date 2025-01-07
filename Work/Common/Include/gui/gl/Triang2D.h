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

class NATGL_API Triang2D
{
    void* _triangulator = nullptr;
    
public:
//    enum class Type {Fan=0, Strip, Triangles, Error};
    Triang2D(td::UINT4 nMaxVertices);
    ~Triang2D();
    
    std::tuple<gui::gl::Primitive, td::UINT4> triangulate(const gui::gl::Point2d* verticesIn, td::UINT4 nVerticesIn, cnt::PushBackVector<gui::gl::Point2d>& verticesOut, cnt::PushBackVector<td::UINT4>& indicesOut);
    
    std::tuple<gui::gl::Primitive, td::UINT4> triangulate(const cnt::PushBackVector<gui::gl::Point2d>& verticesIn, cnt::PushBackVector<gui::gl::Point2d>& verticesOut, cnt::PushBackVector<td::UINT4>& indicesOut);
    
    gui::gl::Command* triangulate(gui::gl::Buffer* pBuffer, const gui::gl::Point2d* verticesIn, td::UINT4 nVerticesIn);
    
    gui::gl::Command* triangulate(gui::gl::Buffer* pBuffer, const cnt::PushBackVector<gui::gl::Point2d>& verticesIn);
    
    //used to find indices over previously triangulated polygon (used to draw polygon border)
    gui::gl::Command* createRecycledLine(gui::gl::Buffer* pBuffer, gui::gl::Primitive, const gui::gl::Point2d* verticesToSearch, td::UINT4 startingVertex, td::UINT4 nVerticesToSearch);
    
    //returns true if ok and number of inserted vertices and indices
    static std::tuple<gui::gl::Primitive, td::UINT4, td::UINT4> triangulateLine(const gui::gl::Point2d* verticesIn, td::UINT4 nVerticesIn, float width, bool bClosed, cnt::PushBackVector<gui::gl::Point2d>& verticesOut, cnt::PushBackVector<td::UINT4>& indicesOut);
    
    gui::gl::Command* triangulateLine(gui::gl::Buffer* pBuffer, const gui::gl::Point2d* verticesIn, td::UINT4 nVerticesIn, float width, bool bClosed);
};

//Generates triangulated symmetric polygons in form of TRIANGL_FAN structure
NATGL_API void generateSymPolygon(const gui::gl::Circle& c, size_t nNodes, cnt::PushBackVector<gui::gl::Point2d>& vertices, float firstPtAngle = 0, cnt::PushBackVector<gui::gl::Point2d>* pTextureCoords = nullptr);

} //namespace gl

} //namespace gui

