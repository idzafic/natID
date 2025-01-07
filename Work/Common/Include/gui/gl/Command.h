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
#include <td/Types.h>
#include <gui/gl/GL.h>
#include <functional>
#include <td/ColorID.h>
#include <td/Color.h>
//#include <gui/gl/Program.h>
#include <tuple>
#include <gui/gl/Textures.h>

namespace gui
{
namespace gl
{
	
class CommandList;

class NATGL_API Command
{
    friend class CommandList;
public:
    enum class Type : td::BYTE {None = 0, DrawArrays, DrawArrayPointsWithSize, DrawArrayPatchesWithSize, DrawElementPointsWithSize, DrawElementPatchesWithSize, DrawElements, DrawArraysInstanced, DrawElementsInstanced, MVP, Uniforms, Custom};
    enum class DrawSubType : td::BYTE {None=0, DrawWithClr3, DrawWithClr3z, DrawWithClr4, DrawWithClr4z, DrawWithTexture, DrawWithTexturez, DrawInstanced, DrawWithTextureInstanced};
    
    enum class Uniform : td::BYTE {None=0, ClrID, Z, Byte, Clr3, Clr4, Mat4Ptr, Flt, Uint, Vec2, Vec3, Uvec2, Uvec3, Vec1Ptr, Vec2Ptr, Vec3Ptr, Vec4Ptr, Uvec1Ptr, Uvec2Ptr, Uvec3Ptr, Uvec4Ptr};
    
    typedef struct _additinalUniform
    {
        Uniform type;
        td::BYTE pos;
        union
        {
            void* ptrVal;
            glm::vec3 vec3;
            glm::uvec3 uvec3;
            glm::vec2 vec2;
            glm::uvec2 uvec2;
            float fVal;
            td::UINT4 u4Val;
            td::BYTE clr[4];
            td::ColorID clrID;
            td::BYTE byteVal;
            td::UINT2 u2Val;
        };
    } AdditionalUniform;
    
    // Define a typedef for the function pointer type
    typedef void (*CommandPtr)(const Command*);
    typedef struct _Range
    {
        td::UINT4 startPos;
        td::UINT4 len;
    }Range;
    
    typedef struct _Color
    {
        td::ColorID colorID = td::ColorID::Transparent;
        td::BYTE alpha = 255;
    } Color;
    
    typedef struct _cleaner
    {
        td::LUINT8 first;
        td::UINT4 second;
        td::UINT2 third;
        td::BYTE forth;
    } Cleaner;
    
    typedef struct _draw
    {
        Range range;
        Primitive primType;
        Uniform firstUniform;
        td::BYTE firstUniformPos;
        td::BYTE uniforms[4];
    } Draw;
    
    typedef struct _drawPointsWithPtSize
    {
        Range range;
        Primitive primType;
        float pointSize;
        Uniform firstUniform;
        td::BYTE firstUniformPos;
    } DrawPointsWithPtSize;
    
    typedef struct _drawPatchesWithSize
    {
        Range range;
        Primitive primType;
        td::UINT4 patchesPerVertex;
        Uniform firstUniform;
        td::BYTE firstUniformPos;
    } DrawPatchesWithSize;
    
    typedef struct _clrDraw3
    {
        Range range;
        Primitive primType;
        td::BYTE rgb[3];
        Uniform firstUniform;
        td::BYTE firstUniformPos;
        td::BYTE uniformVal;
    } ClrDraw3;
    
    typedef struct _clrDraw3z
    {
        Range range;
        Primitive primType;
        td::BYTE rgb[3];
        td::BYTE z;
        td::BYTE dummy[2];
    } ClrDraw3z;
    
    typedef struct _clrDraw4
    {
        Range range;
        Primitive primType;
        td::Color rgba;
        td::BYTE dummy[2];
    } ClrDraw4;
    
    typedef struct _clrDraw4z
    {
        Range range;
        Primitive primType;
        td::Color rgba;
        td::BYTE z;
        td::BYTE dummy;
    } ClrDraw4z;
    
    typedef struct _clrDrawTexture
    {
        Range range;
        Primitive primType;
        gui::gl::Textures::Unit texturePosition;
        td::BYTE textureID;
        Uniform firstUniform;
        td::BYTE firstUniformPos;
        td::BYTE uniforms[2];
    } ClrDrawTexture;
    
    typedef struct _clrDrawTexturez
    {
        Range range;
        Primitive primType;
        gui::gl::Textures::Unit texturePosition;
        td::BYTE textureID;
        td::BYTE z;
        Uniform firstUniform;
        td::BYTE firstUniformPos;
        td::BYTE uniform;
    } ClrDrawTexturez;
    
    typedef struct _drawInstanced
    {
        Range range;
        Primitive primType;
        td::UINT4 noOfInstances;
        td::BYTE dummy[2];
    } DrawInstanced;
    
    typedef struct _drawTextureInstanced
    {
        Range range;
        Primitive primType;
        td::UINT2 noOfInstances;
        td::BYTE textureID;
        Uniform firstUniform;
        td::BYTE firstUniformPos;
        td::BYTE uniform;
    } DrawTextureInstanced;
    
    typedef struct _mvp
    {
        glm::mat4x4* pMatrix;
        Uniform firstUniform;
        td::BYTE firstUniformPos;
        td::BYTE uniforms[5];
    } MVP;
    
    typedef struct _uniforms
    {
        Uniform firstUniform;
        td::BYTE firstUniformPos;
        td::BYTE uniforms[13];
    } Uniforms;
    
    

private:
    CommandPtr _command = nullptr;
public:
    union
    {
        Cleaner cleaner;
        Draw draw;
        DrawPointsWithPtSize drawPointsWithPointSize;
        DrawPatchesWithSize drawPatchesWithSize;
        ClrDraw3 clrDraw3;
        ClrDraw3z clrDraw3z;
        ClrDraw4 clrDraw4;
        ClrDraw4z clrDraw4z;
        ClrDrawTexture clrDrawTexture;
        ClrDrawTexturez clrDrawTexturez;
        DrawInstanced drawInstanced;
        DrawTextureInstanced drawTextureInstanced;
        MVP mvp;
        Uniforms uniforms;
    };
    
private:
    td::BYTE _type : 4 = 0;
    td::BYTE _drawSubType : 4 = 0;
private:
//    std::tuple<Type, DrawSubType> getTypeInfo() const;
    inline Type getType() const
    {
        return (Type) _type;
    }
    
    inline DrawSubType getDrawSubType() const
    {
        return (DrawSubType) _drawSubType;
    }
    
    inline bool isDraw() const;
    
    void setTypeInfo(Type type, DrawSubType drawSubType);
    
    void encodeDrawElements();
    void encodeDrawArrays();
    void encodeDrawElementsInstanced();
    void encodeDrawArraysInstanced();
    void encodeMatrix();
    
    void execute();
protected:
    void reset();
public:
    Command();
    
    //command creation
    Command& createDrawElements(gui::gl::Primitive primType, size_t indexStartPos, td::UINT4 nIndices);
    Command& createDrawArrays(gui::gl::Primitive primType, size_t vertexStartPos, td::UINT4 nVertices);
    Command& createDrawElementsInstanced(gui::gl::Primitive primType, size_t indexStartPos, td::UINT4 nIndices, td::UINT4 nInstances);
    Command& createDrawArraysInstanced(gui::gl::Primitive primType, size_t vertexStartPos, td::UINT4 nVertices, td::UINT4 nInstances);
    Command& createMVPSetter(glm::mat4x4* pUniformMatrix, AdditionalUniform* pAdditionalUniforms = nullptr, td::BYTE nUniforms=0);
    bool createUniformSetter(AdditionalUniform* pUniforms, td::BYTE nUniforms);
    
    Command& createDrawArrayPoints(size_t vertexStartPos, td::UINT4 nVertices, float pointSize);
    Command& createDrawElementPoints(size_t indexStartPos, td::UINT4 nIndices, float pointSize);
    
    Command& createDrawArrayPatches(size_t vertexStartPos, td::UINT4 nVertices, int verticesPerPatch);
    Command& createDrawElementPatches(size_t indexStartPos, td::UINT4 nIndices, int verticesPerPatch);
    //instanced drawing (command previously created with createDrawElements or createDrawArrays)
    Command& setNumberOfInstances(td::UINT4 nInstances);
    
    //Additional uniforms
    bool setAdditionalUniforms(AdditionalUniform* pUniforms, td::BYTE nUniforms=1);
    Command& setTexture(gui::gl::Textures::Unit gpuPosition, td::INT1 textureLoc);
    //RGB used in color uniform (color3 or vec3)
    Command& setColor(td::ColorID newColor);
    //RGBA used in color uniform (color4 or vec4)
    Command& setColor(td::Color colorRGBA);
    Command& setColor(td::ColorID newColor, td::BYTE alpha);
    Command& setColor(td::ColorID newColor, float alpha);
    
    bool encode();
};


} //namespace gl
} //namespace gui

