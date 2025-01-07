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
#include <gui/Color.h>

class MainView : public gui::gl::View
{
    using Base = gui::gl::View;
    gui::Texture _samplerWoodFloor; 
    gui::Texture _samplerWoodFloorNormal;

    glm::mat4 _perspectiveMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _mvpMatrix;

    glm::mat4 _modelViewMat;
    glm::mat4 _normalMat;
    glm::mat4 _modelMatrix;

    glm::vec3 _lightPos;
    gui::gl::Textures _textures;
    gui::gl::Buffer _gpuBuffer;
    gui::gl::Buffer _outlineBuffer;

    Program _mainPass;

    ProgramOutline _outlinePass;
    
    float _angleX = 0;
    float _angleY = 0;

    bool rotation = false;
    float _dAngle = 0.01f;
    float _dAngleAct = 0.f;

    glm::vec3 _sunColor = glm::vec3(1.f);
    
private:
    bool setupShaders()
    {
        if (!_mainPass.link(":shVert", ":shFrag"))
            return false;

        if (!_outlinePass.link(":outlineV", ":outlineF"))
            return false;

        return true;
    }
    
    //setup data and send it to GPU, create command list
    void setup()
    {
        //specify buffer layouts before creatin the context
        _gpuBuffer.init(64, 100, 100, { gui::gl::DataType::vec3, gui::gl::DataType::vec2, gui::gl::DataType::vec3 });

        //encode command to set transformation matrix (uniform)
        auto pMtxSetterCmd = _gpuBuffer.createCommand();
        pMtxSetterCmd->createMVPSetter(&_mvpMatrix);
        _gpuBuffer.encode(pMtxSetterCmd);

         //Define the cube's vertices and texture coordinates

#define NORMAL_xPLUS 1.0f, 0.0f, 0.0f
#define NORMAL_xMINUS -1.0f, 0.0f, 0.0f
#define NORMAL_yPLUS 0.0f, 1.0f, 0.0f
#define NORMAL_yMINUS 0.0f, -1.0f, 0.0f
#define NORMAL_zPLUS 0.0f, 0.0f, 1.0f
#define NORMAL_zMINUS 0.0f, 0.0f, -1.0f

        float vertices[] = {
            // Front face (number 1)
            0.5f,   0.5f,  0.5f,  1.0f, 1.0f, NORMAL_zPLUS,    // Vertex 0 (top-right)
           -0.5f,   0.5f,  0.5f,  0.0f, 1.0f, NORMAL_zPLUS,   // Vertex 1 (top-left)
           -0.5f,  -0.5f,  0.5f,  0.0f, 0.0f, NORMAL_zPLUS,   // Vertex 2 (bottom-left)
            0.5f,  -0.5f,  0.5f,  1.0f, 0.0f, NORMAL_zPLUS,   // Vertex 3 (bottom-right)

            // Right face (number 2)
            0.5f, -0.5f,  0.5f,  1.0f, 1.0f, NORMAL_xPLUS,// Vertex 8 (bottom-left)
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, NORMAL_xPLUS,// Vertex 9 (bottom-right)
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f, NORMAL_xPLUS,// Vertex 10 (top-left)
            0.5f,  0.5f, 0.5f,   1.0f, 0.0f, NORMAL_xPLUS,// Vertex 11 (top-right)
            
            // Back face (number 6)
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  NORMAL_zMINUS, // Vertex 0 (top-right)
           -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  NORMAL_zMINUS,  // Vertex 1 (top-left)
           -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,  NORMAL_zMINUS, // Vertex 2 (bottom-left)
            0.5f,  0.5f, -0.5f,  1.0f, 0.0f,  NORMAL_zMINUS, // Vertex 3 (bottom-right)
            
            // Left face (number 5)
           -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, NORMAL_xMINUS,// Vertex 12 (bottom-left)
           -0.5f,  -0.5f, 0.5f,  0.0f, 1.0f, NORMAL_xMINUS,// Vertex 13 (bottom-right)
           -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, NORMAL_xMINUS,// Vertex 14 (top-left)
           -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, NORMAL_xMINUS,// Vertex 15 (top-right)

            // Top face (number 3)
           -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, NORMAL_yPLUS,// Vertex 16 (bottom-left)
            0.5f,  0.5f,  0.5f,  0.0f, 1.0f, NORMAL_yPLUS,// Vertex 17 (bottom-right)
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f, NORMAL_yPLUS,// Vertex 18 (top-left)
           -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, NORMAL_yPLUS,// Vertex 19 (top-right)

            // Bottom face (number 4)
           -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, NORMAL_yMINUS,   // Vertex 20 (bottom-left)
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, NORMAL_yMINUS,   // Vertex 21 (bottom-right)
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f, NORMAL_yMINUS,   // Vertex 22 (top-left)
           -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, NORMAL_yMINUS   // Vertex 23 (top-right)
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


        
        //encode command to draw textured cube
        auto pCubeTextureCmd = _gpuBuffer.createCommand();
        pCubeTextureCmd->createDrawElements(gui::gl::Primitive::Triangles, 0, nIndices);
        pCubeTextureCmd->setTexture(gui::gl::Textures::Unit::T0, 0);

        _gpuBuffer.encode(pCubeTextureCmd);

        if (!_gpuBuffer.commit())
        {
            mu::dbgLog("ERROR! Cannot commit buffer to GPU");
            return;
        }

        // Load textures
        _textures.append({ &_samplerWoodFloor, &_samplerWoodFloorNormal });
        _mainPass.setBuffer(&_gpuBuffer);

        _mainPass.setTextures(&_textures);

        //specify buffer layouts before creatin the context
        _outlineBuffer.init(64, 100, 100, { gui::gl::DataType::vec3,gui::gl::DataType::vec2, gui::gl::DataType::vec3 });

        _outlineBuffer.appendVertices(vertices, nVertices);

        _outlineBuffer.appendIndices(indices, nIndices);

        //encode command to draw textured cube
        auto pCubeTextureCmdOutline = _outlineBuffer.createCommand();
        pCubeTextureCmdOutline->createDrawElements(gui::gl::Primitive::Triangles, 0, nIndices);

        _outlineBuffer.encode(pCubeTextureCmdOutline);

        if (!_outlineBuffer.commit())
        {
            mu::dbgLog("ERROR! Cannot commit buffer to GPU");
            return;
        }

        _outlinePass.setBuffer(&_outlineBuffer);

        _mainPass.setTextureUniforms();

        gui::gl::Context::enable(gui::gl::Context::Flag::DepthTest);

        gui::gl::Context::setDepthBufferFunction(gui::gl::DepthBuffer::Function::Less);

        gui::gl::Context::enable(gui::gl::Context::Flag::StencilTest);

        gui::gl::Context::setStencilBufferOperation(gui::gl::StencilBuffer::Operation::Keep, gui::gl::StencilBuffer::Operation::Keep, gui::gl::StencilBuffer::Operation::Replace);

        gui::gl::Context::enable(gui::gl::Context::Flag::CullFace);
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
        float fov = 90.0f; // Field of view in degrees
        float aspectRatio = 16.0f / 9.0f; // Aspect ratio of the viewport
        float nearClip = 0.1f; // Near clipping plane
        float farClip = 100.0f; // Far clipping plane

        // Create a perspective matrix
        _perspectiveMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        
        // Camera parameters
        glm::vec3 cameraPosition = glm::vec3(2.0, 0.0, 2.0f);  // New camera position
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);    // Camera target (where the camera is looking)
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);         // Up vector

        // Create a new view matrix
        _viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
        
        _mvpMatrix = _perspectiveMatrix * _viewMatrix; //* I for model

        _lightPos = glm::vec3(-2., 0., 2.);
        
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
        _angleX += _dAngleAct;
        //_angleY += _dAngleAct;

        // Create a model matrix with rotations around X and Y axes
        _modelMatrix = glm::mat4(1.0f);
        _modelMatrix = glm::rotate(_modelMatrix, _angleX, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X axis
        _modelMatrix = glm::rotate(_modelMatrix, _angleY, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y axis

        // Combine the perspective, view, and model matrices to get the final MVP matrix
        _modelViewMat = _viewMatrix * _modelMatrix;
        _normalMat = glm::transpose(glm::inverse(_modelViewMat));
        _mvpMatrix = _perspectiveMatrix * _viewMatrix * _modelMatrix;
        return true;
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        // Clear
        gui::gl::Context::clear(td::ColorID::Black);
        gui::gl::Context::clear({gui::gl::Context::Clear::Color, gui::gl::Context::Clear::Depth, gui::gl::Context::Clear::Stencil });

        gui::gl::Context::setStencilBufferFunction(gui::gl::StencilBuffer::Function::Always, 1, 0xFF);
        gui::gl::Context::setStencilBufferMask(0xFF);

        _mainPass.activate();    
        _mainPass.setMV(_modelViewMat);
        _mainPass.setN(_normalMat);
        _mainPass.setLight(_lightPos, _sunColor);
        _mainPass.setV(_viewMatrix);
        _mainPass.setTextureUniforms();
        _mainPass.execute();
//        _mainPass.execute(0, 2);

        _mainPass.deActivate();
      
        gui::gl::Context::setStencilBufferFunction(gui::gl::StencilBuffer::Function::NotEqual, 1, 0xFF);//Passes if (ref & mask) != (stencil & mask). << pass ako je stencil value = 0 
        gui::gl::Context::setStencilBufferMask(0x00);//dont write to stencil buffer
        gui::gl::Context::disable(gui::gl::Context::Flag::DepthTest);//dont write to depth buffer, not used here anyway
        
        _outlinePass.activate();
        _outlinePass.setP(_perspectiveMatrix);
        _outlinePass.setV(_viewMatrix);

#define SCALEMODEL

#ifdef SCALEMODEL
        float outlineThickness = 0.03;
        auto scaledmodel = glm::scale(_modelMatrix, glm::vec3(1.0f + outlineThickness));
        _outlinePass.setM(scaledmodel);
#else
        _outlinePass.setM(_modelMatrix);
#endif
        _outlinePass.execute();
        //_outlinePass.execute(2, 1);

        _outlinePass.deActivate();

        gui::gl::Context::setStencilBufferFunction(gui::gl::StencilBuffer::Function::Always, 0, 0xFF);
        gui::gl::Context::setStencilBufferMask(0xFF);
        gui::gl::Context::enable(gui::gl::Context::Flag::DepthTest);
    }

public:
    MainView()
    : _samplerWoodFloor(":wood_floor")
    , _samplerWoodFloorNormal(":wood_floor_NRM")

    {
        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes, gui::gl::DepthBuffer::Size::B3, gui::gl::StencilBuffer::Size::B1);
        createContext(reqContext, {gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks });


       // setAnimation(true);
    }
    
    ~MainView()
    {
        makeCurrentContext();
    }
    
    void updateSpeed(float val)
    {
        _dAngle = val;
        _dAngleAct = rotation ? _dAngle : 0.f;
    }
    
    void sunColorPicker(td::ColorID color)
    {
        float a;
        gui::Color::getRGBA(color, _sunColor[0], _sunColor[1], _sunColor[2], a);
    }

    void switchRotation()
    {
        rotation = !rotation;
        _dAngleAct = rotation ? _dAngle : 0.f;
    }

};
