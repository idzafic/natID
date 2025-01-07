//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/View.h>
#include <gui/Key.h>
#include <gui/Texture.h>
#include <gui/FileDialog.h>
#include <glm/gtc/matrix_transform.hpp>

//Demonstrates:
// - animation 3D mode

//#define DEBUG_GL

class ViewGLAnimation : public gui::gl::View
{
    using Base = gui::gl::View;
    glm::mat4 _perspectiveMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _mvpMatrix;
    
    float _angleX = 0;
    float _angleY = 0;
    
    GLuint _shaderProgram = 0;
    GLuint _vao = 0;
    GLuint _vbo = 0;
    GLuint _ebo = 0;
    GLuint _mvpLoc = 0;
    
    const unsigned int triangles = 6 * 2;   // Number of triangles rendered
    //shader layout
    const unsigned int vertexIndex = 0;
    const unsigned int colorIndex = 1;
private:
    bool setupShaders()
    {
//        mu::ILogger* pLog = mu::pApp()->getLogger();
//        if (pLog)
//            pLog->showInfo("setupShaders", "Pos1");
        
        _shaderProgram = linkProgram(":shVert", ":shFrag");
        if (_shaderProgram == 0)
            return false;
        //all ok, get index of model variable for faster access
        _mvpLoc = glGetUniformLocation(_shaderProgram, "mvp");
        assert(_mvpLoc >= 0);
        if (_mvpLoc < 0)
            return false;
#ifdef DEBUG_GL
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            if (pLog)
                pLog->showInfo("setupShaders", "ERROR! OpenGL error in setupVBO");
            
            mu::dbgLog("ERROR! OpenGL error in setupVBO! Error code = %x", error);
            return false;
        }
#endif
//        if (pLog)
//            pLog->showInfo("setupShaders", "Pos10");
        return true;
    }
    
    
    //setup data and send it to GPU
    void setupVBO()
    {
        // Define the cube's vertices and colors
        float vertices[] = {
            // Position           // Color
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Vertex 0: Red
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, // Vertex 1: Green
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, // Vertex 2: Blue
             0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, // Vertex 3: Yellow

             0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f, // Vertex 4: Magenta
            -0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f, // Vertex 5: Gray
            -0.5f, -0.5f, -0.5f,  0.8f, 0.2f, 0.4f, // Vertex 6: Rose
             0.5f, -0.5f, -0.5f,  0.2f, 0.8f, 0.4f  // Vertex 7: Mint
        };

        unsigned short indices[] = {
          // Front
          0, 1, 2,
          2, 3, 0,

          // Right
          0, 3, 7,
          7, 4, 0,

          // Bottom
          2, 6, 7,
          7, 3, 2,

          // Left
          1, 5, 6,
          6, 2, 1,

          // Back
          4, 7, 6,
          6, 5, 4,

          // Top
          5, 1, 0,
          0, 4, 5,
        };

        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_ebo);
        glBindVertexArray(_vao);
        
        // Bind VBO and copy vertex data to it
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Bind EBO and copy index data to it
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Set vertex attribute pointers
        glVertexAttribPointer(vertexIndex, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(vertexIndex);

        glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(colorIndex);

        // Unbind VAO, VBO, and EBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
#ifdef DEBUG_GL
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            mu::ILogger* pLog = mu::pApp()->getLogger();
            if (pLog)
                pLog->showError("ViewGLAnimation::onInit", "Error! Cannot setup shaders!");
            mu::dbgLog("ERROR! OpenGL error in setupVBO! Error code = %x", error);
            assert(false);
        }
#endif
    }
protected:
    
    void onInit() override
    {
        mu::ILogger* pLog = mu::pApp()->getLogger();
        if (pLog)
            pLog->showInfo("ViewGLAnimation::onInit", "Pos0");
        
//        setAnimation(true);
        auto [major, minor] = getOpenGLVersion();
        mu::dbgLog("INFO: Used OpenGL ver. %d.%d", major, minor);
        // Initialize OpenGL settings
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (!setupShaders())
        {
            if (pLog)
                pLog->showError("ViewGLAnimation::onInit", "Error! Cannot setup shaders!");
            
            mu::dbgLog("ERROR! OpenGL cannnot setup shaders!");
            assert(false);
            return;
        }
        setupVBO();
        
        // Set up the perspective parameters
        float fov = 45.0f; // Field of view in degrees
        float aspectRatio = 16.0f / 9.0f; // Aspect ratio of the viewport
        float nearClip = 0.1f; // Near clipping plane (relative to camera)
        float farClip = 100.0f; // Far clipping plane (relative to camera)

        // Create a perspective matrix
        _perspectiveMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        
        // Camera parameters
        glm::vec3 cameraPosition = glm::vec3(0, 0, 3.0f);  // New camera position
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.5f);    // Camera target (where the camera is looking)
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);         // Up vector

        // Create a new view matrix
        _viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
        
        // Calculate MVP matrix using identity model matrix
        _mvpMatrix = _perspectiveMatrix * _viewMatrix; //* I for model
        
        glEnable(GL_DEPTH_TEST);
        // Set face culling winding order
//        glEnable(GL_CULL_FACE);
        
#ifdef DEBUG_GL
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            if (pLog)
                pLog->showError("setupShaders", "ERROR! OpenGL error in onInit last pos");
            mu::dbgLog("OpenGL error: %x", error);
        }
#endif
    }
    
    bool prepareNextFrame() override
    {
        _angleX += 0.01f;
        _angleY += 0.01f;
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
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(_shaderProgram);
        
        glUniformMatrix4fv(_mvpLoc, 1, GL_FALSE, glm::value_ptr(_mvpMatrix));

        glBindVertexArray(_vao);
        glDrawElements(GL_TRIANGLES, triangles * 3, GL_UNSIGNED_SHORT, NULL);

        // Unbind VAO and shader program
        glBindVertexArray(0);
        glUseProgram(0);
    }
    
public:
    ViewGLAnimation()
    {
        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes, gui::gl::DepthBuffer::Size::B2);
        createContext(reqContext, {gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks });
    }
    
    ~ViewGLAnimation()
    {
        makeCurrentContext();
        // Delete OpenGL program
        if (_shaderProgram)
        {
            glDeleteProgram(_shaderProgram);
            _shaderProgram = 0;
        }
        // Delete OpenGL buffers
        if (_vbo)
        {
            glDeleteBuffers(1, &_vbo);
            _vbo = 0;
        }
        
        if (_ebo)
        {
            glDeleteBuffers(1, &_ebo);
            _ebo = 0;
        }
        
        if (_vao)
        {
            glDeleteVertexArrays(1, &_vao);
            _vao = 0;
        }
    }
};
