// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Function.h @brief Wrapper around Metal shader function objects, including constants and handles. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#ifdef MU_LINUX
#ifdef None
#undef None
#endif
#endif
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

/// @brief Represents a compiled Metal shader function (vertex, fragment, kernel, etc.).
class NATGLX_API Function
{
    public:
    /// @brief Enumerates the shader stage types a function may belong to.
    enum class Type : td::BYTE {Vertex = 1, Fragment = 2, Kernel = 3, Visible = 5, Intersection = 6};
    /// @brief Compilation option flags for shader functions.
    enum class Options : td::BYTE {None = 0, CompileToBinary = 1};

    /// @brief Represents a specialization constant declared in a Metal shader function.
    class Constant
    {
        MTL::FunctionConstant* _pFunctionConstant = nullptr; ///< Pointer to the native Metal function constant.

    public:

        /// @brief Copies the given constant into this instance.
        /// @param constant The source constant to copy from.
        void operator = (const Constant& constant);

        /// @brief Allocates a new uninitialized Constant object.
        /// @return A newly allocated Constant.
        static Constant alloc();

        /// @brief Initializes the constant object.
        void init();

        /// @brief Returns the name of this specialization constant as declared in the shader.
        /// @return The constant name string.
        td::String name() const;

        /// @brief Returns the data type of this specialization constant.
        /// @return The GLX data type identifier.
        glx::DataType type() const;

        /// @brief Returns the index used to identify this constant when setting values.
        /// @return The constant index.
        size_t index() const;

        /// @brief Indicates whether this constant must be provided before pipeline compilation.
        /// @return True if the constant is required.
        bool required() const;
    };


    /// @brief An opaque handle to a shader function within a linked render pipeline.
    class Handle
    {
        friend class RenderPipeline;
    private:
        MTL::FunctionHandle* _pFunctionHandle; ///< Pointer to the native Metal function handle.
        /// @brief Constructs a Handle from a native Metal function handle pointer.
        /// @param pFunctionHandle Pointer to the native handle; must not be null.
        Handle(MTL::FunctionHandle*);
    public:
        /// @brief Default-constructs a null Handle.
        Handle();

        /// @brief Copies the given handle into this instance.
        /// @param constant The source handle to copy from.
        void operator = (const Handle& constant);

        /// @brief Returns the shader stage type of the function referenced by this handle.
        /// @return The function type enum value.
        Function::Type functionType() const;

        /// @brief Returns the name of the function referenced by this handle.
        /// @return The function name string.
        td::String name() const;

        /// @brief Returns the device on which the function's pipeline was created.
        /// @return The owning Device.
        Device device() const;
    };

private:
    MTL::Function* _pFunction; ///< Pointer to the underlying Metal function object.
public:
    /// @brief Default-constructs a null Function.
    Function();

    /// @brief Constructs a Function wrapping an existing native Metal function pointer.
    /// @param pFunction Pointer to an MTL::Function object; ownership semantics follow Metal retain rules.
    Function(MTL::Function*);

    /// @brief Copy-constructs a Function, retaining the underlying native resource.
    /// @param fn The source Function to copy from.
    Function(const Function& fn);

    /// @brief Copy-assigns a Function, retaining the underlying native resource.
    /// @param fn The source Function to copy from.
    void operator = (const Function& fn);

    /// @brief Destructor; releases the underlying native Metal function.
    ~Function();

    /// @brief Returns the debug label associated with this function.
    /// @return The label string, or an empty string if none is set.
    td::String label() const;

    /// @brief Sets a debug label on this function for GPU profiling tools.
    /// @param label The label string to assign.
    void setLabel(const td::String& label);

    /// @brief Returns the device on which this function was compiled.
    /// @return The owning Device.
    Device device() const;

    /// @brief Returns the shader stage type of this function.
    /// @return The function type enum value.
    Function::Type functionType() const;

    /// @brief Returns the patch type if this function is a post-tessellation vertex shader.
    /// @return The patch type enum value.
    Patch::Type patchType() const;

    /// @brief Returns the number of control points per patch for tessellation functions.
    /// @return Control point count, or 0 if this is not a tessellation function.
    size_t patchControlPointCount() const;
//
//    NS::Array*             vertexAttributes() const;
//
//    NS::Array*             stageInputAttributes() const;

    /// @brief Returns the name of this shader function as declared in the source.
    /// @return The function name string.
    td::String name() const;

//    NS::Dictionary*        functionConstantsDictionary() const;
//
//    class ArgumentEncoder* newArgumentEncoder(NS::UInteger bufferIndex);
//
//    class ArgumentEncoder* newArgumentEncoder(NS::UInteger bufferIndex, const MTL::AutoreleasedArgument* reflection);

    /// @brief Returns the compilation options used when this function was compiled.
    /// @return The options bit-field.
    Options options() const;

    /// @brief Releases the underlying native Metal function object.
    void release();

    /// @brief Returns a mutable pointer to the underlying native Metal function.
    /// @return Raw MTL::Function pointer.
    MTL::Function* getNatPtr()
    {
        return _pFunction;
    }

    /// @brief Returns a const pointer to the underlying native Metal function.
    /// @return Const raw MTL::Function pointer.
    const MTL::Function* getNatPtr() const
    {
        return _pFunction;
    }
};


} //namespace glx
