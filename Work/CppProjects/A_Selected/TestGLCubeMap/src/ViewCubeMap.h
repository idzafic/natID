//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/CameraView.h>
#include <gui/Key.h>
#include <gui/Texture.h>
#include <gui/FileDialog.h>
#include "ProgramCube.h"
#include "ProgramSphere.h"
#include <gui/gl/Buffer.h>
#include <gui/gl/Textures.h>
#include <gui/gl/ObjectGenerator.h>
#include <gui/gl/SkyBox.h>
#include <gui/gl/Text.h>
#include <td/Timer.h>

//Demonstrates:

// - Cube and Sphere generation
// - CubeMap
// - Sphere Mapping
// - SkyBox
// - Instanced drawing

class ViewCubeMap : public gui::gl::CameraView
{
    using Base = gui::gl::View;
    gui::Texture _frameTexture; //for frame export
    
    gui::gl::Camera _camera;
    
    glm::mat4 _cameraProjectionMatrix;
    
    td::Timer<false> _timer;
    
    gui::gl::Textures _textures;
    //1. Cube part
    gui::gl::Buffer _cubeBuffer;
    //Program to draw cubes
    ProgramCube _programCube;
    gui::gl::Command* _pCubeMtxSetterCmd = nullptr;
    gui::gl::Command* _pCubeTextureCmd = nullptr;
    
    //2. Sphere part
    gui::gl::Buffer _sphereBuffer;

    //program to draw spheres (planets)
    ProgramCube _programSphere;
    gui::gl::Command* _pSphereMtxSetterCmd = nullptr;
    
    gui::gl::SkyBox _skyBox;
    gui::gl::Text _text;
    
    float _accAngle = 0;
    float _dAngle = 0.01f;
    float _fps = 0.0f;
    float _font3Scale = 0.6f;
    td::UINT2 _nFrames = 0;
    
//    const unsigned int nTriangles = 6 * 2;   // Number of triangles rendered
    td::BYTE _usedTexture = 0; //0 or 1
    td::BYTE _updateText = 0;
private:
    bool setupPrograms()
    {
        if (!_programCube.link(":shCubeVert", ":shCubeFrag"))
            return false;
        if (!_programSphere.link(":shSphereVert", ":shSphereFrag"))
            return false;
        return true;
    }
    
    //setup data and send it to GPU, create command list
    void setup()
    {
        // Load textures
        _textures.reserve(6);
        _textures.appendCubeMap({":cm0Left", ":cm0Front", ":cm0Right", ":cm0Back", ":cm0Top", ":cm0Bottom"});
        _textures.appendCubeMap({":cm1Left", ":cm1Front", ":cm1Right", ":cm1Back", ":cm1Top", ":cm1Bottom"});
        _textures.appendCubeMap({":cm2Left", ":cm2Front", ":cm2Right", ":cm2Back", ":cm2Top", ":cm2Bottom"});
        _textures.appendCubeMap({":cm3Left", ":cm3Front", ":cm3Right", ":cm3Back", ":cm3Top", ":cm3Bottom"}); //sand
        _textures.append({":earth", ":venus"});

        assert(_textures.size() == 6);
        
        td::UINT4 textCubeMap = _textures.getID(3);
        _skyBox.init(&_cameraProjectionMatrix, textCubeMap, gui::gl::Textures::Unit::T3);
        _skyBox.commit(100.0f);
        
        //encode command to set transformation matrix (uniform)
        _pCubeMtxSetterCmd = _cubeBuffer.createCommand();
        _pCubeMtxSetterCmd->createMVPSetter(&_cameraProjectionMatrix);
        _cubeBuffer.encode(_pCubeMtxSetterCmd);
        
        _pSphereMtxSetterCmd = _sphereBuffer.createCommand();
        _pSphereMtxSetterCmd->createMVPSetter(&_cameraProjectionMatrix);
        _sphereBuffer.encode(_pSphereMtxSetterCmd);
        
        float mx=1.5f;
        float instanceDataCube[] ={
            0,              0,       0, -4*mx, 0, 0, -1, 0, 1,
            math::FLT_PI/4, 0,       0, -3*mx, 0, 0, 0, 1, -1,
            0, math::FLT_PI/4,       0, -2*mx, 0, 0, 0, 1, -1,
            0, math::FLT_PI/3,       0, -1*mx, 0, 0, 1, 1, -1,
            0, math::FLT_PI/2,       0,  0, 0, 0, 1, 1, -1,
            0, math::FLT_PI/3,       0,  1*mx, 0, 0, 1, 1, -1,
            0, math::FLT_PI/4,       0,  2*mx, 0, 0, 1, 1, -1,
            math::FLT_PI/4, 0,       0,  3*mx, 0, 0, 1, 1, -1,
            0,              0,       0,  4*mx, 0, 0, 1, 1, -1,
            
            0,              0,       0, -4*mx, -1, -1, -1, 0, 1,
            math::FLT_PI/4, math::FLT_PI/2,       0, -3*mx, -1, -1, 0, 1, -1,
            math::FLT_PI/2, math::FLT_PI/4,       0, -2*mx, -1, -1, 0, 1, -1,
            math::FLT_PI/2, math::FLT_PI/3,       0, -1*mx, -1, -1, 1, 1, -1,
            math::FLT_PI/2, math::FLT_PI/2,       0,  0, -1, -1, 1, 1, -1,
            math::FLT_PI/2, math::FLT_PI/3,       0,  1*mx, -1, -1, 1, 1, -1,
            math::FLT_PI/2, math::FLT_PI/4,       0,  2*mx, -1, -1, 1, 0, -1,
            math::FLT_PI/4, 0,                    0,  3*mx, 0, -1, 1, 1, -1,
            math::FLT_PI/2,              0,       0,  4*mx, -1, -1, 1, 1, -1,
            
            0,              0,       math::FLT_PI/2, -4*mx, 1, -2, -1, 0, 1,
            math::FLT_PI/4, 0,       math::FLT_PI/2, -3*mx, 1, -2, 0, 1, -1,
            0, math::FLT_PI/4,       math::FLT_PI/2, -2*mx, 1, -2, 0, 1, -1,
            0, math::FLT_PI/3,       math::FLT_PI/2, -1*mx, 1, -2, 1, 1, -1,
            0, math::FLT_PI/2,       math::FLT_PI/2,  0, 1, -2, 1, 1, -1,
            0, math::FLT_PI/3,       math::FLT_PI/2,  1*mx, 1, -2, 1, 1, -1,
            0, math::FLT_PI/4,       math::FLT_PI/2,  2*mx, 1, -2, 1, 1, -1,
            math::FLT_PI/4, 0,       math::FLT_PI/2,  3*mx, 1, -2, 1, 1, -1,
            0,              0,       math::FLT_PI/2,  4*mx, 1, -2, 1, 1, -1,
            
            0,              0,       math::FLT_PI/2, -4*mx, -3, -2, -1, 0, 1,
            math::FLT_PI/4, 0,       math::FLT_PI/2, -3*mx, -3, -2, 0, 1, -1,
            0, math::FLT_PI/4,       math::FLT_PI/2, -2*mx, -3, -2, 0, 0, -1,
            0, math::FLT_PI/3,       math::FLT_PI/2, -1*mx, -3, -2, 1, 0, 0,
            0, math::FLT_PI/2,       math::FLT_PI/2,  0, -3, -2, 1, 0, -1,
            0, math::FLT_PI/3,       math::FLT_PI/2,  1*mx, -3, -2, 0, 1, 0,
            0, math::FLT_PI/4,       math::FLT_PI/2,  2*mx, -3, -2, 0, -1, 0,
            math::FLT_PI/4, 0,       math::FLT_PI/2,  3*mx, -3, -2, 0, 0, -1,
            0,              0,       math::FLT_PI/2,  4*mx, -3, -2, 0, 0, 1,
            
            0,              0,       math::FLT_PI/2, -4*mx, -5, -2, 1, 0, -1,
            math::FLT_PI/4, 0,       math::FLT_PI/2, -3*mx, -5, -2, 0, -1, 1,
            0, math::FLT_PI/4,       math::FLT_PI/2, -2*mx, -5, -2, 0, 0, -1,
            0, math::FLT_PI/3,       math::FLT_PI/2, -1*mx, -5, -2, -1, 0, 0,
            0, math::FLT_PI/2,       math::FLT_PI/2,  0, -5, -2, 1, 0, 1,
            0, math::FLT_PI/3,       math::FLT_PI/2,  1*mx, -5, -2, 0, -1, 0,
            0, math::FLT_PI/4,       math::FLT_PI/2,  2*mx, -5, -2, 0, 1, 0,
            math::FLT_PI/4, 0,       math::FLT_PI/2,  3*mx, -5, -2, 0, 0, 1,
            0,              0,       math::FLT_PI/2,  4*mx, -5, -2, 0, 0, -1
        };
        
        _cubeBuffer.appendInstances(instanceDataCube, 45);
        
        //encode command to draw textured cube
        // Define it using cube map
        gui::gl::Command* cmd = _cubeBuffer.addCube(1.0f, gui::gl::Texture::Type::CubeMap);
        cmd->setNumberOfInstances(45);
        _cubeBuffer.encode(cmd);
        
        if (!_cubeBuffer.commit())
        {
            mu::dbgLog("ERROR! Cannot commit cube buffer to GPU");
            return;
        }
        
        float m=1.25f;
        float instanceDataSphere[] ={
            0,              0,       0, -4,  5*m, 0, 0, 1, 0,
            0,              0,       0, -4,  4*m, 0, 0, -1, 1,
            0, math::FLT_PI/4,       0, -4,  3*m, 0, 0, 1, -1,
            0, math::FLT_PI/3,       0, -4,  2*m, 0, 1, 1, -1,
            0, math::FLT_PI/2,       0, -4,  1*m, 0, 1, 1, -1,
            0, math::FLT_PI/2,       0, -4,  0, 0, 1, 1, -1,
            0, math::FLT_PI/3,       0, -4, -1*m, 0, 1, 1, -1,
            0, math::FLT_PI/4,       0, -4, -2*m, 0, 1, 1, -1,
            math::FLT_PI/4, 0,       0, -4, -3*m, 0, 1, 1, -1,
            0,              0,       0, -4, -4*m, 0, 1, 1, -1,
            
            0,              0,       0,  4,   5*m, 0, -1, 0, 1,
            math::FLT_PI/4, 0,       0,  4,   4*m, 0, 0, 1, -1,
            0, math::FLT_PI/4,       0,  4,   3*m, 0, 0, 1, -1,
            0, math::FLT_PI/3,       0,  4,   2*m, 0, 1, 1, -1,
            0, math::FLT_PI/2,       0,  4,   1*m, 0, 1, 1, -1,
            0, math::FLT_PI/2,       0,  4,   0, 0, 1, 1, -1,
            0, math::FLT_PI/3,       0,  4,  -1*m, 0, 1, 1, -1,
            0, math::FLT_PI/4,       0,  4,  -2*m, 0, 1, 1, -1,
            math::FLT_PI/4, 0,       0,  4,  -3*m, 0, 1, 1, -1,
            0,              0,       0,  4,  -4*m, 0, 1, 1, -1

        };
        
        _sphereBuffer.appendInstances(instanceDataSphere, 20);
        
        //encode command to draw textured spheres (planets)
        gui::gl::Command* cmdSphere = _sphereBuffer.addSphere(0.6f, 72, 32, gui::gl::Texture::Type::Regular2D);
        cmdSphere->setNumberOfInstances(20);
        _sphereBuffer.encode(cmdSphere);
        
        if (!_sphereBuffer.commit())
        {
            mu::dbgLog("ERROR! Cannot commit sphere buffer to GPU");
            return;
        }
        gui::Size sz;
        getSize(sz);
        _text.init(sz, gui::gl::Textures::Unit::T15);
        
        _text.allocFonts(3);
        
        bool useFancyFonts = false; //set to true/false to see changes
        if (useFancyFonts)
        {
            if (!_text.loadFont(0, ":recharge"))
                return;
            
            auto pFont = _text.loadFont(1, ":zaphino");
            if (!pFont)
                return;
            pFont->setSpaceBetweenLines(-18.0f);
            
            auto pFont2 = _text.loadFont(2, ":videoPhreak");
            if (!pFont2)
                return;
            pFont2->setAdditionalSpaceBetweenGlyphs(3.0f);
        }
        else
        {
            if (!_text.loadFont(0, ":lucida"))
                return;
            if (!_text.loadFont(1, ":vinq")) // vinq
                return;
            
            if (!_text.loadFont(2, ":korataki"))
                return;
        }
        
        
        gui::gl::Font& fnt = _text.getFont(0);
        gui::gl::Point2d pt(10, 10);
        gui::gl::Size txtSize = fnt.addString(pt, "Test for text", 13, td::ColorID::Red);
        pt.y += txtSize.height;
        txtSize = fnt.addString(pt, "Yet another string\nwith a new line\nand another new line !", 0, td::ColorID::DarkGreen);
        pt.y += txtSize.height;
        txtSize = fnt.addString(pt, tr("something"), td::ColorID::DarkBlue);
        pt.y += txtSize.height;
        gui::gl::Font& fnt2 = _text.getFont(1);
        txtSize = fnt2.addString(pt, "Yet another string\nwith a new line\nand another new line...", 0, td::ColorID::DarkRed);
        pt.y += txtSize.height;
        fnt2.addString(pt, tr("something"), td::ColorID::MediumVioletRed);
        _text.commit();
        
        _programCube.setBuffer(&_cubeBuffer);
        _programCube.setTextures(&_textures);
        
        _programSphere.setBuffer(&_sphereBuffer);
        _programSphere.setTextures(&_textures);
        
        _skyBox.setCamera(&_camera);
        _camera.setSkyBox(&_skyBox);
        
        _timer.start();
    }
    
protected:
    
    void onInit() override
    {
        auto [major, minor] = getOpenGLVersion();
        mu::dbgLog("INFO! Used OpenGL version %d.%d", major, minor);
        
        if (!setupPrograms())
        {
            mu::dbgLog("ERROR! OpenGL cannnot setup shaders!");
            assert(false);
            return;
        }
        
        
        //specify buffer layouts before creatin the context
        //                                 Vertex  only, no need for uv
        _cubeBuffer.init(16, 100, 100, 100, {gui::gl::DataType::vec3}, {gui::gl::DataType::vec3, gui::gl::DataType::vec3, gui::gl::DataType::vec3});
        
        _sphereBuffer.init(16, 4096, 4096*3, 100, {gui::gl::DataType::vec3, gui::gl::DataType::vec2}, {gui::gl::DataType::vec3, gui::gl::DataType::vec3, gui::gl::DataType::vec3});
        
        setup();
        // Set up the perspective parameters
//        float fov = 45.0f; // Field of view in degrees
//        float aspectRatio = 16.0f / 9.0f; // Aspect ratio of the viewport
//        float nearClip = 0.01f; // Near clipping plane
//        float farClip = 100.0f; // Far clipping plane
//
//        // Create a perspective matrix
//        _perspectiveMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        _cameraProjectionMatrix = _perspectiveMatrix * _camera.getViewMatrix(); //* I for model
        
        _programCube.activate();
        _programCube.setMVP(_cameraProjectionMatrix);
        _programCube.setAccAngle(_accAngle);
        
        _programSphere.activate();
        _programSphere.setMVP(_cameraProjectionMatrix);
        _programSphere.setAccAngle(_accAngle);
    }
    
    bool prepareNextFrame() override
    {
        _accAngle += _dAngle;
        if (_accAngle >= 2*math::FLT_PI)
        {
            _accAngle -= 2*math::FLT_PI;
        }
        _cameraProjectionMatrix = _perspectiveMatrix * _camera.getViewMatrix();
        ++_nFrames;
        double dT = _timer.getDurationInMilliseconds();
        if (dT > 200.)
        {
            _updateText = 1;
            _fps = 1000.f * float(_nFrames / dT);
            _nFrames = 0;
            _timer.start();
        }
        
        if (_updateText != 0)
        {
            char tmp[64];
            
            gui::gl::Font& fnt3 = _text.getFont(2);
            fnt3.reset();
            //add frame size info
            {
                int len = td::format(tmp, "Visible frame size:\n(%d x %d)", (int) _frameSize.width, (int) _frameSize.height);
//                int len = SNPRINTF(tmp, 64, _TRUNCATE, "Visible frame size:\n(%d x %d)", (int) _frameSize.width, (int) _frameSize.height);
                gui::gl::Font::Format format;
                format.scale = _font3Scale;
                format.colorID = td::ColorID::LemonChiffon;
                format.hAlignment = td::HAlignment::Right;
                gui::gl::Point2d pt(_frameSize.width-10, 10);
                fnt3.addString(pt, tmp, len, format);
            }
            
            //add FPS info
            {
                int len = td::format(tmp, "FPS:\n%.1f", _fps);
//                int len = SNPRINTF(tmp, 64, _TRUNCATE, "FPS:\n%.1f", _fps);
                gui::gl::Font::Format format;
                format.scale = _font3Scale;
                format.colorID = td::ColorID::DarkRed;
                format.hAlignment = td::HAlignment::Center;
                gui::gl::Point2d pt(_frameSize.width/2.0f, 10);
                fnt3.addString(pt, tmp, len, format);
            }
            
            _updateText = 0;
        }
        
        return true;
//        reDraw();
    }
    
    void onResize(const gui::Size& newSize) override
    {
        if (newSize.width == 0)
            return;
        if (newSize.height == 0)
            return;
        
        CameraView::onResize(newSize);
        
        _text.setFrameSize(newSize);
        _updateText = 1; //Set flag to recreate text
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        // Clear
        gui::gl::Context::clear(td::ColorID::Black);
        gui::gl::Context::clear({gui::gl::Context::Clear::Color, gui::gl::Context::Clear::Depth});
        
        _skyBox.render();
        
        gui::gl::Context::enable(gui::gl::Context::Flag::DepthTest);
        gui::gl::Context::enable(gui::gl::Context::Flag::CullFace);
        _programCube.activate();
        
        _programCube.setAccAngle(_accAngle);
        if (_usedTexture == 0)
            _programCube.setTextureUniforms(0, 1);
        else
            _programCube.setTextureUniforms(2, 3);
        
        _programCube.execute();
        
        _programSphere.activate();
        
        _programSphere.setAccAngle(_accAngle);
        _programSphere.setTextureUniforms(4, 5);
        
        _programSphere.execute();

         //Unbind program
        _programSphere.deActivate();
        
        _text.render();
    }

public:
    ViewCubeMap()
    : _camera(glm::vec3(0.0, 0.0, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f))
    {
        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes, gui::gl::DepthBuffer::Size::B2);
        reqContext.setPreferredFrameRateRange(60, 60); //Limit FPS to 60 FPS
        createContext(reqContext);
        setCamera(&_camera);
    }
    
    ~ViewCubeMap()
    {
        makeCurrentContext();
    }
    
    void updateSpeed(float val)
    {
        _dAngle = val;
    }
    
    void setFont3ScaleFactor(float scale)
    {
        if (_font3Scale != scale)
        {
            _updateText = 1; //Set flag to recreate text
            _font3Scale = scale;
        }
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
            td::UINT4 dlgID = 56;
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
