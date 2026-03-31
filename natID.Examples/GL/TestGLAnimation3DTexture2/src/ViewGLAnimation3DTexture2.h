//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/View.h>
#include <gui/Key.h>
#include <gui/Texture.h>
#include <gui/FileDialog.h>
#include "Program.h"
#include <gui/gl/CommandList.h>
#include <gui/gl/Buffer.h>
#include <gui/gl/Textures.h>

//Demonstrates:
// - all in one textures
// - animation 3D mode with Buffer, CommandList, Command, Program,...

class ViewGLAnimation3DTexture2 : public gui::gl::View
{
    using Base = gui::gl::View;
    gui::Texture _frameTexture; //for frame export
    gui::Texture _samplerDiceWithNumbers; //to map on cube
    gui::Texture _samplerDiceWithDots; //to map on cube
    glm::mat4 _perspectiveMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _mvpMatrix;
    
    
//    GLuint _textureIDs[2];  //two textures
    gui::gl::Textures _textures;
    gui::gl::Buffer _gpuBuffer;
    
    Program _program;
    gui::gl::Command* pMtxSetterCmd = nullptr;
    gui::gl::Command* pCubeTextureCmd = nullptr;
    
    float _angleX = 0;
    float _angleY = 0;
    float _dAngle = 0.01f;
    
//    const unsigned int nTriangles = 6 * 2;   // Number of triangles rendered
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
        pMtxSetterCmd->createMVPSetter(&_mvpMatrix);
        _gpuBuffer.encode(pMtxSetterCmd);
        
        // Define the cube's vertices and texture coordinates
        float vertices[] = {
            // Front face (number 1)
            0.5f,   0.5f,  0.5f,  1.0f / 6.0f, 1.0f,    // Vertex 0 (top-right)
           -0.5f,   0.5f,  0.5f,  0.0f, 1.0f,           // Vertex 1 (top-left)
           -0.5f,  -0.5f,  0.5f,  0.0f, 0.0f,           // Vertex 2 (bottom-left)
            0.5f,  -0.5f,  0.5f,  1.0f / 6.0f, 0.0f,      // Vertex 3 (bottom-right)

            // Right facec (number 2)
            0.5f, -0.5f,  0.5f,  1.0f / 6.0f, 0.0f, // Vertex 8 (bottom-left)
            0.5f, -0.5f, -0.5f,  2.0f / 6.0f, 0.0f, // Vertex 9 (bottom-right)
            0.5f,  0.5f, -0.5f,  2.0f / 6.0f, 1.0f, // Vertex 10 (top-left)
            0.5f,  0.5f, 0.5f,   1.0f / 6.0f, 1.0f, // Vertex 11 (top-right)
            
            // Back face (number 6)
            0.5f, -0.5f, -0.5f,  2.0f / 6.0f, 0.0f,   // Vertex 0 (top-right)
           -0.5f, -0.5f, -0.5f,  3.0f / 6.0f, 0.0f,    // Vertex 1 (top-left)
           -0.5f,  0.5f, -0.5f,  3.0f / 6.0f, 1.0f,   // Vertex 2 (bottom-left)
            0.5f,  0.5f, -0.5f,  2.0f / 6.0f, 1.0f,   // Vertex 3 (bottom-right)
            
            // Left face (number 5)
           -0.5f, -0.5f, -0.5f,  3.0f / 6.0f, 0.0f, // Vertex 12 (bottom-left)
           -0.5f,  -0.5f, 0.5f,  4.0f / 6.0f, 0.0f, // Vertex 13 (bottom-right)
           -0.5f,  0.5f,  0.5f,  4.0f / 6.0f, 1.0f, // Vertex 14 (top-left)
           -0.5f,  0.5f, -0.5f,  3.0f / 6.0f, 1.0f, // Vertex 15 (top-right)

            // Top face (number 3)
           -0.5f,  0.5f,  0.5f,  4.0f / 6.0f, 0.0f, // Vertex 16 (bottom-left)
            0.5f,  0.5f,  0.5f,  5.0f / 6.0f, 0.0f, // Vertex 17 (bottom-right)
            0.5f,  0.5f, -0.5f,  5.0f / 6.0f, 1.0f, // Vertex 18 (top-left)
           -0.5f,  0.5f, -0.5f,  4.0f / 6.0f, 1.0f, // Vertex 19 (top-right)

            // Bottom face (number 4)
           -0.5f, -0.5f, -0.5f,  5.0f / 6.0f, 0.0f,    // Vertex 20 (bottom-left)
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,           // Vertex 21 (bottom-right)
            0.5f, -0.5f,  0.5f,  1.0f, 1.0f,           // Vertex 22 (top-left)
           -0.5f, -0.5f,  0.5f,  5.0f / 6.0f, 1.0f,    // Vertex 23 (top-right)
        };
        
        td::UINT4 indices[] =
        {
            0, 1, 2, // Front face
            2, 3, 0, // Front face

            4, 5, 6, // Rigt face
            4, 6, 7, // Rigt face

            8, 9, 11, // Back face
            9, 10, 11, // Back face

            12, 13, 14, // Left face
            12, 14, 15, // Left face

            16, 17, 18, // Top face
            16, 18, 19, // Top face

            20, 21, 22, // Bottom face
            20, 22, 23  // Bottom face
        };

        td::UINT4 nVertices = 4*6;
        
        _gpuBuffer.appendVertices(vertices, nVertices);
        
        td::UINT4 nIndices = 3*2*6;
        _gpuBuffer.appendIndices(indices, nIndices);
        
        //encode command to draw tetured cube
        pCubeTextureCmd = _gpuBuffer.createCommand();
        pCubeTextureCmd->createDrawElements(gui::gl::Primitive::Triangles, 0, nIndices);
        pCubeTextureCmd->setTexture(gui::gl::Textures::Unit::T0, _usedTexture);
        _gpuBuffer.encode(pCubeTextureCmd);
        
        if (!_gpuBuffer.commit())
        {
            mu::dbgLog("ERROR! Cannot commit buffer to GPU");
            return;
        }
        
        // Load textures
        _textures.append({&_samplerDiceWithNumbers, &_samplerDiceWithDots});
//        _program.setTextureUnit();
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
        setup();
        
        
        // Set up the perspective parameters
        float fov = 45.0f; // Field of view in degrees
        float aspectRatio = 16.0f / 9.0f; // Aspect ratio of the viewport
        float nearClip = 0.1f; // Near clipping plane
        float farClip = 100.0f; // Far clipping plane

        // Create a perspective matrix
        _perspectiveMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        
        // Camera parameters
        glm::vec3 cameraPosition = glm::vec3(3.0, 3.0, 3.0f);  // New camera position
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.5f);    // Camera target (where the camera is looking)
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);         // Up vector

        // Create a new view matrix
        _viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
        
        _mvpMatrix = _perspectiveMatrix * _viewMatrix; //* I for model
        
        gui::gl::Context::enable(gui::gl::Context::Flag::DepthTest);
        gui::gl::Context::enable(gui::gl::Context::Flag::CullFace);
        
        dbgCheckGLError();
    }
    
    bool prepareNextFrame() override
    {
        _angleX += _dAngle;
//        _angleY += _dAngle/2;
        // Create a model matrix with rotations around X and Y axes
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::rotate(modelMatrix, _angleX, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X axis
        modelMatrix = glm::rotate(modelMatrix, _angleY, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y axis

        // Combine the perspective, view, and model matrices to get the final MVP matrix
        _mvpMatrix = _perspectiveMatrix * _viewMatrix * modelMatrix;
        return true;
//        reDraw();
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        // Clear
        gui::gl::Context::clear(td::ColorID::Black);
        gui::gl::Context::clear({gui::gl::Context::Clear::Color, gui::gl::Context::Clear::Depth});

        _program.activate();
        _program.execute();

        // Unbind program
        _program.deActivate();
    }

public:
    ViewGLAnimation3DTexture2()
    : _samplerDiceWithNumbers(":numbers")
    , _samplerDiceWithDots(":dots")
    {
        //specify buffer layouts before creatin the context
        _gpuBuffer.init(64, 100, 100, {gui::gl::DataType::vec3, gui::gl::DataType::vec2});
        
        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes, gui::gl::DepthBuffer::Size::B2);
        createContext(reqContext, {gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks });
    }
    
    ~ViewGLAnimation3DTexture2()
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
        
        pCubeTextureCmd->setTexture(gui::gl::Textures::Unit::T0, _usedTexture);
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
