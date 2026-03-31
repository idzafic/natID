//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/Program.h>

class Program : public gui::gl::Program
{
    td::INT4 _zoomLocation;
protected:
    
    //if linking is ok, this method is called
    void onLinked() override
    {
        _zoomLocation = getUniformLocation("zoomFactor");
        assert(_zoomLocation >= 0);
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
    void setZoom(float zoom)
    {
        setUniformValue(_zoomLocation, zoom);
    }
};
