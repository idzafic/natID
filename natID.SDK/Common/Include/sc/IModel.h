// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <compiler/Definitions.h>

#ifdef MU_WINDOWS
    #ifdef MODELSOLVER_EXPORTS
        #define MODELSOLVER_API __declspec(dllexport)
    #else
        #define MODELSOLVER_API __declspec(dllimport)
    #endif
#else
    #ifdef MODELSOLVER_EXPORTS
        #define MODELSOLVER_API __attribute__((visibility("default")))
    #else
        #define MODELSOLVER_API
    #endif
#endif

#include "ILog.h"
#include <cnt/SafeFullVector.h>
#include "SolutionOptions.h"


namespace sc
{

//static solver interface
class IStatic
{
public:
    enum class Problem : unsigned char {NLE=0, WLS};
    //solves the problem
    virtual sc::Solution solve() = 0;
};

//dynamic solver interface
class IDynamic
{
public:
    enum class Problem : unsigned char {ODE=0, DAE};
    
    //set time to specified value and solves initial value problem (if defined)
    //this method should be called once per simulation
    virtual bool reset(double t0 = 0) = 0;

    //solves nStep integration steps (moves solution in time by dT * nSteps)
    virtual sc::Solution step(int nSteps = 1) = 0;
    
    //returns current integration step size (dT)
    virtual double getStepSize() const = 0;
    //sets integration step size (solution method has to be given in the model)
    virtual void setStepSize(double dT) = 0;
};

//model part (all together)
template <typename TDATA, class TSOLVERINTERFACE>
class IModel
{
public:
    enum class Type : unsigned char {StaticReal=0, StaticComplex, DynamicReal};

    //model vectors
    using NameVector = cnt::SafeFullVector<td::String>;
    using ValueVector = cnt::SafeFullVector<TDATA>;
    using IndexVector = cnt::SafeFullVector<uint32_t>;
    
    virtual void setSolutionOptions(const sc::SolutionOptions& options) = 0;
    virtual void setLogger(sc::ILog* pLog) = 0;
    
    //initialization
    virtual bool initFromFile(const td::String& fileModelName) = 0; //from .dmodl file
    virtual bool initFromString(const td::String& strModelContent) = 0; //model is in strModelContent

    //model information
    virtual size_t getNumberOfVariables() const = 0;
    virtual size_t getNumberOfParameters() const = 0;

    //variable and parameter indices (indices are required for value manipulations)
    virtual ssize_t getVariableIndex(const td::String& varName) const = 0;
    virtual ssize_t getParameterIndex(const td::String& paramName) const = 0;

    //names of all variables and parameters
    virtual NameVector getVariableNames() const = 0;
    virtual NameVector getParameterNames() const = 0;
    
    //output variable and parameter indices (specified by out="true" in the model)
    virtual IndexVector getOutputSymbolIndices() const = 0;
    virtual NameVector getOutputSymbolNames(const IndexVector& symbIndices) const = 0;
    
    //get values for all parameters
    virtual void getParameterValues(ValueVector& values) const = 0;
    //get values for parameters specified by the vector of indices
    virtual bool getParameterValues(const IndexVector& indices, ValueVector& values) const = 0;
    //get values for all variables
    virtual void getVariableValues(ValueVector& values) const = 0;
    //get values for symbols specified by the vector of indices (symbIndices)
    virtual bool getOutputSymbolValues(const IndexVector& symbIndices, ValueVector& values) const = 0;
    
    //typically can be used for dynamic model (just before reset)
    virtual bool setVariableValues(const ValueVector& values) = 0;
    //setParmeterValues can be used any time to simulate model changes
    virtual bool setParameterValues(const IndexVector& indices, const ValueVector& values) = 0;
    
    //returns interface to a solver: IDynamic or IStatic
    virtual TSOLVERINTERFACE* getSolverInterface() = 0;

    //cleans the object (destroys the object and releases memory)
    virtual void release() = 0;
};

using IRealStaticModel = IModel<double, IStatic>;
using ICmplxStaticModel = IModel<td::cmplx, IStatic>;
using IRealDynamicModel = IModel<double, IDynamic>;


//returns default terminal (consolve/cout) logger
MODELSOLVER_API sc::ILog* getConsoleLogger();

//creates real static model which returns IStatic solver interface in real valued domain
MODELSOLVER_API IRealStaticModel* createRealStaticModel(IStatic::Problem problemType, ILog* pLog = nullptr, unsigned int maxIter = 1000);
//creates complex static model which returns IStatic solver interface in complex valued domain
MODELSOLVER_API ICmplxStaticModel* createCmplxStaticModel(IStatic::Problem problemType, ILog* pLog = nullptr, unsigned int maxIter = 1000);
//creates real dynamic model which returns IDynamic solver interface in real valued domain
MODELSOLVER_API IRealDynamicModel* createRealDynamicModel(IDynamic::Problem problemType, ILog* pLog = nullptr, unsigned int maxIter = 1000);

}

