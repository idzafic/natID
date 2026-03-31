//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/Program.h>
#include <gui/gl/Textures.h>

class ProgramSphere : public Program
{
protected:
    
public:
    ProgramSphere()
    {
    }
    
    void setTextureUniforms(td::BYTE texturePos1, td::BYTE texturePos2)
    {
        _pTextures->setUniform(_uniformLocations[(td::BYTE) Location::Texture1], gui::gl::Textures::Unit::T2, texturePos1);
        _pTextures->setUniform(_uniformLocations[(td::BYTE) Location::Texture2], gui::gl::Textures::Unit::T3, texturePos2);
    }
};
