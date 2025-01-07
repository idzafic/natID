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
#include <gui/gl/Types.h>
#include <cnt/PushBackVector.h>
#include <cnt/SafeFullVector.h>
#include <gui/gl/CommandList.h>

namespace gui
{

namespace gl
{

class Program;
class Triang2D;

class NATGL_API Buffer
{
    friend class Program;
    friend class Triang2D;
    
//public:
//    static constexpr size_t MaxNoOfLayouts = 8;
protected:
    
private:
    gui::gl::CommandList _commandList;
    cnt::PushBackVector<VertexType> _vertexData;
    cnt::PushBackVector<VertexType> _instanceData;
    cnt::PushBackVector<IndexType> _indexData;
    cnt::SafeFullVector<gui::gl::DataType> _vertexPositions;
    cnt::SafeFullVector<gui::gl::DataType> _instancePositions;
    cnt::SafeFullVector<td::UINT2> _instanceDivisors;
    td::UINT4 _vao = 0;
    td::UINT4 _vbo = 0;
    td::UINT4 _ibo = 0;
    td::UINT4 _ebo = 0;
    td::UINT2 _vertexRecLenInBytes = 0;
    td::UINT2 _instanceRecLenInBytes = 0;
    td::BYTE _nVertexFloats = 0;
    td::BYTE _nInstanceFloats = 0;
    td::BYTE _committed = 0; //0-not commited, 1-commit done, 2-needs reset (recreation)
private:
    bool isAddOnCommitted() const;
protected:
    const cnt::PushBackVector<VertexType>& getVertices() const;
public:
    Buffer();
    ~Buffer();
    
    //add and initialize buffer layout, space for tmp buffers, and space for commands of the layout
    bool init(size_t nCommandsToReserve, size_t nVerticesToReserve, size_t nIndicesToReserve, const std::initializer_list<DataType>& vboLayout);
    bool init(size_t nCommandsToReserve, size_t nVerticesToReserve, size_t nIndicesToReserve, size_t nInstanceEntries, const std::initializer_list<DataType>& vboLayout, const std::initializer_list<DataType>& instLayout, const std::initializer_list<td::UINT2>& instDivisors = {});
    
    //append data to current layoutID
    void appendVertexUnits(const gui::gl::VertexType* pVertexData, size_t nVertexFloatUnits);
    void appendVertices(const gui::gl::VertexType* pVertexData, size_t nVertices);
    void appendInstances(const gui::gl::VertexType* pInstanceData, size_t nInstances);
    
    void appendIndices(const gui::gl::IndexType* pIndices, size_t nIndices);
    void appendVertices(const cnt::PushBackVector<gui::gl::Point2d>& points); //must be only 1 position in current layout (vec2)
    void appendVertices(const gui::gl::Point2d* points, size_t nPoints); //must be only 1 position in current layout (vec2)
    void appendIndices(const cnt::PushBackVector<gui::gl::IndexType>& indices);
    
    void appendVertex(const gui::gl::Point2d& pt);
    void appendIndex(gui::gl::IndexType index);
    void updateVertex(size_t pos, const gui::gl::Point2d& pt);
    void getVertex(size_t pos,gui::gl::Point2d& pt) const;
    
    void insertVertex(size_t pos, const gui::gl::Point2d& pt);
    
    gui::gl::Command* addCube(float sideLength, gui::gl::Texture::Type textureType = gui::gl::Texture::Type::None, bool generateNormals = false);

    gui::gl::Command* addSphere(float r, td::UINT4 nSectors, td::UINT4 nStacks, gui::gl::Texture::Type textureType = gui::gl::Texture::Type::None, bool generateNormals = false, float xyAsym = 1.0f);
    
    //get vertex index position from current layout ID
    td::UINT4 getNoOfVertices() const;
    size_t getNoOfIndices() const;
    size_t getNoOfInstances() const;

    //activate using current layout
    void activate() const;
    void deActivate() const;
//    void setCurrentLayoutID(td::INT1 layoutID);
//    td::INT1 getCurrentLayoutID() const;
    
    td::UINT2 getVertexStride() const;
    td::UINT2 getInstanceStride() const;
    
    gui::gl::CommandList* getCommandList();
    
    //reserve number of commands
    void reserveCommands(size_t nCommands);
    //places command on the list and links function pointers
    bool encode(gui::gl::Command* pComm);
    //removes pComm from list if it is last in the list
    bool remove(gui::gl::Command* pComm);
    //creates empy command
    gui::gl::Command* createCommand();
    //transfer data to GPU
    bool commit();
    
    //release from GPU and reset internal pointers
    void reset();
    //release all memory allocated by the Buffer
    void release();
};


} //namespace gl

} //namespace gui

