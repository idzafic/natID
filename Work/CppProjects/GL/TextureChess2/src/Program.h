//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/Program.h>

class Program : public gui::gl::Program
{
    td::INT4 _textureSamplerLoc = -1;
protected:
    
    //if linking is ok, this method is called
    void onLinked() override
    {
        _matrixLocation = getUniformLocation("mvp");
        _textureSamplerLoc = getUniformLocation("textureSampler");
        assert(_matrixLocation >= 0 && _textureSamplerLoc >= 0);
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
    
//    void setTextureUnit()
//    {
//        td::UINT4 textureUnit = 0;
//        setUniformValue(_textureSamplerLoc, textureUnit);
//    }
};
