// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/natGL.h>
#include <gui/gl/GL.h>
#include <td/String.h>
#include <glm/glm.hpp>
#include <td/ColorID.h>
#include <cnt/SafeFullVector.h>
#include <gui/gl/Textures.h>
#include <gui/gl/CommandList.h>

namespace gui
{
namespace gl
{
	
class View;
class Buffer;
//class Command;

//class Textures;
class ProgHelper;
class SkyBox;

class NATGL_API Program
{
    friend class View;
    friend class Command;
    friend class ProgHelper;
    friend class SkyBox;
protected:
    gui::gl::CommandList _commandList; //if empty, commmands from _pBuffer will be executed
    gui::gl::Buffer* _pBuffer = nullptr;
    gui::gl::Textures* _pTextures = nullptr;
    cnt::SafeFullVector<td::INT4> _uniformLocations; //max 255
    cnt::SafeFullVector<td::INT4> _uniformLocationsForMatrices; //max 255
    td::UINT4 _programID = 0;
    td::INT4 _color3Location = -1;
    td::INT4 _color4Location = -1;
    td::INT4 _matrixLocation = -1;
private:
    static td::UINT4 _link(const char* vertexResIDOrText, const char* fragmentResIDOrText, const char* tessCtrlShaderResID = nullptr, const char* tessEvalShaderResID = nullptr, const char* geometryShaderID = nullptr);

protected:
    //check shader compilation
    static bool isCompilationOK(GLSL::Type glslType, td::UINT4 shaderID, const char* shaderFileName);
    //check GLSL shader linking into a program
    static bool isLinkingOK(td::UINT4 programID, const char* vertexShaderFileName, const char* fragmentShaderFileName);
    
    //flloating point square matrices
    void setUniformValue(td::UINT4 uniformIndex, const glm::mat4x4& mat);
    void setUniformValue(td::UINT4 uniformIndex, const glm::mat3x3& mat);
    
    //floating point vectors
    void setUniformValue(td::UINT4 uniformIndex, const glm::vec1& vec);
    void setUniformValue(td::UINT4 uniformIndex, const glm::vec2& vec);
    void setUniformValue(td::UINT4 uniformIndex, const glm::vec3& vec);
    void setUniformValue(td::UINT4 uniformIndex, const glm::vec4& vec);
    
    //unisgned int (4bytes) vectors
    void setUniformValue(td::UINT4 uniformIndex, const glm::uvec1& vec);
    void setUniformValue(td::UINT4 uniformIndex, const glm::uvec2& vec);
    void setUniformValue(td::UINT4 uniformIndex, const glm::uvec3& vec);
    void setUniformValue(td::UINT4 uniformIndex, const glm::uvec4& vec);
    
    //scalars
    void setUniformValue(td::UINT4 uniformIndex, td::UINT4 val);
    void setUniformValue(td::UINT4 uniformIndex, float val);
    
    //sets RGB values from clrID using glm::vec3
    void setUniformValue(td::UINT4 uniformIndex, td::ColorID clrID);
    //sets RGB values from clrID and A from alpha using glm::vec4
    void setUniformValue(td::UINT4 uniformIndex, td::ColorID clrID, td::BYTE alpha);
    
    void setUniformValue(td::UINT4 uniformIndex, gui::gl::Texture::Type textureType, td::UINT4 textureID, gui::gl::Textures::Unit samplerUnit);
    
    //common (most used) setters
    void setColor3(const glm::vec3& color3); //use _colorSetter3
    void setColor4(const glm::vec4& color4); //use _colorSetter4
    void setMVP(const glm::mat4x4& mat4); //use _matrixSetter
    
    //returns index of a uniform
    td::INT4 getUniformLocation(const char* uniformName);
    
    bool getUniformLocationsForMatrices(const char** pMatrixNames, size_t nMatrices);
    void setUniformValuesForMatrices(const glm::mat4x4** pMatrices, size_t nMatrices);
    void setUniformValuesForMatrices(const glm::mat4x4* pMatrices, size_t nMatrices);
    void setUniformValuesForMatrices(const cnt::SafeFullVector<glm::mat4x4>& matrices, size_t nToSet = 0); //0-means all, otherwise first nToSet
    
    gui::gl::Textures* getTextures();
    
    void reserveUniforms(td::BYTE nUniforms);
    bool uniformLocationsAreOk() const;
    
    //if linking is ok onLinked is called
    virtual void onLinked();
    //if linking is NOT ok onError is called
    virtual void onError();
    
public:
    Program();
    ~Program();
    //compile and link using text resources for vertex and fragment shader and optional geometry shader
    bool link(const char* vertexShaderResID, const char* fragmentShaderResID, const char* geometryShaderResID = nullptr);
    //compile and link using text resources for vertex, fragment, tesselation, and tess control shader and optional geometry shader
    bool link(const char* vertexShaderResID, const char* fragmentShaderResID, const char* tessCtrlShaderResID, const char* tessEvalShaderResID, const char* geometryShaderResID = nullptr);
    
    td::UINT4 getID() const;

    //check if program is ok
    bool isOK() const;
    //get accumulated error
    static td::String getLastError();
    //set buffer which will be activated when Program activates
    void setBuffer(gui::gl::Buffer* pBuffer);

    void setTextures(gui::gl::Textures* pTextures);

    //buffer layoutID to be used with
    void activate() const;
    
    void execute(); //requires previous call to activate, execute own commands if any, otherwise execute commands from buffer
    void execute(size_t startPos, size_t count); //requires previous call to activate, it will execute commands from buffer
    
    //commands
    //reserve number of commands
    void reserveCommands(size_t nCommands);
    //places command on the list and links function pointers
    bool encode(gui::gl::Command* pComm);
    //removes pComm from list if it is last in the list
    bool remove(gui::gl::Command* pComm);
    //creates empy command
    gui::gl::Command* createCommand();
    
    void deActivate() const;
    void release();
    static void useNone();
};

} //namespace gl
} //namespace gui

