//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/OrthoView.h>
#include <gui/Key.h>
#include <gui/Texture.h>
#include <gui/FileDialog.h>
#include <gui/gl/ObjectGenerator.h>

//Demonstrates:
// - orthogonal transformation
// - projection (model-view-projection) matrix in vertex shader
// - usage logical pixels instead of normalized coordinates
// - multiple primitives in a vbo
// - usage of indices
// - Y axis inversion (fix to TL or BL point on resize)
// - drag, scroll and zoom
// - export frame buffer to png/jpg

class ViewGLTexture : public gui::gl::OrthoView
{
    using Base = gui::gl::OrthoView;
    gui::Texture _frameTexture; //for frame export
    gui::Texture _samplerTextureBrick; //to show in view
    gui::Texture _samplerTextureNature; //to show in view
    GLuint _textureIDs[2];  //two textures
    
    GLuint _shaderProgram = 0;
    GLuint _vao = 0;
    GLuint _vbo = 0;
    GLuint _mvpLoc = 0;
    GLuint _textureScaleLoc = 0;
    GLuint _textureSamplerLoc = 0;
    float _textureScale = 1.0f;
    const int _nRectPoints = 4;
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
        _textureScaleLoc = glGetUniformLocation(_shaderProgram, "textureScale");
        assert(_textureScaleLoc >= 0);
        if (_textureScaleLoc < 0)
            return false;
        dbgCheckGLError();
        return true;
    }
    
    void loadTexture()
    {
        // Load texture
        glGenTextures(2, _textureIDs); //two textures
        //Prepare brickWall
        glBindTexture(GL_TEXTURE_2D, _textureIDs[0]);

        int width = _samplerTextureBrick.getWidth();
        int height = _samplerTextureBrick.getHeight();
        auto texBytes = _samplerTextureBrick.getBytes();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texBytes);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        //Prepare nature
        glBindTexture(GL_TEXTURE_2D, _textureIDs[1]);

        width = _samplerTextureNature.getWidth();
        height = _samplerTextureNature.getHeight();
        texBytes = _samplerTextureNature.getBytes();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texBytes);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    //setup data and send it to GPU
    void setupVBO()
    {
        gui::gl::Point2d rectVertices[] =
        {
            //x     y
            {10.0f, 10.0f},
            {300.0f, 10.0f},
            {300.0f,  150.0f},
            {10.0f,  150.0f} //end of rect data
        };
        
        gui::gl::Point2d  textureCoords[] =
        {
            {0.0f, 0.0f},  // Texture coordinate for Vertex 1
            {1.0f, 0.0f},  // Texture coordinate for Vertex 2
            {1.0f, 1.0f},   // Texture coordinate for Vertex 3
            {0.0f, 1.0f}   // Texture coordinate for Vertex 4
        };
        
        //prepare data in CPU memory
        cnt::PushBackVector<gui::gl::Point2d> vertexPoints;
        vertexPoints.reserve(1024); //a bit more
        vertexPoints.push_back(rectVertices, _nRectPoints);
        
        cnt::PushBackVector<gui::gl::Point2d> textureCoordinates;
        textureCoordinates.reserve(1024); //a bit more
        textureCoordinates.push_back(textureCoords, _nRectPoints);

        cnt::PushBackVector<gui::gl::Point2d> symPolygonVertices;
        symPolygonVertices.reserve(64); //a bit more than needed
        cnt::PushBackVector<gui::gl::Point2d> symPolygonTextureCoords;
        symPolygonTextureCoords.reserve(64); //a bit more than needed
        
        gui::gl::Circle c(gui::gl::Point2d(300, -300), 90); //upper rigth
        gui::gl::generateSymPolygon(c, 4, symPolygonVertices, math::FLT_PI/4, &symPolygonTextureCoords); //square
        vertexPoints.push_back(symPolygonVertices);
        textureCoordinates.push_back(symPolygonTextureCoords);
        
        c.center.x = -300; //upper left
        gui::gl::generateSymPolygon(c, 5, symPolygonVertices, 0.0f, &symPolygonTextureCoords); //pentagon
        vertexPoints.push_back(symPolygonVertices);
        textureCoordinates.push_back(symPolygonTextureCoords);
        
        c.center.y = 300; //bottom left
        gui::gl::generateSymPolygon(c, 6, symPolygonVertices, 0.0f, &symPolygonTextureCoords); //hexagon
        vertexPoints.push_back(symPolygonVertices);
        textureCoordinates.push_back(symPolygonTextureCoords);
        
        //bottom rigth
        c.center.x = 300;
        c.center.y = 300;
        gui::gl::generateSymPolygon(c, 360, symPolygonVertices, 0.0f, &symPolygonTextureCoords); //simulate circle with 300 pts
        vertexPoints.push_back(symPolygonVertices);
        textureCoordinates.push_back(symPolygonTextureCoords);
        
        auto nVertices = vertexPoints.size();
        auto nTextCoords = textureCoordinates.size();
        assert(nVertices == nTextCoords);
        // ODAVDE - NOVO
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        glGenBuffers(1, &_vbo);

        // Bind VBO and copy data
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        size_t nVertexBytes = vertexPoints.size() * sizeof(gui::gl::Point2d);
        size_t nTextureBytes = textureCoordinates.size() * sizeof(gui::gl::Point2d);
        
        //allocate GPU buffer for both (vertex and texture coordinates)
        glBufferData(GL_ARRAY_BUFFER, nVertexBytes + nTextureBytes, NULL, GL_STATIC_DRAW);
        //copy vertex coordinates to buffer into the first part of the vbo
        glBufferSubData(GL_ARRAY_BUFFER, 0, nVertexBytes, vertexPoints.begin());
        //copy vertex coordinates to buffer into the second part of the vbo
        glBufferSubData(GL_ARRAY_BUFFER, nVertexBytes, nTextureBytes, textureCoordinates.begin());

        // Set up vertex attribute pointers
        int layoutVertexXY = 0; //first input param for vertex shader
        glVertexAttribPointer(layoutVertexXY, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(layoutVertexXY);
        
        int layoutTexXY = 1; //Second input param for vertex shader
        size_t offsetToTextureData = sizeof(gui::gl::Point2d) * vertexPoints.size();
        glVertexAttribPointer(layoutTexXY, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)(offsetToTextureData));
        glEnableVertexAttribArray(layoutTexXY);
        
        // Unbind VAO
        glBindVertexArray(0);

        dbgCheckGLError();
    }
protected:
    
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
        loadTexture();
        
        dbgCheckGLError();
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear OPENGL BUFFERS

        // Use the shader program
        glUseProgram(_shaderProgram);
        
        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _textureIDs[_usedTexture]);

        // Set up uniforms (model matrix and texture scale and texureSamples)
        const auto& projectionMatrix = getProjection();
        glUniformMatrix4fv(_mvpLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix)); //mvp
        glUniform1f(_textureScaleLoc, 1.0f); //texture scale

        // Use the VAO
        glBindVertexArray(_vao);

        //Draw with indices
        //draw rectangle using indices
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); //rect
        
        glUniform1f(_textureScaleLoc, _textureScale);
        
        auto tr = glm::translate(projectionMatrix, glm::vec3(0, -200, 0));
        glUniformMatrix4fv(_mvpLoc, 1, GL_FALSE, glm::value_ptr(tr));
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); //rect translate
        
        glUniformMatrix4fv(_mvpLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        int startIndex = _nRectPoints;
        glDrawArrays(GL_TRIANGLE_FAN, startIndex, 4); //quad
        startIndex += 4;
        glDrawArrays(GL_TRIANGLE_FAN, startIndex, 5); //pentagon
        startIndex += 5;
        glDrawArrays(GL_TRIANGLE_FAN, startIndex, 6); //hexagon
        startIndex += 6;
        glDrawArrays(GL_TRIANGLE_FAN, startIndex, 360); //circle
        
        // Unbind VAO and shader program
        glBindVertexArray(0);
        glUseProgram(0);
    }
    
    void increaseZoom()
    {
        auto zoomFactor = getZoom();
        if (zoomFactor >= 2.0f)
            return;
        zoomFactor += 0.1f;
        setZoom(zoomFactor);
        reDraw();
    }
    
    void decreaseZoom()
    {
        auto zoomFactor = getZoom();
        if (zoomFactor <= 0.11f)
            return;
        zoomFactor -= 0.1f;
        setZoom(zoomFactor);
        reDraw();
    }

    bool onKeyPressed(const gui::Key& key) override
    {
        //Check if Base has something to do on this key
        if (Base::onKeyPressed(key))
            return true;
        //otherwise, do my part
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
        //call base impelementation first
        Base::onPrimaryButtonPressed(inputDevice);
        //then do my part
        if (!inputDevice.isAnyModifierPressed())
        {
//            increaseZoom();
            auto framePt = inputDevice.getFramePoint();
            auto modelPt = getModelPoint(framePt);
            mu::dbgLog("LeftFC(%.1f, %.1f), ModelC(%.1f, %.1f)", framePt.x, framePt.y, modelPt.x, modelPt.y);
        }
    }
    
    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        //call base impelementation first
        Base::onSecondaryButtonPressed(inputDevice);
        //then do my part
        if (!inputDevice.isAnyModifierPressed())
        {
//            decreaseZoom();
            auto framePt = inputDevice.getFramePoint();
            auto modelPt = getModelPoint(framePt);
            mu::dbgLog("RighFC(%.1f, %.1f), ModelC(%.1f, %.1f)", framePt.x, framePt.y, modelPt.x, modelPt.y);
        }
    }
    
    void onResize(const gui::Size& newSize) override
    {
        mu::dbgLog("New size=(%.1fx%.1f", newSize.width, newSize.height);
        gui::gl::OrthoView::onResize(newSize);
    }
    
public:
    ViewGLTexture()
    : _samplerTextureBrick(":brickWall")
    , _samplerTextureNature(":nature")
    {
        //do not invert Y axis
        createContext(false);
        registerForFocusEvents();
//        enableResizeEvent();
    }
    
    ~ViewGLTexture()
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
        glDeleteTextures(2, _textureIDs);
        if (_vao)
        {
            glDeleteVertexArrays(1, &_vao);
            _vao = 0;
        }
    }
    
    void updateTextScale(float val)
    {
        _textureScale = val;
        reDraw();
    }
    
    void switchTexture()
    {
        if (_usedTexture == 0)
            _usedTexture = 1;
        else
            _usedTexture = 0;
        reDraw();
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
