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
#include "natGLX.h"
//#include <glx/GLX.h>
#include <td/String.h>
//#include "Device.h"

namespace MTL
{
class FunctionConstant;
class FunctionHandle;
class Function;
} //namespace MTL

namespace glx
{

    class RenderPipeline;
    class Device;

class NATGLX_API Function
{
    public:
    enum class Type : td::BYTE {Vertex = 1, Fragment = 2, Kernel = 3, Visible = 5, Intersection = 6};
    enum class Options : td::BYTE {None = 0, CompileToBinary = 1};
    
    class Constant
    {
        MTL::FunctionConstant* _pFunctionConstant;
        Constant(MTL::FunctionConstant*);
    public:
        Constant();
        void operator = (const Constant& constant);
        
        static Constant alloc();

        void init();

        td::String name() const;

        glx::DataType type() const;

        size_t index() const;

        bool required() const;
    };
    
    
    class Handle
    {
        friend class RenderPipeline;
    private:
        MTL::FunctionHandle* _pFunctionHandle;
        Handle(MTL::FunctionHandle*);
    public:
        Handle();
        void operator = (const Handle& constant);
        
        Function::Type functionType() const;

        td::String name() const;

        Device device() const;
    };

private:
    MTL::Function* _pFunction;
public:
    Function();
    Function(MTL::Function*);
    td::String label() const;
    void setLabel(const char* label);

    Device device() const;

    Function::Type functionType() const;

    Patch::Type patchType() const;

    size_t patchControlPointCount() const;
//
//    NS::Array*             vertexAttributes() const;
//
//    NS::Array*             stageInputAttributes() const;

    td::String name() const;

//    NS::Dictionary*        functionConstantsDictionary() const;
//
//    class ArgumentEncoder* newArgumentEncoder(NS::UInteger bufferIndex);
//
//    class ArgumentEncoder* newArgumentEncoder(NS::UInteger bufferIndex, const MTL::AutoreleasedArgument* reflection);

    Options options() const;
    
    void release();
    
    MTL::Function* getNatPtr()
    {
        return _pFunction;
    }
    
    const MTL::Function* getNatPtr() const
    {
        return _pFunction;
    }
};


} //namespace glx

