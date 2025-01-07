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
// - different buffers for xy and color
// - flat color mode


class ViewGLAnimationFlat : public gui::gl::View
{
    using Base = gui::gl::View;
    glm::mat4 _perspectiveMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _mvpMatrix;
    
    float _angleX = 0;
    float _angleY = 0;
    
    GLuint _shaderProgramFlat = 0;
    GLuint _shaderProgramInt = 0;
    GLuint _vao = 0;
    GLuint _vboVertices = 0;
    GLuint _vboColors = 0;
    GLuint _ebo = 0;
    GLuint _mvpLoc = 0;
    
    const unsigned int triangles = 6 * 2;   // Number of triangles rendered
    //shader layout
    const unsigned int vertexLayoutPos = 0;
    const unsigned int colorLayoutPos = 1;
    bool _bFlat = true;
private:
    bool setupShaders()
    {
        _shaderProgramFlat = linkProgram(":shVertFlat", ":shFragFlat");
        if (_shaderProgramFlat == 0)
            return false;
        
        _shaderProgramInt = linkProgram(":shVertInt", ":shFragInt");
        if (_shaderProgramInt == 0)
            return false;
        
        //all ok, get index of model variable for faster access
        _mvpLoc = glGetUniformLocation(_shaderProgramFlat, "mvp");
        assert(_mvpLoc >= 0);
        if (_mvpLoc < 0)
            return false;
#ifdef DEBUG_GL
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            mu::dbgLog("ERROR! OpenGL error in setupVBO! Error code = %x", error);
            return false;
        }
#endif
        return true;
    }
    
    
    //setup data and send it to GPU
    void setupVBO()
    {
        float vertices[] = 
        {
            // Front face
             0.5,  0.5,  0.5,
            -0.5,  0.5,  0.5,
            -0.5, -0.5,  0.5,
             0.5, -0.5,  0.5,

            // Back face
             0.5,  0.5, -0.5,
            -0.5,  0.5, -0.5,
            -0.5, -0.5, -0.5,
             0.5, -0.5, -0.5,
        };

        float vertexColors[] = 
        {
            1.0, 0.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 0.0, 1.0,
            1.0, 1.0, 0.0,

            0.0, 1.0, 1.0,
            1.0, 0.0, 1.0,
            1.0, 1.0, 1.0,
            0.5, 0.5, 0.5,
        };

        unsigned short triangleIndices[] = {
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
        glBindVertexArray(_vao);

        glGenBuffers(1, &_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);

        glGenBuffers(1, &_vboVertices);
        glBindBuffer(GL_ARRAY_BUFFER, _vboVertices);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(vertexLayoutPos, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(vertexLayoutPos);

        glGenBuffers(1, &_vboColors);
        glBindBuffer(GL_ARRAY_BUFFER, _vboColors);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);

        glVertexAttribPointer(colorLayoutPos, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(colorLayoutPos);

        // Unbind to prevent accidental modification
        glBindVertexArray(0);
    
#ifdef DEBUG_GL
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            mu::dbgLog("ERROR! OpenGL error in setupVBO! Error code = %x", error);
            assert(false);
        }
#endif
    }
protected:
    
    void onInit() override
    {
        auto [major, minor] = getOpenGLVersion();
        mu::dbgLog("Used opengl %d.%d", major, minor);
        // Initialize OpenGL settings
        if (!setupShaders())
        {
            mu::dbgLog("ERROR! OpenGL cannnot setup shaders!");
            assert(false);
            return;
        }
        setupVBO();
        
        // Set up the perspective parameters
        float fov = 45.0f; // Field of view in degrees
        float aspectRatio = 16.0f / 9.0f; // Aspect ratio of the viewport
        float nearClip = 0.1f; // Near clipping plane
        float farClip = 100.0f; // Far clipping plane

        // Create a perspective matrix
        _perspectiveMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        
        // Camera parameters
        glm::vec3 cameraPosition = glm::vec3(0, 0, 3.0f);  // New camera position
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.5f);    // Camera target (where the camera is looking)
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);         // Up vector

        // Create a new view matrix
        _viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
        
        _mvpMatrix = _perspectiveMatrix * _viewMatrix; //* I for model
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        
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

        if (_bFlat)
            glUseProgram(_shaderProgramFlat);
        else
            glUseProgram(_shaderProgramInt);

        
        glUniformMatrix4fv(_mvpLoc, 1, GL_FALSE, glm::value_ptr(_mvpMatrix));

        glBindVertexArray(_vao);
        glDrawElements(GL_TRIANGLES, triangles * 3, GL_UNSIGNED_SHORT, NULL);

        // Unbind VAO and shader program
        glBindVertexArray(0);
        glUseProgram(0);
    }

    
public:
    ViewGLAnimationFlat()
    {
        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes, gui::gl::DepthBuffer::Size::B2);
        createContext(reqContext, {gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks });
        
    }
    
    ~ViewGLAnimationFlat()
    {
        makeCurrentContext();
        // Delete OpenGL program
        if (_shaderProgramFlat)
        {
            glDeleteProgram(_shaderProgramFlat);
            _shaderProgramFlat = 0;
        }
        // Delete OpenGL buffers
        if (_vboVertices)
        {
            glDeleteBuffers(1, &_vboVertices);
            _vboVertices = 0;
        }
        
        if (_vboColors)
        {
            glDeleteBuffers(1, &_vboColors);
            _vboColors = 0;
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
    
    void switchColorShadingMode()
    {
        _bFlat = !_bFlat;
        //no need to call reDraw since we are in animation mode
    }
};
