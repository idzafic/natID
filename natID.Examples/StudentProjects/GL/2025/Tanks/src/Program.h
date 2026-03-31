
#pragma once
#include <gui/gl/Program.h>

class Program : public gui::gl::Program
{
    float ugao = 0;
protected:
    
    //if linking is ok, this method is called
    void onLinked() override
    {
        _matrixLocation = getUniformLocation("mvp");

       // ugao = getUniformLocation("ugaoX");

        reserveUniforms(2);

        _uniformLocations[0] = getUniformLocation("defaultColor");

        _uniformLocations[1] = getUniformLocation("ugaoX");

        //assert(uniformLocationsAreOk());
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

    void setDefaultColor(const glm::vec3& color)
    {
        setUniformValue(_uniformLocations[0], color);

    }

    void setUgao(float x) {
        setUniformValue(_uniformLocations[1], x);
    }
};
