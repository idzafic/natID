//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/Program.h>

class Program : public gui::gl::Program
{
protected:
    
    //if linking is ok, this method is called
    void onLinked() override
    {
        _matrixLocation = getUniformLocation("mvp");
#ifdef USE_NORMALS_TEXTURE
        reserveUniforms(6);
#else
        reserveUniforms(5);

#endif
        _uniformLocations[0] = getUniformLocation("texWoodFloor");
        _uniformLocations[1] = getUniformLocation("mv");
        _uniformLocations[2] = getUniformLocation("n");
        _uniformLocations[3] = getUniformLocation("v");
        _uniformLocations[4] = getUniformLocation("lightPos");


#ifdef USE_NORMALS_TEXTURE
        _uniformLocations[4] = getUniformLocation("texWoodFloorNorm"); 
#endif

        assert(uniformLocationsAreOk());
    }
    
    void onError() override
    {
        td::String lastErr = gui::gl::Program::getLastError();
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

    void setMV(const glm::mat4x4& mv)
    {
        setUniformValue(_uniformLocations[1], mv);
    }
    void setV(const glm::mat4x4& v)
    {
        setUniformValue(_uniformLocations[3], v);
    }

    void setN(const glm::mat4x4& n)
    {
        setUniformValue(_uniformLocations[2], n);
    }

    void setLightPos(const glm::vec3& lightPos)
    {
        setUniformValue(_uniformLocations[4], lightPos);
}


    void setTextureUniforms()
    {
        _pTextures->setUniform(_uniformLocations[0], gui::gl::Textures::Unit::T0, 0);
#ifdef USE_NORMALS_TEXTURE
        _pTextures->setUniform(_uniformLocations[1], gui::gl::Textures::Unit::T1, 1);
#endif
    }
};



class ProgramNoTex : public gui::gl::Program
{
protected:

    //if linking is ok, this method is called
    void onLinked() override
    {
        _matrixLocation = getUniformLocation("mvp");
        assert(_matrixLocation >= 0);
    }

    void onError() override
    {
        assert(false);
    }

public:
    ProgramNoTex()
    {
    }

    //set MVP matrix uniform
    void setMVP(const glm::mat4x4& mvp)
    {
        setUniformValue(_matrixLocation, mvp);
    }
};
