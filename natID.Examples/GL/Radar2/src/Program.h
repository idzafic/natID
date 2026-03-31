//
//  Created by Izudin Dzafic on 11/04/2025.
//
#pragma once
#include <gui/gl/Program.h>
#include <gui/gl/View.h>
#include <glm/glm.hpp>



class Program : public gui::gl::Program
{
    
protected:
    
    //if linking is ok, this method is called
    void onLinked() override
    {
        reserveUniforms(4);

        _uniformLocations[0] = getUniformLocation("leftClick");
        _uniformLocations[1] = getUniformLocation("rightClick");
        _uniformLocations[2] = getUniformLocation("viewportSize");
        _uniformLocations[3] = getUniformLocation("animStep");;
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
    
    //uniform setters
    void setLeftClick(const gui::Point& pt)
    {
        glm::vec2 leftClickPos = { pt.x, pt.y };
        setUniformValue(_uniformLocations[0], leftClickPos);
    }
    
    void setRightClick(const gui::Point& pt)
    {
        glm::vec2 rightClickPos = {pt.x, pt.y};
        setUniformValue(_uniformLocations[1], rightClickPos);
    }
    
    void setViewPortSize(const gui::Size& sz)
    {
        glm::vec2 vpSize = {sz.width, sz.height};
        setUniformValue(_uniformLocations[2], vpSize);
    }
    
    void setVAnimStep(float animStep)
    {
        setUniformValue(_uniformLocations[3], animStep);
    }
};
