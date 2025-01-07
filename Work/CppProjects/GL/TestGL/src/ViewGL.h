//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/View.h>
#define DEBUG_GL

class ViewGL : public gui::gl::View
{
    GLuint _shaderProgram = 0;
    GLuint _vertexArrayObject = 0;
    GLuint _vertexBufferObject = 0;
    GLint _zoomLocation = 0;
    float _zoomFactor = 1.0f;
private:
    bool setupShaders()
    {
        _shaderProgram = linkProgram(":shVert", ":shFrag");
        if (_shaderProgram == 0)
            return false;
        //all ok, get index of zoomFactor variable for faster access
        _zoomLocation = glGetUniformLocation(_shaderProgram, "zoomFactor");
        assert(_zoomLocation >= 0);
        if (_zoomLocation < 0)
            return false;
        return true;
    }
    //setup data and send it to GPU
    void setupVBO()
    {
        // Vertex data for a simple rectangle with colors
        GLfloat vertices[] = 
        {
            //x        y     R      G     B  (A is set to 1.0 in fragment shader)
            -0.5f,   0.5f,  1.0f, 0.0f, 0.0f, // Red color (rectangle)
             0.5f,   0.5f,  0.0f, 1.0f, 0.0f, // Green color (rectangle)
             0.5f,  -0.5f,  0.0f, 0.0f, 1.0f, // Blue color (rectangle)
            -0.5f,  -0.5f,  1.0f, 1.0f, 0.0f,  // Yellow color (end of rectangle)
            -1.0f,   0.0f,  1.0f, 1.0f, 1.0f,  // White color (horizontal axis)
             1.0f,   0.0f,  1.0f, 1.0f, 1.0f,  // White color (horizontal axis)
             0.0f,  -1.0f,  1.0f, 1.0f, 1.0f,  // White color (vertical axis)
             0.0f,   1.0f,  1.0f, 1.0f, 1.0f,  // White color (vertical axis)
            -0.98f, -0.98f, 0.0f, 1.0f, 1.0f,  // Turquoise color (Bounding box)
             0.98f, -0.98f, 0.0f, 1.0f, 1.0f,  // Turquoise color (Bounding box)
             0.98f,  0.98f, 0.0f, 1.0f, 1.0f,  // Turquoise color (Bounding box)
            -0.98f,  0.98f, 0.0f, 1.0f, 1.0f  // Turquoise color (Bounding box)
        };
        
        glGenVertexArrays(1, &_vertexArrayObject);
        glBindVertexArray(_vertexArrayObject);
        
        glGenBuffers(1, &_vertexBufferObject);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        // Specify the layout of the vertex data
        GLint positionAttrib = glGetAttribLocation(_shaderProgram, "position");
        glVertexAttribPointer(positionAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(positionAttrib);
        
        GLint colorAttrib = glGetAttribLocation(_shaderProgram, "color");
        glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
        glEnableVertexAttribArray(colorAttrib);
        
        // Unbind VAO and VBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //check error
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
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (!setupShaders())
        {
            mu::dbgLog("ERROR! OpenGL cannnot setup shaders!");
            assert(false);
            return;
        }
        setupVBO();
#ifdef DEBUG_GL
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            mu::dbgLog("OpenGL error: %x", error);
        }
#endif
    }
    
    void onDraw(const gui::Rect& rect) override
    {
//        static int drawNo = 1;
//        mu::dbgLog("onDraw no = %d", drawNo++);
        // Clear the background
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //clean with black
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Use shader program
        glUseProgram(_shaderProgram);
        // Set zoom factor
        glUniform1f(_zoomLocation, _zoomFactor);
        // Bind VAO (use same buffer to draw all elements
        glBindVertexArray(_vertexArrayObject);
        // Draw rectangle by drawing triangles
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); //draw rectangle
        //Draw horizonal and vertical axis
        glDrawArrays(GL_LINES, 4, 4); //draw horizontal vertical axis
        //Draw bounding rectangle
        glDrawArrays(GL_LINE_LOOP, 8, 4); //draw horizontal vertical axis
        
        // Unbind VAO
        glBindVertexArray(0);
        // Unuse shader program
        glUseProgram(0);
#ifdef DEBUG_GL
        //check error
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) 
        {
            mu::dbgLog("ERROR! OpenGL error: %x", error);
            assert(false);
        }
#endif
    }
    
    void increaseZoom()
    {
        if (_zoomFactor >= 2.0f)
            return;
        _zoomFactor += 0.1f;
        reDraw();
    }
    
    void decreaseZoom()
    {
        if (_zoomFactor <= 0.11f)
            return;
        _zoomFactor -= 0.1f;
        reDraw();
    }
    
    bool onKeyPressed(const gui::Key& key) override
    {
        if (key.isASCII())
        {
            char ch = key.getChar();
            if (ch == 'G' || ch == 'g')
            {
                increaseZoom();
                return true;
            }
            else if (ch == 'H' || ch == 'h')
            {
                decreaseZoom();
                return true;
            }
        }
        return false;
    }
    
    void onPrimaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        increaseZoom();
        auto pt = inputDevice.getFramePoint();
        mu::dbgLog("LeftMC(%.1f, %.1f)", pt.x, pt.y);
    }
    
    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        decreaseZoom();
        auto pt = inputDevice.getFramePoint();
        mu::dbgLog("RighMC(%.1f, %.1f)", pt.x, pt.y);
    }
public:
    ViewGL()
    {
        gui::gl::Context reqContext;
        createContext(reqContext, {gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks });
    }
    
    ~ViewGL()
    {
        makeCurrentContext();
        // Delete OpenGL program
        if (_shaderProgram)
        {
            glDeleteProgram(_shaderProgram);
            _shaderProgram = 0;
        }
        // Delete OpenGL buffers
        if (_vertexArrayObject)
        {
            glDeleteVertexArrays(1, &_vertexArrayObject);
            _vertexArrayObject = 0;
        }
        if (_vertexBufferObject)
        {
            glDeleteBuffers(1, &_vertexBufferObject);
            _vertexBufferObject = 0;
        }
    }
};
