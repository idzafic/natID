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

        reserveUniforms(6);

        _uniformLocations[0] = getUniformLocation("texWoodFloor");
        _uniformLocations[1] = getUniformLocation("mv");
        _uniformLocations[2] = getUniformLocation("n");
        _uniformLocations[3] = getUniformLocation("v");
        _uniformLocations[4] = getUniformLocation("lightPos");
        _uniformLocations[5] = getUniformLocation("lightColor");

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

    void setLight(const glm::vec3& pos, const glm::vec3& color)
    {
        setUniformValue(_uniformLocations[4], pos);
        setUniformValue(_uniformLocations[5], color);

    }


    void setTextureUniforms()
    {
        _pTextures->setUniform(_uniformLocations[0], gui::gl::Textures::Unit::T0, 0);
    }
};



class ProgramOutline : public gui::gl::Program
{
protected:

    //if linking is ok, this method is called
    void onLinked() override
    {
        reserveUniforms(3);

        _uniformLocations[0] = getUniformLocation("m");
        _uniformLocations[1] = getUniformLocation("v");
        _uniformLocations[2] = getUniformLocation("p");

        assert(uniformLocationsAreOk());
    }

    void onError() override
    {
        assert(false);
    }

public:
    void setM(const glm::mat4x4& m)
    {
        setUniformValue(_uniformLocations[0], m);
    }
    void setV(const glm::mat4x4& v)
    {
        setUniformValue(_uniformLocations[1], v);
    }
    void setP(const glm::mat4x4& p)
    {
        setUniformValue(_uniformLocations[2], p);
    }

};
