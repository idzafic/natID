// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Library.h @brief Wrapper around a Metal shader library that provides access to compiled shader functions. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "natGLX.h"
#include <td/String.h>


namespace MTL
{
class CompileOptions;
class Library;
} //namespace MTL

namespace glx
{

class Function;
class Device;

/// @brief Wraps a Metal shader library and exposes factory methods for creating Function objects.
class NATGLX_API Library
{
    friend class Device;

    public:
    /// @brief Identifies whether the library is statically linked or dynamically loaded.
    enum class Type : td::BYTE { Executable = 0, Dynamic = 1};

    /// @brief Encapsulates Metal shader compilation options used when building a Library from source.
    class CompileOptions
    {
        friend class Device;
    public:
        /// @brief Metal shading language version constants used during compilation.
        enum class Language : td::UINT4 {
            Version1_0 = 65536,  ///< Metal Shading Language version 1.0.
            Version1_1 = 65537,  ///< Metal Shading Language version 1.1.
            Version1_2 = 65538,  ///< Metal Shading Language version 1.2.
            Version2_0 = 131072, ///< Metal Shading Language version 2.0.
            Version2_1 = 131073, ///< Metal Shading Language version 2.1.
            Version2_2 = 131074, ///< Metal Shading Language version 2.2.
            Version2_3 = 131075, ///< Metal Shading Language version 2.3.
            Version2_4 = 131076  ///< Metal Shading Language version 2.4.
        };
    private:
        MTL::CompileOptions* _pCompileOptions; ///< Pointer to the native Metal compile options object.

        /// @brief Constructs CompileOptions wrapping an existing native pointer.
        /// @param pCompileOptions Pointer to an MTL::CompileOptions object.
        CompileOptions(MTL::CompileOptions* pCompileOptions);
    public:
        // constructor with vectors
        /// @brief Default-constructs a CompileOptions object with Metal defaults.
        CompileOptions();

        /// @brief Copy-assigns compile options from another instance.
        /// @param compileOptions The source options to copy.
        void operator = (const CompileOptions& compileOptions);

        /// @brief Allocates a new uninitialized CompileOptions object.
        /// @return A newly allocated CompileOptions.
        static CompileOptions alloc();

        /// @brief Initializes this object with default Metal compilation settings.
        void init();

    //    NS::Dictionary*              preprocessorMacros() const;
    //    void                         setPreprocessorMacros(const NS::Dictionary* preprocessorMacros);

        /// @brief Returns whether fast-math optimizations are enabled for this compilation.
        /// @return True if fast-math is enabled.
        bool fastMathEnabled() const;

        /// @brief Enables or disables fast-math optimizations.
        /// @param fastMathEnabled True to enable fast-math, false to disable.
        void setFastMathEnabled(bool fastMathEnabled);

        /// @brief Returns the Metal shading language version targeted by this compilation.
        /// @return The Language version enum value.
        Language languageVersion() const;

        /// @brief Sets the Metal shading language version for compilation.
        /// @param languageVersion The desired language version.
        void setLanguageVersion(Language languageVersion);

        /// @brief Returns whether the compiled output will be an executable or dynamic library.
        /// @return The Library::Type enum value.
        Library::Type libraryType() const;

        /// @brief Sets whether the compiled output is an executable or dynamic library.
        /// @param libraryType The desired library type.
        void setLibraryType(Library::Type libraryType);

        /// @brief Returns the install name used when creating a dynamic library.
        /// @return The install name string.
        td::String installName() const;

        /// @brief Sets the install name used when linking against a dynamic library.
        /// @param installName Null-terminated install name string.
        void setInstallName(const char* installName);

//        NS::Array*                   libraries() const;
//        void                         setLibraries(const NS::Array* libraries);

        /// @brief Returns whether invariance is preserved across shader compilations.
        /// @return True if invariance is preserved.
        bool preserveInvariance() const;

        /// @brief Enables or disables preservation of shader invariance.
        /// @param preserveInvariance True to preserve invariance.
        void setPreserveInvariance(bool preserveInvariance);

        /// @brief Releases the underlying native compile options object.
        void release();
    };

private:
    MTL::Library* _pLibrary; ///< Pointer to the underlying native Metal library object.

    /// @brief Constructs a Library wrapping an existing native Metal library pointer.
    /// @param pLibrary Pointer to an MTL::Library object.
    Library(MTL::Library*);
public:
    /// @brief Default-constructs a null Library.
    Library();

    /// @brief Copy-constructs a Library, retaining the underlying native resource.
    /// @param lib The source Library to copy from.
    Library(const Library& lib);

    /// @brief Copy-assigns a Library, retaining the underlying native resource.
    /// @param lib The source Library to copy from.
    void operator = (const Library& lib);

    /// @brief Destructor; releases the underlying native Metal library.
    ~Library();

//   void newFunction(const char* pFunctionName, const class FunctionConstantValues* pConstantValues, const std::function<void(Function* pFunction, NS::Error* pError)>& completionHandler);
//
//    void             newFunction(const class FunctionDescriptor* pDescriptor, const std::function<void(Function* pFunction, NS::Error* pError)>& completionHandler);
//
//    void             newIntersectionFunction(const class IntersectionFunctionDescriptor* pDescriptor, const std::function<void(Function* pFunction, NS::Error* pError)>& completionHandler);

    /// @brief Returns the debug label associated with this library.
    /// @return The label string.
    td::String label() const;

    /// @brief Sets a debug label on this library for GPU profiling tools.
    /// @param label Null-terminated label string.
    void setLabel(const char* label);

    /// @brief Returns the device on which this library was created.
    /// @return The owning Device.
    Device device() const;

    /// @brief Looks up and returns a Function with the given name from this library.
    /// @param functionName Null-terminated name of the shader function to look up.
    /// @return A Function object wrapping the located shader function.
    Function newFunction(const char* functionName);
//
//    class Function  newFunction(const NS::String* name, const class FunctionConstantValues* constantValues, NS::Error** error);
//
//    void             newFunction(const NS::String* name, const class FunctionConstantValues* constantValues, void (^completionHandler)(MTL::Function*, NS::Error*));
//
//    void             newFunction(const class FunctionDescriptor* descriptor, void (^completionHandler)(MTL::Function*, NS::Error*));
//
//    class Function  newFunction(const class FunctionDescriptor* descriptor, NS::Error** error);
//
//    void             newIntersectionFunction(const class IntersectionFunctionDescriptor* descriptor, void (^completionHandler)(MTL::Function*, NS::Error*));
//
//    class Function  newIntersectionFunction(const class IntersectionFunctionDescriptor* descriptor, NS::Error** error);

//    void getFunctionNames(const cnt::SafeFullVector<td::String>&) const;

    /// @brief Returns whether this library is an executable or dynamic library.
    /// @return The Library::Type enum value.
    Library::Type type() const;

    /// @brief Returns the install name of this library (relevant for dynamic libraries).
    /// @return The install name string.
    td::String installName() const;

    /// @brief Returns whether this library object wraps a valid native Metal library.
    /// @return True if the library is valid and ready to use.
    bool isOk() const;

    /// @brief Releases the underlying native Metal library object.
    void release();
};


} //namespace glx
