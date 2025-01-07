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
#include <td/Types.h>
#include "Program.h"
#include "Textures.h"

namespace gui
{
namespace gl
{

class Camera;

class NATGL_API SkyBox
{
    friend class Camera;
protected:
    class Prog : public gui::gl::Program
    {
        td::INT4 _samplerPos = -1;
    protected:
        void onLinked() override;
        void onError() override;
    public:
        Prog();
        void setSampler(gui::gl::Textures::Unit unit);
    };

private:
protected:
    Prog _program;
    glm::mat4 _skyBoxMtx;
    const glm::mat4* _projectionMatrix = nullptr;
    const gui::gl::Camera* _pCamera = nullptr;
    
    gui::gl::Textures::Unit _texUnit = gui::gl::Textures::Unit::T0;
    td::UINT4 _textureID = 0;
    td::UINT4 _vao = 0;
    td::UINT4 _vbo = 0;
    td::BYTE _commited = 0;
    
public:
    SkyBox();
    ~SkyBox();
    //transfer data to GPU
    void init(const glm::mat4* pProjectionMatrix,  td::UINT4 textureID, gui::gl::Textures::Unit tu);
    void setCamera(const gui::gl::Camera* pCamera);
    void updateTransformation();
    void commit(float sideSize);

    void render(bool disableDepthTest = true);
    
    //release all memory allocated by the Buffer
    void release();
};

} //namespace gl
} //namespace gui


