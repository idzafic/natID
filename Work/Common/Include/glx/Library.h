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
#include "Device.h"


namespace MTL
{
class CompileOptions;
class Library;
} //namespace MTL

namespace glx
{

    class Function;

class NATGLX_API Library
{
    friend class Device;
    
    public:
    enum class Type : td::BYTE { Executable = 0, Dynamic = 1};
    class CompileOptions
    {
        friend class Device;
    public:
        enum class Language : td::UINT4 {
            Version1_0 = 65536,
            Version1_1 = 65537,
            Version1_2 = 65538,
            Version2_0 = 131072,
            Version2_1 = 131073,
            Version2_2 = 131074,
            Version2_3 = 131075,
            Version2_4 = 131076
        };
    private:
        MTL::CompileOptions* _pCompileOptions;
        CompileOptions(MTL::CompileOptions* pCompileOptions);
    public:
        // constructor with vectors
        CompileOptions();
        void operator = (const CompileOptions& compileOptions);
        
        static CompileOptions alloc();

       void init();

    //    NS::Dictionary*              preprocessorMacros() const;
    //    void                         setPreprocessorMacros(const NS::Dictionary* preprocessorMacros);

        bool fastMathEnabled() const;
        void setFastMathEnabled(bool fastMathEnabled);

        Language languageVersion() const;
        void setLanguageVersion(Language languageVersion);

        Library::Type libraryType() const;
        void setLibraryType(Library::Type libraryType);

        td::String installName() const;
        void setInstallName(const char* installName);

//        NS::Array*                   libraries() const;
//        void                         setLibraries(const NS::Array* libraries);

        bool preserveInvariance() const;
        void setPreserveInvariance(bool preserveInvariance);
        
        void release();
    };
    
private:
    MTL::Library* _pLibrary;
    Library(MTL::Library*);
public:
    Library();
    void operator = (const Library& lib);
    
//   void newFunction(const char* pFunctionName, const class FunctionConstantValues* pConstantValues, const std::function<void(Function* pFunction, NS::Error* pError)>& completionHandler);
//
//    void             newFunction(const class FunctionDescriptor* pDescriptor, const std::function<void(Function* pFunction, NS::Error* pError)>& completionHandler);
//
//    void             newIntersectionFunction(const class IntersectionFunctionDescriptor* pDescriptor, const std::function<void(Function* pFunction, NS::Error* pError)>& completionHandler);

    td::String label() const;
    void setLabel(const char* label);

    class Device device() const;

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

    Library::Type type() const;

    td::String installName() const;
    
    bool isOk() const;
    
    void release();
};


} //namespace glx

