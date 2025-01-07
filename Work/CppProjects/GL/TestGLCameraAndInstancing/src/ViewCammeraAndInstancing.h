//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/CameraView.h>
#include <gui/Key.h>
#include <gui/Texture.h>
#include <gui/FileDialog.h>
#include "Program.h"
#include <gui/gl/CommandList.h>
#include <gui/gl/Buffer.h>
#include <gui/gl/Textures.h>
#include <gui/gl/ObjectGenerator.h>

//Demonstrates:

// - camera opertions
// - all in one textures
// - animation 3D mode with Buffer, CommandList, Command, Program,...

class ViewCammeraAndInstancing : public gui::gl::CameraView
{
    using Base = gui::gl::View;
    gui::Texture _frameTexture; //for frame export
    gui::Texture _textureDiceWithNumbers; //to map on cube
    gui::Texture _textureDiceWithDots; //to map on cube
    gui::Texture _textureEarth;
    gui::Texture _textureSand;
    
    gui::gl::Camera _camera;
    glm::mat4 _perspectiveMatrix;
    
    glm::mat4 _cameraProjectionMatrix;
    
    gui::gl::Textures _textures;
    gui::gl::Buffer _gpuBuffer;

    Program _program;
    gui::gl::Command* pMtxSetterCmd = nullptr;
    gui::gl::Command* pCubeTextureCmd = nullptr;
    
    float _accAngle = 0;
    float _dAngle = 0.01f;
    
    td::BYTE _usedTexture = 0; //0 or 1
private:
    bool setupShaders()
    {
        if (!_program.link(":shVert", ":shFrag"))
            return false;
        return true;
    }
    
    //setup data and send it to GPU, create command list
    void setup()
    {
        //encode command to set transformation matrix (uniform)
        pMtxSetterCmd = _gpuBuffer.createCommand();
        pMtxSetterCmd->createMVPSetter(&_cameraProjectionMatrix);
        _gpuBuffer.encode(pMtxSetterCmd);
        
        float instanceData[] ={
            0,              0,       0, -4, 0, 0, -1, 0, 1,
            math::FLT_PI/4, 0,       0, -3, 0, 0, 0, 1, -1,
            0, math::FLT_PI/4,       0, -2, 0, 0, 0, 1, -1,
            0, math::FLT_PI/3,       0, -1, 0, 0, 1, 1, -1,
            0, math::FLT_PI/2,       0,  0, 0, 0, 1, 1, -1,
            0, math::FLT_PI/3,       0,  1, 0, 0, 1, 1, -1,
            0, math::FLT_PI/4,       0,  2, 0, 0, 1, 1, -1,
            math::FLT_PI/4, 0,       0,  3, 0, 0, 1, 1, -1,
            0,              0,       0,  4, 0, 0, 1, 1, -1,
            
            0,              0,       0, -4, -1, -1, -1, 0, 1,
            math::FLT_PI/4, math::FLT_PI/2,       0, -3, -1, -1, 0, 1, -1,
            math::FLT_PI/2, math::FLT_PI/4,       0, -2, -1, -1, 0, 1, -1,
            math::FLT_PI/2, math::FLT_PI/3,       0, -1, -1, -1, 1, 1, -1,
            math::FLT_PI/2, math::FLT_PI/2,       0,  0, -1, -1, 1, 1, -1,
            math::FLT_PI/2, math::FLT_PI/3,       0,  1, -1, -1, 1, 1, -1,
            math::FLT_PI/2, math::FLT_PI/4,       0,  2, -1, -1, 1, 0, -1,
            math::FLT_PI/4, 0,       0,  3, 0, -1, 1, 1, -1,
            math::FLT_PI/2,              0,       0,  4, -1, -1, 1, 1, -1,
            
            0,              0,       math::FLT_PI/2, -4, 1, -2, -1, 0, 1,
            math::FLT_PI/4, 0,       math::FLT_PI/2, -3, 1, -2, 0, 1, -1,
            0, math::FLT_PI/4,       math::FLT_PI/2, -2, 1, -2, 0, 1, -1,
            0, math::FLT_PI/3,       math::FLT_PI/2, -1, 1, -2, 1, 1, -1,
            0, math::FLT_PI/2,       math::FLT_PI/2,  0, 1, -2, 1, 1, -1,
            0, math::FLT_PI/3,       math::FLT_PI/2,  1, 1, -2, 1, 1, -1,
            0, math::FLT_PI/4,       math::FLT_PI/2,  2, 1, -2, 1, 1, -1,
            math::FLT_PI/4, 0,       math::FLT_PI/2,  3, 1, -2, 1, 1, -1,
            0,              0,       math::FLT_PI/2,  4, 1, -2, 1, 1, -1,
            
            0,              0,       math::FLT_PI/2, -4, -3, -2, -1, 0, 1,
            math::FLT_PI/4, 0,       math::FLT_PI/2, -3, -3, -2, 0, 1, -1,
            0, math::FLT_PI/4,       math::FLT_PI/2, -2, -3, -2, 0, 0, -1,
            0, math::FLT_PI/3,       math::FLT_PI/2, -1, -3, -2, 1, 0, 0,
            0, math::FLT_PI/2,       math::FLT_PI/2,  0, -3, -2, 1, 0, -1,
            0, math::FLT_PI/3,       math::FLT_PI/2,  1, -3, -2, 0, 1, 0,
            0, math::FLT_PI/4,       math::FLT_PI/2,  2, -3, -2, 0, -1, 0,
            math::FLT_PI/4, 0,       math::FLT_PI/2,  3, -3, -2, 0, 0, -1,
            0,              0,       math::FLT_PI/2,  4, -3, -2, 0, 0, 1,
            
            0,              0,       math::FLT_PI/2, -4, -5, -2, 1, 0, -1,
            math::FLT_PI/4, 0,       math::FLT_PI/2, -3, -5, -2, 0, -1, 1,
            0, math::FLT_PI/4,       math::FLT_PI/2, -2, -5, -2, 0, 0, -1,
            0, math::FLT_PI/3,       math::FLT_PI/2, -1, -5, -2, -1, 0, 0,
            0, math::FLT_PI/2,       math::FLT_PI/2,  0, -5, -2, 1, 0, 1,
            0, math::FLT_PI/3,       math::FLT_PI/2,  1, -5, -2, 0, -1, 0,
            0, math::FLT_PI/4,       math::FLT_PI/2,  2, -5, -2, 0, 1, 0,
            math::FLT_PI/4, 0,       math::FLT_PI/2,  3, -5, -2, 0, 0, 1,
            0,              0,       math::FLT_PI/2,  4, -5, -2, 0, 0, -1
        };
        
        _gpuBuffer.appendInstances(instanceData, 45);
        
        //encode command to draw textured cube
        // Define the cube's vertices and texture coordinates
        gui::gl::Command* cmd = _gpuBuffer.addCube(1.0f, gui::gl::Texture::Type::CubeMapAllInOne);
        cmd->setNumberOfInstances(45);
        _gpuBuffer.encode(cmd);
        
        if (!_gpuBuffer.commit())
        {
            mu::dbgLog("ERROR! Cannot commit buffer to GPU");
            return;
        }
        
        // Load textures
        _textures.append({&_textureDiceWithNumbers, &_textureDiceWithDots, &_textureEarth, &_textureSand});
        
        _program.setBuffer(&_gpuBuffer);
        _program.setTextures(&_textures);
    }
protected:
    
    void onInit() override
    {
        auto [major, minor] = getOpenGLVersion();
        mu::dbgLog("Used opengl %d.%d", major, minor);
        
        if (!setupShaders())
        {
            mu::dbgLog("ERROR! OpenGL cannnot setup shaders!");
            assert(false);
            return;
        }
        
        //specify buffer layouts before creatin the context
        _gpuBuffer.init(64, 100, 100, 100, {gui::gl::DataType::vec3, gui::gl::DataType::vec2}, {gui::gl::DataType::vec3, gui::gl::DataType::vec3, gui::gl::DataType::vec3});
        
        setup();
        // Set up the perspective parameters
        float fov = 45.0f; // Field of view in degrees
        float aspectRatio = 16.0f / 9.0f; // Aspect ratio of the viewport
        float nearClip = 0.1f; // Near clipping plane
        float farClip = 100.0f; // Far clipping plane

        // Create a perspective matrix
        _perspectiveMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        _cameraProjectionMatrix = _perspectiveMatrix * _camera.getViewMatrix(); //* I for model
        
        _program.setMVP(_cameraProjectionMatrix);
        _program.setAccAngle(_accAngle);
        
        gui::gl::Context::enable(gui::gl::Context::Flag::DepthTest);
        gui::gl::Context::enable(gui::gl::Context::Flag::CullFace);
        
#ifdef DEBUG_GL
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            mu::dbgLog("OpenGL error: %x", error);
        }
#endif
        
    }
    
    bool prepareNextFrame() override
    {
        _accAngle += _dAngle;
        if (_accAngle >= 2*math::FLT_PI)
        {
            _accAngle -= 2*math::FLT_PI;
        }
        _cameraProjectionMatrix = _perspectiveMatrix * _camera.getViewMatrix();
        return true;
//        reDraw();
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        // Clear
        gui::gl::Context::clear(td::ColorID::Black);
        gui::gl::Context::clear({gui::gl::Context::Clear::Color, gui::gl::Context::Clear::Depth});

        _program.activate();
        
        _program.setAccAngle(_accAngle);
        if (_usedTexture == 0)
            _program.setTextureUniforms(0, 1);
        else
            _program.setTextureUniforms(2, 3);
        
        _program.execute();

        // Unbind program
        _program.deActivate();
    }

public:
    ViewCammeraAndInstancing()
    : _textureDiceWithNumbers(":numbers")
    , _textureDiceWithDots(":dots")
    , _textureEarth(":earth")
    , _textureSand(":sand")
    , _camera(glm::vec3(3.0, 3.0, 10.0f), glm::vec3(0.0f, 0.0f, 0.5f))
    {
        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes, gui::gl::DepthBuffer::Size::B2);
        createContext(reqContext);
        setCamera(&_camera);
    }
    
    ~ViewCammeraAndInstancing()
    {
        makeCurrentContext();
    }
    
    void updateSpeed(float val)
    {
        _dAngle = val;
    }
    
    void switchTexture()
    {
        if (_usedTexture == 0)
            _usedTexture = 1;
        else
            _usedTexture = 0;
    }
    
    void exportFrame()
    {
        _frameTexture.setExportJPGQuality(80); //80%
        
        readFrameFrameBuffer(_frameTexture, [this]()
        {
            _frameTexture.mirrorVertically();
            
            td::UINT4 dlgID = 6666; //should be unique dlg ID
            
            gui::SaveFileDialog::show(this, "Export Frame", {{"JPEG file with losses", "*.jpg"}, {"PNG file (no losses)", "*.png"},{"TGA file (no losses)", "*.tga"},{"Bitmap file (no losses)", "*.bmp"}}, dlgID, [this](gui::FileDialog* pFD)
            {
              if (pFD->getStatus() == gui::FileDialog::Status::OK)
              {
                  const char* pExt = pFD->getFileExtension(); //just for debug
                  if (_frameTexture.saveToFile(pFD->getFileName()))
                  {
                      gui::Alert::show("Export Frame OK", pFD->getFileName());
                  }
              }
            }, "Frame");
            
//            gui::SaveFileDialog* dlg = new gui::SaveFileDialog(this, "Export Frame", {{"JPEG file with losses", "*.jpg"}, {"PNG file (no losses)", "*.png"},{"TGA file (no losses)", "*.tga"},{"Bitmap file (no losses)", "*.bmp"}}, "Frame");
//            
//            dlg->openModal([this](gui::FileDialog* pFD)
//            {
//                if (pFD->getStatus() == gui::FileDialog::Status::OK)
//                {
//                    const char* pExt = pFD->getFileExtension(); //just for debug
//                    if (_frameTexture.saveToFile(pFD->getFileName()))
//                    {
//                        gui::Alert::show("Export Frame OK", pFD->getFileName());
//                    }
//                }
//            });
        });
    }
};
