// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Buffer.h
    @brief Manages OpenGL vertex, index, and instance data buffers on the GPU. */
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

/// @brief Holds and manages GPU-side vertex, index, and instance buffer objects (VBO/IBO/EBO/VAO).
class NATGL_API Buffer
{
    friend class Program;
    friend class Triang2D;

//public:
//    static constexpr size_t MaxNoOfLayouts = 8;
protected:

private:
    gui::gl::CommandList _commandList; ///< List of draw commands associated with this buffer.
    cnt::PushBackVector<VertexType> _vertexData; ///< CPU-side vertex data storage.
    cnt::PushBackVector<VertexType> _instanceData; ///< CPU-side instance data storage.
    cnt::PushBackVector<IndexType> _indexData; ///< CPU-side index data storage.
    cnt::SafeFullVector<gui::gl::DataType> _vertexPositions; ///< Vertex attribute layout descriptors.
    cnt::SafeFullVector<gui::gl::DataType> _instancePositions; ///< Instance attribute layout descriptors.
    cnt::SafeFullVector<td::UINT2> _instanceDivisors; ///< Per-attribute instance divisors for instanced rendering.
    td::UINT4 _vao = 0; ///< OpenGL Vertex Array Object handle.
    td::UINT4 _vbo = 0; ///< OpenGL Vertex Buffer Object handle.
    td::UINT4 _ibo = 0; ///< OpenGL Instance Buffer Object handle.
    td::UINT4 _ebo = 0; ///< OpenGL Element (Index) Buffer Object handle.
    td::UINT2 _vertexRecLenInBytes = 0; ///< Size in bytes of a single vertex record.
    td::UINT2 _instanceRecLenInBytes = 0; ///< Size in bytes of a single instance record.
    td::BYTE _nVertexFloats = 0; ///< Number of float components per vertex.
    td::BYTE _nInstanceFloats = 0; ///< Number of float components per instance.
    td::BYTE _committed = 0; //0-not commited, 1-commit done, 2-needs reset (recreation)
private:
    /// @brief Returns true if an add-on commit has already been performed.
    bool isAddOnCommitted() const;
protected:
    /// @brief Returns a const reference to the internal vertex data vector.
    /// @return Const reference to the vertex data push-back vector.
    const cnt::PushBackVector<VertexType>& getVertices() const;
    /// @brief Deleted copy constructor – Buffer is non-copyable.
    Buffer(const Buffer&) = delete;
    /// @brief Deleted copy assignment – Buffer is non-copyable.
    Buffer& operator =(const Buffer&) = delete;
public:
    /// @brief Default constructor; initialises all handles to zero.
    Buffer();
    /// @brief Destructor; releases all GPU and CPU resources.
    ~Buffer();

    //add and initialize buffer layout, space for tmp buffers, and space for commands of the layout
    /// @brief Initialises the buffer with a vertex/index layout and pre-allocates CPU storage.
    /// @param nCommandsToReserve Number of draw commands to reserve.
    /// @param nVerticesToReserve Number of vertices to reserve.
    /// @param nIndicesToReserve Number of indices to reserve.
    /// @param vboLayout Attribute layout for the vertex buffer.
    /// @return True on success, false on failure.
    bool init(size_t nCommandsToReserve, size_t nVerticesToReserve, size_t nIndicesToReserve, const std::initializer_list<DataType>& vboLayout);
    /// @brief Initialises the buffer with separate vertex and instance layouts.
    /// @param nCommandsToReserve Number of draw commands to reserve.
    /// @param nVerticesToReserve Number of vertices to reserve.
    /// @param nIndicesToReserve Number of indices to reserve.
    /// @param nInstanceEntries Number of instance entries to reserve.
    /// @param vboLayout Attribute layout for the vertex buffer.
    /// @param instLayout Attribute layout for the instance buffer.
    /// @param instDivisors Per-attribute instance divisors (optional).
    /// @return True on success, false on failure.
    bool init(size_t nCommandsToReserve, size_t nVerticesToReserve, size_t nIndicesToReserve, size_t nInstanceEntries, const std::initializer_list<DataType>& vboLayout, const std::initializer_list<DataType>& instLayout, const std::initializer_list<td::UINT2>& instDivisors = {});

    //append data to current layoutID
    /// @brief Appends raw vertex float units to the vertex data buffer.
    /// @param pVertexData Pointer to the source float data.
    /// @param nVertexFloatUnits Number of individual float units to append.
    void appendVertexUnits(const gui::gl::VertexType* pVertexData, size_t nVertexFloatUnits);
    /// @brief Appends whole vertex records to the vertex data buffer.
    /// @param pVertexData Pointer to the source vertex data.
    /// @param nVertices Number of vertices to append.
    void appendVertices(const gui::gl::VertexType* pVertexData, size_t nVertices);
    /// @brief Appends instance records to the instance data buffer.
    /// @param pInstanceData Pointer to the source instance data.
    /// @param nInstances Number of instances to append.
    void appendInstances(const gui::gl::VertexType* pInstanceData, size_t nInstances);

    /// @brief Appends raw index data to the index buffer.
    /// @param pIndices Pointer to the source index array.
    /// @param nIndices Number of indices to append.
    void appendIndices(const gui::gl::IndexType* pIndices, size_t nIndices);
    /// @brief Appends 2D points as vertices; layout must contain exactly one vec2 position.
    /// @param points Vector of 2D points to append.
    void appendVertices(const cnt::PushBackVector<gui::gl::Point2d>& points); //must be only 1 position in current layout (vec2)
    /// @brief Appends an array of 2D points as vertices; layout must contain exactly one vec2 position.
    /// @param points Pointer to the point array.
    /// @param nPoints Number of points to append.
    void appendVertices(const gui::gl::Point2d* points, size_t nPoints); //must be only 1 position in current layout (vec2)
    /// @brief Appends indices from a push-back vector.
    /// @param indices Vector of indices to append.
    void appendIndices(const cnt::PushBackVector<gui::gl::IndexType>& indices);

    /// @brief Appends a single 2D point as a vertex.
    /// @param pt The 2D point to append.
    void appendVertex(const gui::gl::Point2d& pt);
    /// @brief Appends a single index value.
    /// @param index The index value to append.
    void appendIndex(gui::gl::IndexType index);
    /// @brief Overwrites the 2D position at the given vertex position.
    /// @param pos Zero-based index of the vertex to update.
    /// @param pt New 2D point value.
    void updateVertex(size_t pos, const gui::gl::Point2d& pt);
    /// @brief Reads the 2D position of a vertex at the given position.
    /// @param pos Zero-based index of the vertex to read.
    /// @param pt Output parameter that receives the point value.
    void getVertex(size_t pos,gui::gl::Point2d& pt) const;

    /// @brief Inserts a 2D vertex at the specified position, shifting subsequent entries.
    /// @param pos Zero-based index at which to insert.
    /// @param pt The 2D point to insert.
    void insertVertex(size_t pos, const gui::gl::Point2d& pt);

    /// @brief Generates a unit cube and appends its geometry to the buffer.
    /// @param sideLength Length of each cube side.
    /// @param textureType Texture type to apply (default: None).
    /// @param generateNormals Whether to generate per-vertex normals.
    /// @return Pointer to the resulting draw command, or nullptr on failure.
    gui::gl::Command* addCube(float sideLength, gui::gl::Texture::Type textureType = gui::gl::Texture::Type::None, bool generateNormals = false);

    /// @brief Generates a sphere and appends its geometry to the buffer.
    /// @param r Sphere radius.
    /// @param nSectors Number of longitude sectors.
    /// @param nStacks Number of latitude stacks.
    /// @param textureType Texture type to apply (default: None).
    /// @param generateNormals Whether to generate per-vertex normals.
    /// @param xyAsym Asymmetry factor in the XY plane (1.0 = symmetric).
    /// @return Pointer to the resulting draw command, or nullptr on failure.
    gui::gl::Command* addSphere(float r, td::UINT4 nSectors, td::UINT4 nStacks, gui::gl::Texture::Type textureType = gui::gl::Texture::Type::None, bool generateNormals = false, float xyAsym = 1.0f);

    //get vertex index position from current layout ID
    /// @brief Returns the total number of vertices currently stored.
    /// @return Vertex count as a 32-bit unsigned integer.
    td::UINT4 getNoOfVertices() const;
    /// @brief Returns the total number of indices currently stored.
    /// @return Index count.
    size_t getNoOfIndices() const;
    /// @brief Returns the total number of instance entries currently stored.
    /// @return Instance count.
    size_t getNoOfInstances() const;

    //activate using current layout
    /// @brief Binds the VAO and makes this buffer the active one.
    void activate() const;
    /// @brief Unbinds the VAO for this buffer.
    void deActivate() const;
//    void setCurrentLayoutID(td::INT1 layoutID);
//    td::INT1 getCurrentLayoutID() const;

    /// @brief Returns the stride in bytes for the vertex buffer.
    /// @return Vertex stride in bytes.
    td::UINT2 getVertexStride() const;
    /// @brief Returns the stride in bytes for the instance buffer.
    /// @return Instance stride in bytes.
    td::UINT2 getInstanceStride() const;

    /// @brief Returns a pointer to the internal command list.
    /// @return Pointer to the CommandList owned by this buffer.
    gui::gl::CommandList* getCommandList();

    //reserve number of commands
    /// @brief Pre-allocates storage for the specified number of draw commands.
    /// @param nCommands Number of commands to reserve.
    void reserveCommands(size_t nCommands);
    //places command on the list and links function pointers
    /// @brief Encodes a command into the command list, linking its function pointers.
    /// @param pComm Pointer to the command to encode.
    /// @return True if encoding succeeded.
    bool encode(gui::gl::Command* pComm);
    //removes pComm from list if it is last in the list
    /// @brief Removes a command from the list if it is the last entry.
    /// @param pComm Pointer to the command to remove.
    /// @return True if removal succeeded.
    bool remove(gui::gl::Command* pComm);
    //creates empy command
    /// @brief Allocates and returns a new empty Command from the internal list.
    /// @return Pointer to the newly created Command.
    gui::gl::Command* createCommand();
    //transfer data to GPU
    /// @brief Uploads all CPU-side data to the GPU buffers.
    /// @return True if the commit was successful.
    bool commit();

    //release from GPU and reset internal pointers
    /// @brief Releases GPU resources and resets all internal pointers without freeing CPU memory.
    void reset();
    //release all memory allocated by the Buffer
    /// @brief Releases all GPU and CPU memory held by this buffer.
    void release();
};


} //namespace gl

} //namespace gui
