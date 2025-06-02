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
#include "Program.h"
#include <gui/gl/Buffer.h>

class ViewGLTriang2 : public gui::gl::OrthoView
{
    using Base = gui::gl::OrthoView;
    gui::Texture _frameTexture; //for frame export
    Program _program;
    gui::gl::Buffer _gpuBuffer;
    
    gui::gl::Command* _fillE = nullptr;
private:
    bool setupShaders()
    {
        if (!_program.link(":shVert", ":shFrag"))
            return false;
        return true;
    }
    //setup data and send it to GPU
    void setupVBO()
    {
        //create 2D triangulator
        gui::gl::Triang2D tr(1024);
    
        gui::gl::Buffer* pBuffer = &_gpuBuffer;
        
        //Quad
        const gui::gl::Point2d quad[] = {{ 585, 360 }, { 630, 135 }, { 405, 180 }, { 360, 315 } };
        auto cmd = tr.triangulate(pBuffer, quad, 4);
        cmd->setColor(td::ColorID::Red);
        _gpuBuffer.encode(cmd);
        // Pentagon 1 (counter-clockwise)
        {
            const gui::gl::Point2d pentagon[] = { { 100, 450 }, { 200, 450 }, { 90, 360 }, { 200, 180 }, { 45, 360 } };
            cmd = tr.triangulate(pBuffer, pentagon, 5);
            cmd->setColor(td::ColorID::Red);
            _gpuBuffer.encode(cmd);
        }
        
        // Pentagon 2 (counter-clockwise) nonconvex node is second node)
        {
            gui::gl::Point2d pentagon[] = { { 200, 450 }, { 90, 360 }, { 200, 180 }, { 45, 360 }, { 100, 450 }};
            math::translate(pentagon, 5, 150, 0); //move to right
            cmd = tr.triangulate(pBuffer, pentagon, 5);
            cmd->setColor(td::ColorID::Green);
            _gpuBuffer.encode(cmd);
        }
        
        // Pentagon 3 (counter-clockwise) nonconvex node is first node)
        {
            gui::gl::Point2d pentagon[] = { { 90, 360 }, { 200, 180 }, { 45, 360 }, { 100, 450 }, { 200, 450 }};
            math::translate(pentagon, 5, 300, 0); //move to right
            cmd = tr.triangulate(pBuffer, pentagon, 5);
            cmd->setColor(td::ColorID::Blue);
            _gpuBuffer.encode(cmd);
        }
        
        // Pentagon 4 (counter-clockwise) nonconvex node is last node)
        {
            gui::gl::Point2d pentagon[] = { { 200, 180 }, { 45, 360 }, { 100, 450 }, { 200, 450 }, { 90, 360 }};
            math::translate(pentagon, 5, 450, 0); //move to right
            cmd = tr.triangulate(pBuffer, pentagon, 5);
            cmd->setColor(td::ColorID::Yellow);
            _gpuBuffer.encode(cmd);
        }
        
        // Pentagon 5 (clockwise)
        {
            gui::gl::Point2d pentagon[] = { { 45, 360 }, { 200, 180 }, { 90, 360 }, { 200, 450 }, { 100, 450 } };
            math::translate(pentagon, 5, 600, 0); //move to right
            cmd = tr.triangulate(pBuffer, pentagon, 5);
            cmd->setColor(td::ColorID::Red);
            _gpuBuffer.encode(cmd);
        }
        
        // Pentagon 6 (clockwise) nonconvex node is second node)
        {
            gui::gl::Point2d pentagon[] = { { 100, 450 }, { 45, 360 }, { 200, 180 }, { 90, 360 }, { 200, 450 } };
            math::translate(pentagon, 5, 750, 0); //move to right
            cmd = tr.triangulate(pBuffer, pentagon, 5);
            cmd->setColor(td::ColorID::Green);
            _gpuBuffer.encode(cmd);
        }
        
        // Pentagon 7 (clockwise) nonconvex node is first node)
        {
            gui::gl::Point2d pentagon[] = { { 200, 450 }, { 100, 450 }, { 45, 360 }, { 200, 180 }, { 90, 360 } };
            math::translate(pentagon, 5, 900, 0); //move to right
            cmd = tr.triangulate(pBuffer, pentagon, 5);
            cmd->setColor(td::ColorID::Blue);
            _gpuBuffer.encode(cmd);
        }
        
        // Pentagon 8 (clockwise) nonconvex node is last node)
        {
            gui::gl::Point2d pentagon[] = {  { 90, 360 }, { 200, 450 }, { 100, 450 }, { 45, 360 }, { 200, 180 } };
            math::translate(pentagon, 5, 1050, 0); //move to right
            cmd = tr.triangulate(pBuffer, pentagon, 5);
            cmd->setColor(td::ColorID::Yellow);
            _gpuBuffer.encode(cmd);
        }
        
        float lineWidth = 5.0f;
        bool bClosedLine = false;
        //Lines - clockwise order
        {
            //Line1
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }};
            cmd = tr.triangulateLine(pBuffer, line, 2, lineWidth, bClosedLine);
            cmd->setColor(td::ColorID::Red);
            _gpuBuffer.encode(cmd);
        }
        
        {
            //Line2
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }, { 200, 750 }};
            math::translate(line, 3, 150, 0); //move to right
            cmd = tr.triangulateLine(pBuffer, line, 3, lineWidth, bClosedLine);
            cmd->setColor(td::ColorID::Green);
            _gpuBuffer.encode(cmd);
        }
        
        {
            //Line3
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }, { 200, 750 }, { 220, 550 }, { 230, 750 }};
            math::translate(line, 5, 300, 0); //move to right
            cmd = tr.triangulateLine(pBuffer, line, 5, lineWidth, bClosedLine);
            cmd->setColor(td::ColorID::Blue);
            _gpuBuffer.encode(cmd);
        }
        
        {
            //Line4
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }, { 200, 750 }, { 90, 750 }};
            math::translate(line, 4, 450, 0); //move to right
            cmd = tr.triangulateLine(pBuffer, line, 4, lineWidth, bClosedLine);
            cmd->setColor(td::ColorID::White);
            _gpuBuffer.encode(cmd);
        }
        
        {
            //Line5
            bClosedLine = true;
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }, { 200, 750 }, { 90, 750 }};
            math::translate(line, 4, 600, 0); //move to right
            cmd = tr.triangulateLine(pBuffer, line, 4, lineWidth, bClosedLine);
            cmd->setColor(td::ColorID::Yellow);
            _gpuBuffer.encode(cmd);
        }
        
        //Lines - counterclockwise order (CC)
        bClosedLine = false;
        {
            //Line1
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }};
            mu::reverse(line, 2);
            math::translate(line, 2, 750, 0); //move to right
            cmd = tr.triangulateLine(pBuffer, line, 2, lineWidth, bClosedLine);
            cmd->setColor(td::ColorID::Red);
            _gpuBuffer.encode(cmd);
        }
        
        {
            //Line2;
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }, { 200, 750 }};
            mu::reverse(line, 3);
            math::translate(line, 3, 900, 0); //move to right
            cmd = tr.triangulateLine(pBuffer, line, 3, lineWidth, bClosedLine);
            cmd->setColor(td::ColorID::Green);
            _gpuBuffer.encode(cmd);
        }
        
        {
            //Line3
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }, { 200, 750 }, { 220, 550 }, { 230, 750 }};
            mu::reverse(line, 5);
            math::translate(line, 5, 1050, 0); //move to right
            cmd= tr.triangulateLine(pBuffer, line, 5, lineWidth, bClosedLine);
            cmd->setColor(td::ColorID::Blue);
            _gpuBuffer.encode(cmd);
        }
        
        {
            //Line4
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }, { 200, 750 }, { 90, 750 }};
            mu::reverse(line, 4);
            math::translate(line, 4, 1200, 0); //move to right
            cmd = tr.triangulateLine(pBuffer, line, 4, lineWidth, bClosedLine);
            cmd->setColor(td::ColorID::White);
            _gpuBuffer.encode(cmd);
        }
        
        {
            //Line5
            bClosedLine = true;
            gui::gl::Point2d line[] = {  { 90, 550 }, { 200, 550 }, { 200, 750 }, { 90, 750 }};
            mu::reverse(line, 4);
            math::translate(line, 4, 1350, 0); //move to right
            cmd = tr.triangulateLine(pBuffer, line, 4, lineWidth, bClosedLine);
            cmd->setColor(td::ColorID::Yellow);
            _gpuBuffer.encode(cmd);
        }
        
        //Letter E
        
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
        
        td::UINT4 noOfVerticesE = (td::UINT4) (sizeof(letterE) / sizeof(gui::gl::Point2d));
        
        for (td::UINT4 i = 0; i<noOfVerticesE; ++i)
        {
            auto& n = letterE[i];
            n.x += 200;
            n.y += 200;
        }

        td::UINT4 startingVertexE = _gpuBuffer.getNoOfVertices();
        size_t startigIndexE = _gpuBuffer.getNoOfIndices();
        //blue E
        _fillE = tr.triangulate(pBuffer, letterE, noOfVerticesE);
        assert(_fillE);
        _fillE->setColor(td::ColorID::Blue);
        _gpuBuffer.encode(_fillE);
        td::UINT4 nVertices = _gpuBuffer.getNoOfVertices();
        //white line around E
        auto cmdLineE = _gpuBuffer.createCommand();
        cmdLineE->createDrawArrays(gui::gl::Primitive::LineLoop, startingVertexE, noOfVerticesE);
//        auto cmdLineE = tr.createRecycledLine(gui::gl::Primitive::LineLoop, letterE, startingVertexE, noOfVerticesE, &_gpuBuffer, layoutID);
//        assert(cmdLineE);
        cmdLineE->setColor(td::ColorID::White);
        _gpuBuffer.encode(cmdLineE);
        
        if (!_gpuBuffer.commit())
        {
            mu::dbgLog("ERROR! Cannot commit buffer to GPU");
            return;
        }
        
        _program.setBuffer(&_gpuBuffer);
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
        dbgCheckGLError();
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        gui::gl::Context::clear({ gui::gl::Context::Clear::Color, gui::gl::Context::Clear::Depth }); //cler OpenGL buffers
        // Use the shader program (and activate VAO internally)
        _program.activate();
        // Set up model matrix and color in the vertex shader
        const auto& projectionMatrix = getProjection();
        _program.setMVP(projectionMatrix);
        //exeucte all encoded commands
        _program.execute();
        // Unbind shader program (and VAO internally by shader program)
        _program.deActivate();
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
    ViewGLTriang2()
    {
        //specify buffer layouts before creatin the context
        _gpuBuffer.init(64, 1024, 1024, {gui::gl::DataType::vec2});
        
        //invert Y axis
        createContext(true);
    }
    
    ~ViewGLTriang2()
    {
        makeCurrentContext(); //make context so that resources can be deleted
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
        if (_fillE != nullptr)
        {
            _fillE->setColor(clrE);
            reDraw();
        }
    }
};
