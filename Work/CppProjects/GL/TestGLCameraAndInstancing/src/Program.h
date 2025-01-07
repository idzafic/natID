//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/Program.h>
#include <gui/gl/Textures.h>

class Program : public gui::gl::Program
{
protected:
    enum class Location : td::BYTE {Angle, TrMatrix, Texture1, Texture2};
    
    //if linking is ok, this method is called
    void onLinked() override
    {
        reserveUniforms(4);
        _matrixLocation = getUniformLocation("cameraWithProjection");
        _uniformLocations[(td::BYTE) Location::Angle] = getUniformLocation("accAngle");
        _uniformLocations[(td::BYTE) Location::TrMatrix] = _matrixLocation;
        _uniformLocations[(td::BYTE) Location::Texture1] = getUniformLocation("texture1");
        _uniformLocations[(td::BYTE) Location::Texture2] = getUniformLocation("texture2");
        assert(uniformLocationsAreOk());
    }
    
    void onError() override
    {
        assert(false);
    }
    
public:
    Program()
    {
    }
    
    //set MVP matrix uniform
    void setMVP(const glm::mat4x4& mvp)
    {
        setUniformValue(_matrixLocation, mvp);
    }
    
    void setAccAngle(float accAngle)
    {
        setUniformValue(_uniformLocations[(td::BYTE) Location::Angle], accAngle);
    }
    
    void setTextureUniforms(td::BYTE texturePos1, td::BYTE texturePos2)
    {
        _pTextures->setUniform(_uniformLocations[(td::BYTE) Location::Texture1], gui::gl::Textures::Unit::T0, texturePos1);
        _pTextures->setUniform(_uniformLocations[(td::BYTE) Location::Texture2], gui::gl::Textures::Unit::T1, texturePos2);
    }
};
