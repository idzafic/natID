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
// - all in one textures
// - animation 3D mode


class ViewGLAnimation3DTexture : public gui::gl::View
{
    using Base = gui::gl::View;
    gui::Texture _frameTexture; //for frame export
    gui::Texture _samplerDiceWithNumbers; //to map on cube
    gui::Texture _samplerDiceWithDots; //to map on cube
    glm::mat4 _perspectiveMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _mvpMatrix;
    GLuint _textureIDs[2];  //two textures
    
    float _angleX = 0;
    float _angleY = 0;
    float _dAngle = 0.001f;
    
    GLuint _shaderProgram = 0;
    GLuint _vao = 0;
    GLuint _vboVertices = 0;
    GLuint _ebo = 0;
    GLuint _mvpLoc = 0;
    GLuint _textureSamplerLoc = 0;
    
    const unsigned int triangles = 6 * 2;   // Number of triangles rendered
    td::BYTE _usedTexture = 0; //0 or 1
private:
    bool setupShaders()
    {
        _shaderProgram = linkProgram(":shVert", ":shFrag");
        if (_shaderProgram == 0)
            return false;
        //all ok, get index of model variable for faster access
        _mvpLoc = glGetUniformLocation(_shaderProgram, "mvp");
        assert(_mvpLoc >= 0);
        if (_mvpLoc < 0)
            return false;
        _textureSamplerLoc = glGetUniformLocation(_shaderProgram, "textureSampler");
        assert(_textureSamplerLoc >= 0);
        if (_textureSamplerLoc < 0)
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
        // Define the cube's vertices and texture coordinates
        float vertices[] = {
            // Front face (number 1)
            0.5f,   0.5f,  0.5f,  1.0f / 6.0f, 1.0f,    // Vertex 0 (top-right)
           -0.5f,   0.5f,  0.5f,  0.0f, 1.0f,           // Vertex 1 (top-left)
           -0.5f,  -0.5f,  0.5f,  0.0f, 0.0f,           // Vertex 2 (bottom-left)
            0.5f,  -0.5f,  0.5f,  1.0f / 6.0f, 0.0f,      // Vertex 3 (bottom-right)

            // Right face (number 2)
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
        
        unsigned short indices[] =
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

        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        glGenBuffers(1, &_vboVertices);
        glBindBuffer(GL_ARRAY_BUFFER, _vboVertices);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        // Bind EBO and copy index data to it
        glGenBuffers(1, &_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        //layouts
        int vertexLayoutLocation = 0;
        int colorLayoutLocation = 1;
        // Set vertex attribute pointers
        glVertexAttribPointer(vertexLayoutLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(vertexLayoutLocation);

        glVertexAttribPointer(colorLayoutLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(colorLayoutLocation);

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
    
    void loadTextures()
    {
        // Load texture
        glGenTextures(2, _textureIDs); //two textures
        //Prepare dice with numbers
        glBindTexture(GL_TEXTURE_2D, _textureIDs[0]);

        int width = _samplerDiceWithNumbers.getWidth();
        int height = _samplerDiceWithNumbers.getHeight();
        auto texBytes = _samplerDiceWithNumbers.getBytes();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texBytes);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        //Prepare dice with dots
        glBindTexture(GL_TEXTURE_2D, _textureIDs[1]);

        width = _samplerDiceWithDots.getWidth();
        height = _samplerDiceWithDots.getHeight();
        texBytes = _samplerDiceWithDots.getBytes();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texBytes);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    void onInit() override
    {
        auto [major, minor] = getOpenGLVersion();
        mu::dbgLog("Used opengl %d.%d", major, minor);
        // Initialize OpenGL settings
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (!setupShaders())
        {
            mu::dbgLog("ERROR! OpenGL cannnot setup shaders!");
            assert(false);
            return;
        }
        setupVBO();
        loadTextures();
        
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
//        _angleX += _dAngle;
        _angleY += _dAngle;
        // Create a model matrix with rotations around X and Y axes
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::rotate(modelMatrix, -_angleX, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X axis
        modelMatrix = glm::rotate(modelMatrix, -_angleY, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y axis

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
        
        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _textureIDs[_usedTexture]);
        glUniform1i(_textureSamplerLoc, 0);

        glUniformMatrix4fv(_mvpLoc, 1, GL_FALSE, glm::value_ptr(_mvpMatrix));

        glBindVertexArray(_vao);
        glDrawElements(GL_TRIANGLES, triangles * 3, GL_UNSIGNED_SHORT, NULL);

        // Unbind VAO and shader program
        glBindVertexArray(0);
        glUseProgram(0);
    }

public:
    ViewGLAnimation3DTexture()
    : _samplerDiceWithNumbers(":numbers")
    , _samplerDiceWithDots(":dots")
    {
        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes, gui::gl::DepthBuffer::Size::B2);
        createContext(reqContext, {gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks });
    }
    
    ~ViewGLAnimation3DTexture()
    {
        makeCurrentContext();
        // Delete OpenGL program
        if (_shaderProgram)
            glDeleteProgram(_shaderProgram);
        // Delete OpenGL buffers
        if (_vboVertices)
            glDeleteBuffers(1, &_vboVertices);
        
        if (_ebo)
            glDeleteBuffers(1, &_ebo);
        glDeleteTextures(2, _textureIDs);
        if (_vao)
            glDeleteVertexArrays(1, &_vao);
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
