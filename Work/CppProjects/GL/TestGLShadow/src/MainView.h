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
//
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//
//
//#include <stb/stb_image_write.h>

class MainView : public gui::gl::View
{
    using Base = gui::gl::View;
    gui::Texture _samplerWood; 
    gui::Texture _samplerFloor;

    glm::mat4 _perspectiveMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _mvpMatrix;

    glm::mat4 _modelViewMat;
    glm::mat4 _normalMat;
    glm::mat4 _modelMatrix;

    glm::vec3 _lightDir;
    gui::gl::Textures _textures;
    gui::gl::Buffer _gpuBuffer;
    //gui::gl::Buffer _outlineBuffer;
//    gui::gl::Buffer _depthBuffer;

    //gui::gl::Command::AdditionalUniform _uniformMV, _uniformN;
    
    Program _mainPass;

    //ProgramOutline _outlinePass;
    
    float _angleX = 0;
    float _angleY = 0;

    bool rotation = false;
    float _dAngle = 0.01f;
    float _dAngleAct = 0.f;

    glm::vec3 _sunColor = glm::vec3(1.f);

    GLuint _shadowMapFBO;
    GLuint _depthTextureID;
    ProgramDepth _depthPass;
    glm::mat4 _depthProjectionMatrix; 
    glm::mat4 _depthViewMatrix; 
    glm::mat4 _depthMVP;

    glm::mat4 _biasMatrix; 
    gui::Size _size;
    
//    GLint _screenFBO = 0;

    glm::vec3 _cameraPosition;
    bool _cameraRotating = false;
    bool _ccwRotation = true;
    float _cameraOffset = 3.0;
    float _cameraAngleDelta = 0.01;
    bool _cameraMoveUp = false;
    bool _cameraMoveDown = false;
    float _cameraVerticalDelta = 0.2;

private:
    //Creates off-screen FBO and attached texture from Depth buffer
    void createDepthFBO()
    {
        if (_size.width == 0 || _size.height == 0)
            return;
        
        if (_depthTextureID != 0)
        {
            glDeleteTextures(1, &_depthTextureID);
            _depthTextureID= 0;
        }
        
        if (_shadowMapFBO != 0)
        {
            glDeleteFramebuffers(1, &_shadowMapFBO);
            _shadowMapFBO = 0;
        }
        
        // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
        //GLuint _framebufferName = 0;
        glGenFramebuffers(1, &_shadowMapFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, _shadowMapFBO);

        // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
        glGenTextures(1, &_depthTextureID);
        glBindTexture(GL_TEXTURE_2D, _depthTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, _size.width, _size.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depthTextureID, 0);

        // No color output in the bound framebuffer, only depth.
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        // Always check that our framebuffer is ok
        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    bool setupShaders()
    {
        if (!_mainPass.link(":shVert", ":shFrag"))
            return false;

        //if (!_outlinePass.link(":outlineV", ":outlineF"))
        //    return false;

        if (!_depthPass.link(":depthV", ":depthF"))
            return false;

        return true;
    }

    
    //setup data and send it to GPU, create command list
    void setup()
    {
        //specify buffer layouts before creatin the context
        _gpuBuffer.init(64, 100, 100, { gui::gl::DataType::vec3, gui::gl::DataType::vec2, gui::gl::DataType::vec3 });

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
        //pCubeTextureCmd->setTexture(gui::gl::Textures::Unit::T0, 0);

        _gpuBuffer.encode(pCubeTextureCmd);


        if (!_gpuBuffer.commit())
        {
            mu::dbgLog("ERROR! Cannot commit buffer to GPU");
            return;
        }

        // Load textures
        _textures.append({ &_samplerWood, &_samplerFloor });
        _mainPass.setBuffer(&_gpuBuffer);
        _mainPass.setTextures(&_textures);



        ////specify buffer layouts before creatin the context
        //_outlineBuffer.init(64, 100, 100, { gui::gl::DataType::vec3,gui::gl::DataType::vec2, gui::gl::DataType::vec3 });

        //_outlineBuffer.appendVertices(vertices, nVertices);

        //_outlineBuffer.appendIndices(indices, nIndices);

        ////encode command to draw textured cube
        //auto pCubeTextureCmdOutline = _outlineBuffer.createCommand();
        //pCubeTextureCmdOutline->createDrawElements(gui::gl::Primitive::Triangles, 0, nIndices);

        //_outlineBuffer.encode(pCubeTextureCmdOutline);

        //if (!_outlineBuffer.commit())
        //{
        //    mu::dbgLog("ERROR! Cannot commit buffer to GPU");
        //    return;
        //}

        //_outlinePass.setBuffer(&_outlineBuffer);

//        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &_screenFBO);
        
        createDepthFBO();
        
//        //specify buffer layouts before creatin the context
//        _depthBuffer.init(64, 100, 100, { gui::gl::DataType::vec3,gui::gl::DataType::vec2, gui::gl::DataType::vec3 });
//
//        _depthBuffer.appendVertices(vertices, nVertices);
//
//        _depthBuffer.appendIndices(indices, nIndices);
//
//        //encode command to draw textured cube
//        auto pCubeTextureCmdDepth = _depthBuffer.createCommand();
//        pCubeTextureCmdDepth->createDrawElements(gui::gl::Primitive::Triangles, 0, nIndices);
//
//        _depthBuffer.encode(pCubeTextureCmdDepth);
//
//        if (!_depthBuffer.commit())
//        {
//            mu::dbgLog("ERROR! Cannot commit buffer to GPU");
//            return;
//        }

        _depthPass.setBuffer(&_gpuBuffer);


        gui::gl::Context::enable(gui::gl::Context::Flag::DepthTest);
        gui::gl::Context::enable(gui::gl::Context::Flag::CullFace);

    }
protected:
    
    void onResize(const gui::Size& newSize) override
    {
        _size = newSize;
        makeCurrentContext();
        createDepthFBO();
    }
    
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
        float farClip = 50.0f; // Far clipping plane

        // Create a perspective matrix
        _perspectiveMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        
        // Camera parameters
        _cameraPosition = glm::vec3(0.0, _cameraOffset, 4.0f);  // New camera position
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);    // Camera target (where the camera is looking)
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);         // Up vector

        // Create a new view matrix
        _viewMatrix = glm::lookAt(_cameraPosition, cameraTarget, cameraUp);
        
        _mvpMatrix = _perspectiveMatrix * _viewMatrix; //* I for model

        _lightDir = glm::vec3(2., 5., 0.);

        _depthProjectionMatrix = glm::ortho<float>(-25, 25, -20, 20, -20, 20);
        _depthViewMatrix = glm::lookAt(_lightDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

        
        
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
        _depthMVP = _depthProjectionMatrix * _depthViewMatrix * _modelMatrix;

        //camera
        glm::mat4 cameraMatrix = glm::mat4(1.0f);
        if (_cameraMoveUp)
        {
            _cameraMoveUp = false;
            cameraMatrix = glm::translate(cameraMatrix, glm::vec3(0, _cameraVerticalDelta, 0));
        }

        if (_cameraMoveDown)
        {
            _cameraMoveDown = false;
            cameraMatrix = glm::translate(cameraMatrix, glm::vec3(0, -_cameraVerticalDelta, 0));
        }
        if (_cameraRotating)
        {
            cameraMatrix = glm::rotate(cameraMatrix, _ccwRotation ? _cameraAngleDelta : -_cameraAngleDelta, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y axis
        }
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);    // Camera target (where the camera is looking)
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);         // Up vector
        _cameraPosition = glm::vec3(cameraMatrix * glm::vec4(_cameraPosition, 1.));

        // Create a new view matrix
        _viewMatrix = glm::lookAt(_cameraPosition, cameraTarget, cameraUp);


        return true;

    }

    void onDraw(const gui::Rect& rect) override
    {

        // Render to depth framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, _shadowMapFBO);
        gui::gl::Context::clear(td::ColorID::Transparent);
        gui::gl::Context::clear(gui::gl::Context::Clear::Depth);

        _depthPass.activate();
        _depthPass.setDepthMVP(_depthMVP);
        _depthPass.execute();

        //draw second cube, acting as floor
        glm::mat4 floorModelMatrix = glm::mat4(1.0f);
        floorModelMatrix = glm::translate(floorModelMatrix, glm::vec3(0.0, -1.5, 0.0));
        floorModelMatrix = glm::scale(floorModelMatrix, glm::vec3(20.0, 0.1, 20.0));
        glm::mat4 depthMVPFloor = _depthProjectionMatrix * _depthViewMatrix * floorModelMatrix;
        //No need to draw back plane in Depth Buffer
//        _depthPass.setDepthMVP(depthMVPFloor);
//        _depthPass.execute();
//        _depthPass.deActivate();
        //depth pass is done


//        glBindFramebuffer(GL_FRAMEBUFFER, _screenFBO);
        activateScreenFB();
        
        gui::gl::Context::clear(td::ColorID::LightBlue);
        gui::gl::Context::clear({gui::gl::Context::Clear::Color, gui::gl::Context::Clear::Depth });
        

        
        _mainPass.activate();

        glm::mat4 depthBiasMVP = _biasMatrix * _depthMVP;
        _mainPass.setDepthBiasMVP(depthBiasMVP);

        _mainPass.setMVP(_mvpMatrix);
        _mainPass.setMV(_modelViewMat);
        _mainPass.setN(_normalMat);
        _mainPass.setLight(_lightDir, _sunColor);
        _mainPass.setV(_viewMatrix);
        _mainPass.setDepthTexture(_depthTextureID);
        _mainPass.setMainTexture(0);
        _mainPass.execute();

        //draw second cube, acting as floor
        glm::mat4 modelViewMat = _viewMatrix * floorModelMatrix;
        glm::mat4 normalMat = glm::transpose(glm::inverse(floorModelMatrix));
        glm::mat4 mvpMatrix = _perspectiveMatrix * _viewMatrix * floorModelMatrix;
        depthBiasMVP = _biasMatrix * depthMVPFloor;

        _mainPass.setDepthBiasMVP(depthBiasMVP);
        _mainPass.setMVP(mvpMatrix);
        _mainPass.setMV(modelViewMat);
        _mainPass.setN(normalMat);
        _mainPass.setLight(_lightDir, _sunColor);
        _mainPass.setV(_viewMatrix);
        _mainPass.setMainTexture(1);
        _mainPass.execute();

        _mainPass.deActivate();
     
    }
    
    bool onKeyPressed(const gui::Key& key) override
    {

        if (key.getVirtual() == gui::Key::Virtual::Left)
        {
            _lightDir.x += 0.1f;
            _depthViewMatrix = glm::lookAt(_lightDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        }

        if (key.getVirtual() == gui::Key::Virtual::Right)
        {
            _lightDir.x -= 0.1f;
            _depthViewMatrix = glm::lookAt(_lightDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        }

        if (key.getVirtual() == gui::Key::Virtual::Up)
        {
            _lightDir.y += 0.1f;
            _depthViewMatrix = glm::lookAt(_lightDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        }

        if (key.getVirtual() == gui::Key::Virtual::Down)
        {
            _lightDir.y -= 0.1f;
            _depthViewMatrix = glm::lookAt(_lightDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        }


        auto pressedKey = td::toLower(key.getChar());
        if (pressedKey == ' ')
            _cameraRotating = !_cameraRotating;

        if (pressedKey == 'a')
        {
            _ccwRotation = false;
        }
        if (pressedKey == 'd')
        {
            _ccwRotation = true;
        }
        if (pressedKey == 'w')
        {
            _cameraMoveUp = true;
        }
        if (pressedKey == 's')
        {
            _cameraMoveDown = true;
        }

        return true;
    }
 

public:
    MainView()
    : _samplerWood(":wood_floor")
    , _samplerFloor(":wood_floor_NRM")
    , _biasMatrix(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
        )
    , _size(800,600)
    {

        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes, gui::gl::DepthBuffer::Size::B3, gui::gl::StencilBuffer::Size::B1);
        createContext(reqContext, {gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks });

    }
    
    ~MainView()
    {
        makeCurrentContext();
        if (_depthTextureID != 0)
        {
            glDeleteTextures(1, &_depthTextureID);
        }
        
        if (_shadowMapFBO != 0)
        {
            glDeleteFramebuffers(1, &_shadowMapFBO);
        }
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
