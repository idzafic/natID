//
//  Created by Izudin Dzafic on 05/05/2025.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/OrthoView.h>
#include <gui/Key.h>
#include <gui/Texture.h>
#include <gui/FileDialog.h>
#include <gui/gl/ObjectGenerator.h>
#include "Program.h"
#include <gui/gl/Textures.h>
#include <gui/gl/Buffer.h>

//Demonstrates:
// - OOGL API with gl::Textures
// - replicating textures

class ViewChess2 : public gui::gl::OrthoView
{
    using Base = gui::gl::OrthoView;
    gui::gl::Buffer _gpuBuffer;
    Program _program;
    gui::gl::Textures _textures;
    gui::Texture _textureChess1; //to show in view
    gui::Texture _textureChess2; //to show in view
    
    gui::Texture _frameTexture; //for frame export
    gui::gl::Command* _pCmdDrawChess = nullptr; //to update texture
    td::BYTE _usedTexture = 0; //0 or 1 (required for texture switching)
private:
    bool setup()
    {
        if (!_program.link(":shVert", ":shFrag")) //prepare the program
            return false;
        //encode command to set transformation matrix (uniform)
        const auto& projectionMatrix = getProjection();
        gui::gl::Command*  pMtxSetterCmd = _gpuBuffer.createCommand();
        pMtxSetterCmd->createMVPSetter(&projectionMatrix);
        _gpuBuffer.encode(pMtxSetterCmd);
        
        const float c = 300;
        float rectVertices[] = {
            //x   y    s      t
            -c,  -c,  0.0f, 0.0f,
             c,  -c,  4.0f, 0.0f,
             c,   c,  4.0f, 4.0f,
            -c,   c,  0.0f, 4.0f
        };
        _gpuBuffer.appendVertices(rectVertices, 4); //populate buffer on CPU side
        
        //encode command to draw tetured cube
        _pCmdDrawChess = _gpuBuffer.createCommand();
        _pCmdDrawChess->createDrawArrays(gui::gl::Primitive::TriangleFan, 0, 4);
        _pCmdDrawChess->setTexture(gui::gl::Textures::Unit::T0, _usedTexture);
        _gpuBuffer.encode(_pCmdDrawChess);
        
        if (!_gpuBuffer.commit())   //initiate data transfer to GPU
        {
            mu::dbgLog("ERROR! Cannot commit buffer to GPU");
            return false;
        }
        
        // Load textures
        _textures.append({&_textureChess1, &_textureChess2});
        //update program with buffer and textures manager
        _program.setBuffer(&_gpuBuffer);
        _program.setTextures(&_textures);
        
        dbgCheckGLError();
        return true;
    }
    
protected:
    
    void onInit() override
    {
        auto [major, minor] = getOpenGLVersion();
        mu::dbgLog("Used opengl %d.%d", major, minor);
        if (!setup())
        {
            mu::dbgLog("ERROR! OpenGL cannnot setup the scene!");
            return;
        }

        gui::Point refPoint(-600, -300);
        setModelRefPoint(refPoint);
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        gui::gl::Context::clear(td::ColorID::Black);
        gui::gl::Context::clear({gui::gl::Context::Clear::Color, gui::gl::Context::Clear::Depth});

        _program.activate();
        _program.execute();
        // Unbind program
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
    ViewChess2()
    : _textureChess1(":chess1")
    , _textureChess2(":chess2")
    {
        //2 cmds, 4 vertices, 0-ind       xy - vec2,            st - vec2
        _gpuBuffer.init(2, 4, 0, {gui::gl::DataType::vec2, gui::gl::DataType::vec2});
        //do not invert Y axis
        createContext(false);
        registerForFocusEvents();
        
        //handle menu/toolbar action to export frame to image
        onActionItem(20, 10, [this](){
            exportFrame();
        });
    }
    
    ~ViewChess2()
    {
        makeCurrentContext();
    }
    
    void switchTexture()
    {
        if (_usedTexture == 0)
            _usedTexture = 1;
        else
            _usedTexture = 0;
        _pCmdDrawChess->setTexture(gui::gl::Textures::Unit::T0, _usedTexture);
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
        });
        
    }
};
