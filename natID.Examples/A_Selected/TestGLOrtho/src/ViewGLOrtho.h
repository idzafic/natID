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
#include "StatusBar.h"
//Demonstrates:
// - orthogonal transformation
// - projection (model-view-projection) matrix in vertex shader
// - usage logical pixels instead of normalized coordinates
// - multiple primitives in a vbo
// - usage of indices
// - Y axis inversion (fix to TL or BL point on resize)
// - drag, scroll and zoom
// - export frame buffer to png/jpg

class ViewGLOrtho : public gui::gl::OrthoView
{
    using Base = gui::gl::OrthoView;
    StatusBar* _pStatusBar;
    gui::Texture _frameTexture; //for frame export
    GLuint _shaderProgram = 0;
    GLuint _vao = 0;
    GLuint _vbo = 0;
    GLuint _ebo = 0;
    GLint _mvpLoc = 0;
    GLint _colorLoc = 0;
    const int _nRectAndAxisPoints = 10;
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
        _colorLoc = glGetUniformLocation(_shaderProgram, "color");
        assert(_colorLoc >= 0);
        if (_colorLoc < 0)
            return false;
        dbgCheckGLError();
        return true;
    }
    //setup data and send it to GPU
    void setupVBO()
    {
        gui::gl::Point2d rectAndAxisVertices[] =
        {
            //x     y     R      G     B  (A is set to 1.0 in fragment shader)
            {10.0f, 10.0f},
            {300.0f, 10.0f},
            {300.0f,  150.0f},
            {10.0f,  150.0f}, //end of rect data
            {-10000.0f, 0}, //begin negative x axis (red) ind=4
            {0, 0},   // end negative x axis
            {10000, 0}, // end positive x axis (green)
            {0, -10000.0f}, //begin negative y axis (red)
            {0,0}, //negative y axis (red)
            {0, 10000.0f} //positive y axus (green)
        };
        
        cnt::PushBackVector<gui::gl::Point2d> vboPoints;
        vboPoints.reserve(1024); //a bit more
        vboPoints.push_back(rectAndAxisVertices, _nRectAndAxisPoints);

        GLuint rectIndices[] =
        {
            0, 1, 2, 3,  // Rect indices
            0, 2, //diagonal TL-BR
            1, 3 //diagonal TR-BL
        };
        
        cnt::PushBackVector<gui::gl::Point2d> symPolygonVertices;
        symPolygonVertices.reserve(64); //a bit more than needed
        
        gui::gl::Circle c(gui::gl::Point2d(300, -300), 90); //upper rigth
        gui::gl::generateSymPolygon(c, 4, symPolygonVertices, math::FLT_PI/4); //square
        vboPoints.push_back(symPolygonVertices);
        
        c.center.x = -300; //upper left
        gui::gl::generateSymPolygon(c, 5, symPolygonVertices); //pentagon
        vboPoints.push_back(symPolygonVertices);
        
        c.center.y = 300; //bottom left
        gui::gl::generateSymPolygon(c, 6, symPolygonVertices); //hexagon
        vboPoints.push_back(symPolygonVertices);
        
        //bottom rigth
        c.center.x = 300;
        c.center.y = 300;
        gui::gl::generateSymPolygon(c, 360, symPolygonVertices); //simulate circle with 300 pts
        vboPoints.push_back(symPolygonVertices);
        
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        size_t nVBOBytes = vboPoints.size() * sizeof(gui::gl::Point2d);
        glBufferData(GL_ARRAY_BUFFER, nVBOBytes, vboPoints.begin(), GL_STATIC_DRAW); //transfer to GPU memory
        //glBufferData(GL_ARRAY_BUFFER, sizeof(rectAndAxisVertices), rectAndAxisVertices, GL_STATIC_DRAW);

        // Specify vertex position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectIndices), rectIndices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        dbgCheckGLError();
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
    }
    
    void onDraw(const gui::Rect& rect) override
    {
//        static int drawNo = 1;
//        mu::dbgLog("onDraw no = %d", drawNo++);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader program
        glUseProgram(_shaderProgram);

        // Set up model matrix and color in the vertex shader
        const auto& projectionMatrix = getProjection();
        glUniformMatrix4fv(_mvpLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniform3f(_colorLoc, 1.0f, 0.0f, 0.0f); // Red color

        // Use the VAO
        glBindVertexArray(_vao);

        //Draw with indices
        //draw rectangle using indices
        glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, nullptr);
        
        //reuse rectangle points to draw diagonals
        //set white color in the vertex shader for bounding rect
        glUniform3f(_colorLoc, 1.0f, 1.0f, 1.0f); // white color
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, nullptr); //nullptr -> from first index
        glUniform3f(_colorLoc, 1.0f, 1.0f, 0.0f); // Yellow color
        glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT, (void*) (4*sizeof(GLuint))); //from 4-th index
        
        //Draw witout indices
        //draw negative x axis in red
        glUniform3f(_colorLoc, 1.0f, 0.0f, 0.0f); // red color
        glDrawArrays(GL_LINES, 4, 2); //draw horizontal vertical axis
        //draw positive x axis in green
        glUniform3f(_colorLoc, 0.0f, 1.0f, 0.0f); // green color
        glDrawArrays(GL_LINES, 5, 2); //draw horizontal vertical axis
        //draw negative y axis in red
        glUniform3f(_colorLoc, 1.0f, 0.0f, 0.0f); // red color
        glDrawArrays(GL_LINES, 7, 2); //draw horizontal vertical axis
        //draw positive y axis in green
        glUniform3f(_colorLoc, 0.0f, 1.0f, 0.0f); // green color
        glDrawArrays(GL_LINES, 8, 2); //draw horizontal vertical axis
        
        //draw square in yellow
        int startIndex = _nRectAndAxisPoints;
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
            //increaseZoom();
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
            //decreaseZoom();
            auto framePt = inputDevice.getFramePoint();
            auto modelPt = getModelPoint(framePt);
            mu::dbgLog("RighFC(%.1f, %.1f), ModelC(%.1f, %.1f)", framePt.x, framePt.y, modelPt.x, modelPt.y);
        }
    }
    
    void onCursorMoved(const gui::InputDevice& inputDevice) override
    {
        auto framePt = inputDevice.getFramePoint();
        auto modelPt = getModelPoint(framePt);
        _pStatusBar->setCurrentPos(modelPt);
    }
    
    void onZoomChanged() override
    {
        float zoom = (float) getZoom();
        _pStatusBar->setZoom(zoom * 100);
    }
    
    void onOriginChanged() override
    {
        const gui::Point& origin = getOrigin();
        _pStatusBar->setOrigin(origin);
    }
    
public:
    ViewGLOrtho(StatusBar* pStatusBar)
    : _pStatusBar(pStatusBar)
    {
        createContext(true);
        double zoom = getZoom();
        _pStatusBar->setZoom((float)zoom * 100);
        const gui::Point& origin = getOrigin();
        _pStatusBar->setOrigin(origin);
        
    }
    
    ~ViewGLOrtho()
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
    
    void exportFrame()
    {
        _frameTexture.setExportJPGQuality(80); //80%
        
        readFrameFrameBuffer(_frameTexture, [this]()
        {
            _frameTexture.mirrorVertically();
            
            td::UINT4 dlgID = 55;
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
//#ifdef MU_WINDOWS
//            _frameTexture.saveToFile("R:/Frame.jpg", gui::Texture::Type::JPG);
//            _frameTexture.saveToFile("R:/Frame.png", gui::Texture::Type::PNG);
//            _frameTexture.saveToFile("R:/Frame.bmp", gui::Texture::Type::BMP);
//            _frameTexture.saveToFile("R:/Frame.tga", gui::Texture::Type::TGA);
//#else
//            _frameTexture.saveToFile("/Volumes/RAMDisk/Frame.jpg", gui::Texture::Type::JPG);
//            _frameTexture.saveToFile("/Volumes/RAMDisk/Frame.png", gui::Texture::Type::PNG);
//            _frameTexture.saveToFile("/Volumes/RAMDisk/Frame.bmp", gui::Texture::Type::BMP);
//            _frameTexture.saveToFile("/Volumes/RAMDisk/Frame.tga", gui::Texture::Type::TGA);
//#endif
        
    }
};
