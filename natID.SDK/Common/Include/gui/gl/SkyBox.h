// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SkyBox.h
    @brief Declares the gui::gl::SkyBox class for rendering a cube-mapped sky box. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <td/Types.h>
#include "Program.h"
#include "Textures.h"

namespace gui
{
namespace gl
{

class Camera;

/// @brief Renders a cube-mapped sky box around the scene using a dedicated GLSL program.
class NATGL_API SkyBox
{
    friend class Camera;
protected:
    /// @brief Internal GLSL program used exclusively for sky box rendering.
    class Prog : public gui::gl::Program
    {
        td::INT4 _samplerPos = -1; ///< Uniform location of the cube-map sampler in the shader.
    protected:
        /// @brief Called after the program is successfully linked; caches uniform locations.
        void onLinked() override;

        /// @brief Called when a link error occurs; logs diagnostic information.
        void onError() override;
    public:
        /// @brief Default constructor; registers the sky box vertex and fragment shaders.
        Prog();

        /// @brief Binds the cube-map sampler uniform to the specified texture unit.
        /// @param unit The texture unit to bind the sampler to.
        void setSampler(gui::gl::Textures::Unit unit);
    };

protected:
    Prog _program;                                ///< GLSL program used for sky box rendering.
    glm::mat4 _skyBoxMtx;                         ///< Model matrix applied to the sky box geometry.
    const glm::mat4* _projectionMatrix = nullptr; ///< Pointer to the projection matrix provided by the caller.
    const gui::gl::Camera* _pCamera = nullptr;    ///< Pointer to the camera providing view transformation.

    gui::gl::Textures::Unit _texUnit = gui::gl::Textures::Unit::T0; ///< Texture unit to which the cube map is bound.
    td::UINT4 _textureID = 0; ///< OpenGL ID of the cube-map texture.
    td::UINT4 _vao = 0;       ///< OpenGL Vertex Array Object ID for sky box geometry.
    td::UINT4 _vbo = 0;       ///< OpenGL Vertex Buffer Object ID for sky box geometry.
    td::BYTE _commited = 0;   ///< Flag indicating whether geometry has been committed to the GPU.

    /// @brief Deleted copy constructor; sky boxes are non-copyable.
    SkyBox(const SkyBox&) = delete;

    /// @brief Deleted copy-assignment operator; sky boxes are non-copyable.
    SkyBox& operator =(const SkyBox&) = delete;
public:
    /// @brief Default constructor; creates an uninitialised sky box.
    SkyBox();

    /// @brief Destructor; releases all GPU resources held by the sky box.
    ~SkyBox();

    //transfer data to GPU
    /// @brief Initialises the sky box with a projection matrix, cube-map texture and texture unit.
    /// @param pProjectionMatrix Pointer to the projection matrix to use for rendering.
    /// @param textureID         OpenGL ID of the cube-map texture.
    /// @param tu                Texture unit to which the cube map will be bound.
    void init(const glm::mat4* pProjectionMatrix, td::UINT4 textureID, gui::gl::Textures::Unit tu);

    /// @brief Assigns the camera used to derive the view matrix for sky box rendering.
    /// @param pCamera Pointer to the camera object.
    void setCamera(const gui::gl::Camera* pCamera);

    /// @brief Recomputes the sky box model matrix from the current camera state.
    void updateTransformation();

    /// @brief Uploads the sky box cube geometry with the given side length to the GPU.
    /// @param sideSize Half-size (or full size, depending on convention) of the cube sides.
    void commit(float sideSize);

    /// @brief Renders the sky box, optionally disabling the depth test to draw it behind all geometry.
    /// @param disableDepthTest True (default) to disable depth testing during rendering.
    void render(bool disableDepthTest = true);

    //release all memory allocated by the Buffer
    /// @brief Releases all CPU and GPU memory allocated by the sky box.
    void release();
};

} //namespace gl
} //namespace gui
