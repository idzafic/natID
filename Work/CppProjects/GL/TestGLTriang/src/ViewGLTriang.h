//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/OrthoView.h>
#include <gui/gl/Triang2D.h>
#include <gui/Key.h>
#include <math/math.h>
#include <gui/Texture.h>
#include <gui/FileDialog.h>
#include <gui/gl/Color.h>

class ViewGLTriang : public gui::gl::OrthoView
{
    using Base = gui::gl::OrthoView;
    gui::Texture _frameTexture; //for frame export
    GLuint _shaderProgram = 0;
    GLuint _vao = 0;
    GLuint _vbo = 0;
    GLuint _ebo = 0;
    GLint _mvpLoc = 0;
    GLint _colorLoc = 0;
    td::UINT4 _startingIndexOfE = 0;
    td::UINT4 _noOfIndicesInE=0;
    td::UINT4 _startingVertexE = 19;
    td::UINT4 _noOfVerticesE = 12;
    
    td::UINT4 _startingIndexLine1;
    td::UINT4 _nIndicesLine1;
    
    td::UINT4 _startingIndexLine2;
    td::UINT4 _nIndicesLine2;
    
    td::UINT4 _startingIndexLine3;
    td::UINT4 _nIndicesLine3;
    
    td::UINT4 _startingIndexLine4;
    td::UINT4 _nIndicesLine4;
    
    td::UINT4 _startingIndexLine5; //closed line
    td::UINT4 _nIndicesLine5;
    
    //CC-counter clockwise
    td::UINT4 _startingIndexLine1CC;
    td::UINT4 _nIndicesLine1CC;
    
    td::UINT4 _startingIndexLine2CC;
    td::UINT4 _nIndicesLine2CC;
    
    td::UINT4 _startingIndexLine3CC;
    td::UINT4 _nIndicesLine3CC;
    
    td::UINT4 _startingIndexLine4CC;
    td::UINT4 _nIndicesLine4CC;
    
    td::UINT4 _startingIndexLine5CC; //closed line
    td::UINT4 _nIndicesLine5CC;
    td::ColorID _colorE = td::ColorID::Blue;
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
        cnt::PushBackVector<gui::gl::Point2d> vboPoints;
        vboPoints.reserve(1024); //a bit more
        
        cnt::PushBackVector<td::UINT4> eboIndices;
        eboIndices.reserve(1024);
        
        //create 2D triangulator
        gui::gl::Triang2D tr(1024);
        
        //Quad
        const gui::gl::Point2d quad[] = {{ 585, 360 }, { 630, 135 }, { 405, 180 }, { 360, 315 } };
        td::UINT4 offsetInd = 0;
        auto [triangTypeQuad, nIndicesQuad] = tr.triangulate(quad, 4, vboPoints, eboIndices);
        assert(triangTypeQuad == gui::gl::Primitive::TriangleFan);
        assert(nIndicesQuad == 0);
        offsetInd += nIndicesQuad;
        auto iSize = eboIndices.size();
        assert(iSize == 0);
        // Pentagon 1 (counter-clockwise)
        {
            const gui::gl::Point2d pentagon[] = { { 100, 450 }, { 200, 450 }, { 90, 360 }, { 200, 180 }, { 45, 360 } };
            auto [triangTypePent, nIndicesPent] = tr.triangulate(pentagon, 5, vboPoints, eboIndices);
            assert(triangTypePent == gui::gl::Primitive::TriangleFan);
            assert(nIndicesPent == 0);
            iSize = eboIndices.size();
            offsetInd += nIndicesPent;
            assert(iSize == 0);
        }
        
        // Pentagon 2 (counter-clockwise) nonconvex node is second node)
        {
            gui::gl::Point2d pentagon[] = { { 200, 450 }, { 90, 360 }, { 200, 180 }, { 45, 360 }, { 100, 450 }};
            math::translate(pentagon, 5, 150, 0); //move to right
            auto [triangTypePent, nIndicesPent] = tr.triangulate(pentagon, 5, vboPoints, eboIndices);
            assert(triangTypePent == gui::gl::Primitive::TriangleFan);
            assert(nIndicesPent == 0);
            iSize = eboIndices.size();
            offsetInd += nIndicesPent;
            assert(iSize == 0);
        }
        
        // Pentagon 3 (counter-clockwise) nonconvex node is first node)
        {
            gui::gl::Point2d pentagon[] = { { 90, 360 }, { 200, 180 }, { 45, 360 }, { 100, 450 }, { 200, 450 }};
            math::translate(pentagon, 5, 300, 0); //move to right
            auto [triangTypePent, nIndicesPent] = tr.triangulate(pentagon, 5, vboPoints, eboIndices);
            assert(triangTypePent == gui::gl::Primitive::TriangleFan);
            assert(nIndicesPent == 0);
            iSize = eboIndices.size();
            offsetInd += nIndicesPent;
            assert(iSize == 0);
        }
        
        // Pentagon 4 (counter-clockwise) nonconvex node is last node)
        {
            gui::gl::Point2d pentagon[] = { { 200, 180 }, { 45, 360 }, { 100, 450 }, { 200, 450 }, { 90, 360 }};
            math::translate(pentagon, 5, 450, 0); //move to right
            auto [triangTypePent, nIndicesPent] = tr.triangulate(pentagon, 5, vboPoints, eboIndices);
            assert(triangTypePent == gui::gl::Primitive::TriangleFan);
            assert(nIndicesPent == 0);
            iSize = eboIndices.size();
            offsetInd += nIndicesPent;
            assert(iSize == 0);
        }
        
        // Pentagon 5 (clockwise)
        {
            gui::gl::Point2d pentagon[] = { { 45, 360 }, { 200, 180 }, { 90, 360 }, { 200, 450 }, { 100, 450 } };
            math::translate(pentagon, 5, 600, 0); //move to right
            auto [triangTypePent, nIndicesPent] = tr.triangulate(pentagon, 5, vboPoints, eboIndices);
            assert(triangTypePent == gui::gl::Primitive::TriangleFan);
            assert(nIndicesPent == 0);
            iSize = eboIndices.size();
            offsetInd += nIndicesPent;
            assert(iSize == offsetInd);
        }
        
        // Pentagon 6 (clockwise) nonconvex node is second node)
        {
            gui::gl::Point2d pentagon[] = { { 100, 450 }, { 45, 360 }, { 200, 180 }, { 90, 360 }, { 200, 450 } };
            math::translate(pentagon, 5, 750, 0); //move to right
            auto [triangTypePent, nIndicesPent] = tr.triangulate(pentagon, 5, vboPoints, eboIndices);
            assert(triangTypePent == gui::gl::Primitive::TriangleFan);
            assert(nIndicesPent == 0);
            iSize = eboIndices.size();
            offsetInd += nIndicesPent;
            assert(iSize == offsetInd);
        }
        
        // Pentagon 7 (clockwise) nonconvex node is first node)
        {
            gui::gl::Point2d pentagon[] = { { 200, 450 }, { 100, 450 }, { 45, 360 }, { 200, 180 }, { 90, 360 } };
            math::translate(pentagon, 5, 900, 0); //move to right
            auto [triangTypePent, nIndicesPent] = tr.triangulate(pentagon, 5, vboPoints, eboIndices);
            assert(triangTypePent == gui::gl::Primitive::TriangleFan);
            assert(nIndicesPent == 0);
            iSize = eboIndices.size();
            offsetInd += nIndicesPent;
            assert(iSize == offsetInd);
        }
        
        // Pentagon 8 (clockwise) nonconvex node is last node)
        {
            gui::gl::Point2d pentagon[] = {  { 90, 360 }, { 200, 450 }, { 100, 450 }, { 45, 360 }, { 200, 180 } };
            math::translate(pentagon, 5, 1050, 0); //move to right
            auto [triangTypePent, nIndicesPent] = tr.triangulate(pentagon, 5, vboPoints, eboIndices);
            assert(triangTypePent == gui::gl::Primitive::TriangleFan);
            assert(nIndicesPent == 0);
            iSize = eboIndices.size();
            offsetInd += nIndicesPent;
            assert(iSize == offsetInd);
        }
        
        float lineWidth = 5.0f;
        bool bClosedLine = false;
        //Lines - clockwise order
        {
            //Line1
            _startingIndexLine1 = (td::UINT4) eboIndices.size();
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }};
            auto [trType, nVertices, nIndices] =  gui::gl::Triang2D::triangulateLine(line, 2, lineWidth, bClosedLine, vboPoints, eboIndices);
            assert(trType != gui::gl::Primitive::Error);
            _nIndicesLine1 = nIndices;
        }
        
        {
            //Line2
            _startingIndexLine2 = (td::UINT4) eboIndices.size();
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }, { 200, 750 }};
            math::translate(line, 3, 150, 0); //move to right
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 3, lineWidth, bClosedLine, vboPoints, eboIndices);
            assert(trType != gui::gl::Primitive::Error);
            _nIndicesLine2 = nIndices;
        }
        
        {
            //Line3
            _startingIndexLine3 = (td::UINT4) eboIndices.size();
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }, { 200, 750 }, { 220, 550 }, { 230, 750 }};
            math::translate(line, 5, 300, 0); //move to right
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 5, lineWidth, bClosedLine, vboPoints, eboIndices);
            assert(trType != gui::gl::Primitive::Error);
            _nIndicesLine3 = nIndices;
        }
        
        {
            //Line4
//            bClosedLine = false;
            _startingIndexLine4 = (td::UINT4) eboIndices.size();
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }, { 200, 750 }, { 90, 750 }};
            math::translate(line, 4, 450, 0); //move to right
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 4, lineWidth, bClosedLine, vboPoints, eboIndices);
            assert(trType != gui::gl::Primitive::Error);
            _nIndicesLine4 = nIndices;
        }
        
        {
            //Line5
            bClosedLine = true;
            _startingIndexLine5 = (td::UINT4) eboIndices.size();
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }, { 200, 750 }, { 90, 750 }};
            math::translate(line, 4, 600, 0); //move to right
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 4, lineWidth, bClosedLine, vboPoints, eboIndices);
            assert(trType != gui::gl::Primitive::Error);
            _nIndicesLine5 = nIndices;
        }
        
        //Lines - counterclockwise order (CC)
        bClosedLine = false;
        {
            //Line1
            _startingIndexLine1CC = (td::UINT4) eboIndices.size();
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }};
            mu::reverse(line, 2);
            math::translate(line, 2, 750, 0); //move to right
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 2, lineWidth, bClosedLine, vboPoints, eboIndices);
            assert(trType != gui::gl::Primitive::Error);
            _nIndicesLine1CC = nIndices;
        }
        
        {
            //Line2
            _startingIndexLine2CC = (td::UINT4) eboIndices.size();
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }, { 200, 750 }};
            mu::reverse(line, 3);
            math::translate(line, 3, 900, 0); //move to right
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 3, lineWidth, bClosedLine, vboPoints, eboIndices);
            assert(trType != gui::gl::Primitive::Error);
            _nIndicesLine2CC = nIndices;
        }
        
        {
            //Line3
            _startingIndexLine3CC = (td::UINT4) eboIndices.size();
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }, { 200, 750 }, { 220, 550 }, { 230, 750 }};
            mu::reverse(line, 5);
            math::translate(line, 5, 1050, 0); //move to right
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 5, lineWidth, bClosedLine, vboPoints, eboIndices);
            assert(trType != gui::gl::Primitive::Error);
            _nIndicesLine3CC = nIndices;
        }
        
        {
            //Line4
//            bClosedLine = false;
            _startingIndexLine4CC = (td::UINT4) eboIndices.size();
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }, { 200, 750 }, { 90, 750 }};
            mu::reverse(line, 4);
            math::translate(line, 4, 1200, 0); //move to right
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 4, lineWidth, bClosedLine, vboPoints, eboIndices); // gui::gl::triangulateLine(line, 4, lineWidth, bClosedLine, vboPoints, eboIndices);
            assert(trType != gui::gl::Primitive::Error);
            _nIndicesLine4CC = nIndices;
        }
        
        {
            //Line5
            bClosedLine = true;
            _startingIndexLine5CC = (td::UINT4) eboIndices.size();
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }, { 200, 750 }, { 90, 750 }};
            mu::reverse(line, 4);
            math::translate(line, 4, 1350, 0); //move to right
            auto [trType, nVertices, nIndices] = gui::gl::Triang2D::triangulateLine(line, 4, lineWidth, bClosedLine, vboPoints, eboIndices); // gui::gl::triangulateLine(line, 4, lineWidth, bClosedLine, vboPoints, eboIndices);
            assert(trType != gui::gl::Primitive::Error);
            _nIndicesLine5CC = nIndices;
        }
        
        //Letter E
        _startingIndexOfE = (td::UINT4) eboIndices.size();
        
        // Vertex data for drawing letter E with single color
        const float h = 100;
        const float h1 = h / 5;
        const float h2 = h*2/5;
        const float h3 = h*3/5;
        const float h4 = h*4/5;
        const float w = h/2;
        const float w1 = w/3;
        const float w2= w*4/5;

        gui::gl::Point2d letterE[] = {{0,0},{0,h},{w,h},{w,h4},{w1,h4},{w1,h3},{w2,h3},{w2,h2},{w1,h2},{w1,h1},{w,h1},{w,0}};
        
        _noOfVerticesE = (td::UINT4) (sizeof(letterE) / sizeof(gui::gl::Point2d));
        
        for (td::UINT4 i = 0; i<_noOfVerticesE; ++i)
        {
            auto& n = letterE[i];
            n.x += 200;
            n.y += 200;
        }

        _startingVertexE = (td::UINT4) vboPoints.size();
        
        auto [triangTypeE, nIndicesE] = tr.triangulate(letterE, _noOfVerticesE, vboPoints, eboIndices);
        
        _noOfIndicesInE = nIndicesE;
        
        auto vSize = vboPoints.size();
        iSize = eboIndices.size();
        
        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        size_t nVBOBytes = vSize * sizeof(gui::gl::Point2d);
        glBufferData(GL_ARRAY_BUFFER, nVBOBytes, vboPoints.begin(), GL_STATIC_DRAW);

        // Specify vertex position attribute
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        size_t nEBOBytes = iSize * sizeof(td::UINT4);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, nEBOBytes, eboIndices.begin(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
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
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program
        glUseProgram(_shaderProgram);

        // Set up model matrix and color in the vertex shader
        const auto& projectionMatrix = getProjection();
        glUniformMatrix4fv(_mvpLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        // Use the VAO
        glBindVertexArray(_vao);

        //Draw using indices
        //Draw quad in red
        glUniform3f(_colorLoc, 1.0f, 0.0f, 0.0f); // Red color
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        //Draw pentagon1 in red
        glUniform3f(_colorLoc, 1.0f, 0.0f, 0.0f); // Red color
        glDrawArrays(GL_TRIANGLE_FAN, 4, 5);
        //Draw pentagon2 in green
        glUniform3f(_colorLoc, 0.0f, 1.0f, 0.0f); // Green color
        glDrawArrays(GL_TRIANGLE_FAN, 9, 5);
        //Draw pentagon3 in blue
        glUniform3f(_colorLoc, 0.0f, 0.0f, 1.0f); // Blue color
        glDrawArrays(GL_TRIANGLE_FAN, 14, 5);
        //Draw pentagon4 in yellow
        glUniform3f(_colorLoc, 1.0f, 1.0f, 0.0f); // Yellow color
        glDrawArrays(GL_TRIANGLE_FAN, 19, 5);
        //Draw pentagon5 in red
        glUniform3f(_colorLoc, 1.0f, 0.0f, 0.0f); // Red color
        glDrawArrays(GL_TRIANGLE_FAN, 24, 5);
        //Draw pentagon6 in green
        glUniform3f(_colorLoc, 0.0f, 1.0f, 0.0f); // Green color
        glDrawArrays(GL_TRIANGLE_FAN, 29, 5);
        //Draw pentagon7 in blue
        glUniform3f(_colorLoc, 0.0f, 0.0f, 1.0f); // Blue color
        glDrawArrays(GL_TRIANGLE_FAN, 34, 5);
        //Draw pentagon8 in yellow
        glUniform3f(_colorLoc, 1.0f, 1.0f, 0.0f); // Yellow color
        glDrawArrays(GL_TRIANGLE_FAN, 39, 5);
        
        //Lines in (nodes in clockwise order)
        //Draw line1 in red
        glUniform3f(_colorLoc, 1.0f, 0.0f, 0.0f); // Red color
        glDrawElements(GL_TRIANGLES, _nIndicesLine1, GL_UNSIGNED_INT, (void*)(_startingIndexLine1*sizeof(td::UINT4)));
        //Draw line2 in green
        glUniform3f(_colorLoc, 0.0f, 1.0f, 0.0f); // Green color
        glDrawElements(GL_TRIANGLES, _nIndicesLine2, GL_UNSIGNED_INT, (void*)(_startingIndexLine2*sizeof(td::UINT4)));
        //Draw line3 in blue
        glUniform3f(_colorLoc, 0.0f, 0.0f, 1.0f); // Blue color
        glDrawElements(GL_TRIANGLES, _nIndicesLine3, GL_UNSIGNED_INT, (void*)(_startingIndexLine3*sizeof(td::UINT4)));
        //Draw line4 in white (open rect)
        glUniform3f(_colorLoc, 1.0f, 1.0f, 1.0f); // White color
        glDrawElements(GL_TRIANGLES, _nIndicesLine4, GL_UNSIGNED_INT, (void*)(_startingIndexLine4*sizeof(td::UINT4)));
        //Draw line5 in yellow (open rect)
        glUniform3f(_colorLoc, 1.0f, 1.0f, 0.0f); // Yellow color
        glDrawElements(GL_TRIANGLES, _nIndicesLine5, GL_UNSIGNED_INT, (void*)(_startingIndexLine5*sizeof(td::UINT4)));
        
        //Draw line1CC in red
        glUniform3f(_colorLoc, 1.0f, 0.0f, 0.0f); // Red color
        glDrawElements(GL_TRIANGLES, _nIndicesLine1CC, GL_UNSIGNED_INT, (void*)(_startingIndexLine1CC*sizeof(td::UINT4)));
        //Draw line2CC in green
        glUniform3f(_colorLoc, 0.0f, 1.0f, 0.0f); // Green color
        glDrawElements(GL_TRIANGLES, _nIndicesLine2CC, GL_UNSIGNED_INT, (void*)(_startingIndexLine2CC*sizeof(td::UINT4)));
        //Draw line3CC in blue
        glUniform3f(_colorLoc, 0.0f, 0.0f, 1.0f); // Blue color
        glDrawElements(GL_TRIANGLES, _nIndicesLine3CC, GL_UNSIGNED_INT, (void*)(_startingIndexLine3CC*sizeof(td::UINT4)));
        //Draw line4CC in white (open rect)
        glUniform3f(_colorLoc, 1.0f, 1.0f, 1.0f); // White color
        glDrawElements(GL_TRIANGLES, _nIndicesLine4CC, GL_UNSIGNED_INT, (void*)(_startingIndexLine4CC*sizeof(td::UINT4)));
        //Draw line5CC in yellow (open rect)
        glUniform3f(_colorLoc, 1.0f, 1.0f, 0.0f); // Yellow color
        glDrawElements(GL_TRIANGLES, _nIndicesLine5CC, GL_UNSIGNED_INT, (void*)(_startingIndexLine5CC*sizeof(td::UINT4)));
        
        //Draw E in specified color
        glm::vec3 eColor;
        gui::gl::Color::get(_colorE, eColor);
        glUniform3f(_colorLoc, eColor.r, eColor.g, eColor.b); // Specified color
        glDrawElements(GL_TRIANGLES, _noOfIndicesInE, GL_UNSIGNED_INT, (void*)(_startingIndexOfE*sizeof(td::UINT4)));
        //draw white line arround letter E (no need for indices)
        glUniform3f(_colorLoc, 1.0f, 1.0f, 1.0f); // White color
        glDrawArrays(GL_LINE_LOOP, _startingVertexE, _noOfVerticesE);
        
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
            increaseZoom();
        }
    }
    
    void onSecondaryButtonPressed(const gui::InputDevice& inputDevice) override
    {
        //call base impelementation first
        Base::onSecondaryButtonPressed(inputDevice);
        //then do my part
        if (!inputDevice.isAnyModifierPressed())
        {
            decreaseZoom();
        }
    }
    
public:
    ViewGLTriang()
    {
        //invert Y axis
        createContext(true);
    }
    
    ~ViewGLTriang()
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
    
    void setEColor(td::ColorID clrE)
    {
        _colorE = clrE;
        reDraw();
    }
};
