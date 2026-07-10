// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IModel.h
 @brief Interfaces for static and dynamic solver models used in numerical simulation. */
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
/// @brief Interface for static (steady-state) solvers supporting nonlinear and weighted least-squares problems.
class IStatic
{
public:
    /// @brief Enumeration of supported static problem types.
    enum class Problem : unsigned char {NLE=0, ///< Nonlinear equations
                                        WLS    ///< Weighted least-squares
                                       };
    //solves the problem
    /// @brief Solves the static problem and returns the solution status.
    /// @return Solution status indicating success or failure.
    virtual sc::Solution solve() = 0;
};

//dynamic solver interface
/// @brief Interface for dynamic (time-domain) solvers supporting ODE and DAE problems.
class IDynamic
{
public:
    /// @brief Enumeration of supported dynamic problem types.
    enum class Problem : unsigned char {ODE=0, ///< Ordinary differential equations
                                        DAE    ///< Differential-algebraic equations
                                       };

    //set time to specified value and solves initial value problem (if defined)
    //this method should be called once per simulation
    /// @brief Sets the simulation time to t0 and solves the initial value problem.
    /// @param t0 Initial time value (default 0).
    /// @return True if initialization succeeded, false otherwise.
    virtual bool reset(double t0 = 0) = 0;

    //solves nStep integration steps (moves solution in time by dT * nSteps)
    /// @brief Advances the solution by the specified number of integration steps.
    /// @param nSteps Number of integration steps to perform (default 1).
    /// @return Solution status after performing the steps.
    virtual sc::Solution step(int nSteps = 1) = 0;

    //returns current integration step size (dT)
    /// @brief Returns the current integration step size.
    /// @return Current step size dT.
    virtual double getStepSize() const = 0;
    //sets integration step size (solution method has to be given in the model)
    /// @brief Sets the integration step size.
    /// @param dT New step size value.
    virtual void setStepSize(double dT) = 0;
};

//model part (all together)
/// @brief Generic model interface combining data access, initialization, and solver access.
/// @tparam TDATA Numeric data type used for variable and parameter values (e.g., double or complex).
/// @tparam TSOLVERINTERFACE Solver interface type (IStatic or IDynamic).
template <typename TDATA, class TSOLVERINTERFACE>
class IModel
{
public:
    /// @brief Enumeration of supported model types.
    enum class Type : unsigned char {StaticReal=0,   ///< Real-valued static model
                                     StaticComplex,  ///< Complex-valued static model
                                     DynamicReal     ///< Real-valued dynamic model
                                    };

    //model vectors
    using NameVector = cnt::SafeFullVector<td::String>; ///< Vector of symbol names.
    using ValueVector = cnt::SafeFullVector<TDATA>;     ///< Vector of typed data values.
    using IndexVector = cnt::SafeFullVector<uint32_t>;  ///< Vector of symbol indices.

    /// @brief Configures solution options for the model solver.
    /// @param options Solution options structure containing tolerances and iteration limits.
    virtual void setSolutionOptions(const sc::SolutionOptions& options) = 0;

    /// @brief Attaches a logger for receiving diagnostic messages from the solver.
    /// @param pLog Pointer to the logger instance, or nullptr to disable logging.
    virtual void setLogger(sc::ILog* pLog) = 0;

    //initialization
    /// @brief Initializes the model from a .dmodl file on disk.
    /// @param fileModelName Path to the model file.
    /// @return True if initialization succeeded, false otherwise.
    virtual bool initFromFile(const td::String& fileModelName) = 0; //from .dmodl file

    /// @brief Initializes the model from a model definition provided as a string.
    /// @param strModelContent String containing the model definition.
    /// @return True if initialization succeeded, false otherwise.
    virtual bool initFromString(const td::String& strModelContent) = 0; //model is in strModelContent

    //model information
    /// @brief Returns the total number of variables in the model.
    /// @return Number of model variables.
    virtual size_t getNumberOfVariables() const = 0;

    /// @brief Returns the total number of parameters in the model.
    /// @return Number of model parameters.
    virtual size_t getNumberOfParameters() const = 0;

    //variable and parameter indices (indices are required for value manipulations)
    /// @brief Returns the index of a variable by its name.
    /// @param varName Name of the variable to look up.
    /// @return Zero-based index, or -1 if not found.
    virtual ssize_t getVariableIndex(const td::String& varName) const = 0;

    /// @brief Returns the index of a parameter by its name.
    /// @param paramName Name of the parameter to look up.
    /// @return Zero-based index, or -1 if not found.
    virtual ssize_t getParameterIndex(const td::String& paramName) const = 0;

    //names of all variables and parameters
    /// @brief Returns the names of all model variables.
    /// @return Vector of variable name strings.
    virtual NameVector getVariableNames() const = 0;

    /// @brief Returns the names of all model parameters.
    /// @return Vector of parameter name strings.
    virtual NameVector getParameterNames() const = 0;

    //output variable and parameter indices (specified by out="true" in the model)
    /// @brief Returns the indices of symbols marked as output in the model definition.
    /// @return Vector of output symbol indices.
    virtual IndexVector getOutputSymbolIndices() const = 0;

    /// @brief Returns the names of symbols identified by the given index vector.
    /// @param symbIndices Vector of symbol indices.
    /// @return Vector of corresponding symbol name strings.
    virtual NameVector getOutputSymbolNames(const IndexVector& symbIndices) const = 0;

    //get values for all parameters
    /// @brief Retrieves the current values of all parameters.
    /// @param values Output vector populated with all parameter values.
    virtual void getParameterValues(ValueVector& values) const = 0;

    //get values for parameters specified by the vector of indices
    /// @brief Retrieves the current values of parameters at specified indices.
    /// @param indices Vector of parameter indices to query.
    /// @param values Output vector populated with the corresponding values.
    /// @return True if all indices were valid and values were retrieved.
    virtual bool getParameterValues(const IndexVector& indices, ValueVector& values) const = 0;

    //get values for all variables
    /// @brief Retrieves the current values of all model variables.
    /// @param values Output vector populated with all variable values.
    virtual void getVariableValues(ValueVector& values) const = 0;

    //get values for symbols specified by the vector of indices (symbIndices)
    /// @brief Retrieves the current values of output symbols at the specified indices.
    /// @param symbIndices Vector of output symbol indices.
    /// @param values Output vector populated with the corresponding values.
    /// @return True if all indices were valid and values were retrieved.
    virtual bool getOutputSymbolValues(const IndexVector& symbIndices, ValueVector& values) const = 0;

    //typically can be used for dynamic model (just before reset)
    /// @brief Sets the initial values of all model variables (typically used before reset for dynamic models).
    /// @param values Vector of values to assign to all variables.
    /// @return True if values were applied successfully.
    virtual bool setVariableValues(const ValueVector& values) = 0;

    //setParmeterValues can be used any time to simulate model changes
    /// @brief Sets the values of parameters identified by the given indices.
    /// @param indices Vector of parameter indices to update.
    /// @param values Vector of new values corresponding to the given indices.
    /// @return True if all values were set successfully.
    virtual bool setParameterValues(const IndexVector& indices, const ValueVector& values) = 0;

    //returns interface to a solver: IDynamic or IStatic
    /// @brief Returns a pointer to the solver interface (IDynamic or IStatic).
    /// @return Pointer to the solver interface.
    virtual TSOLVERINTERFACE* getSolverInterface() = 0;

    //cleans the object (destroys the object and releases memory)
    /// @brief Releases all resources held by this model object.
    virtual void release() = 0;
};

/// @brief Convenience alias for a real-valued static model.
using IRealStaticModel = IModel<double, IStatic>;
/// @brief Convenience alias for a complex-valued static model.
using ICmplxStaticModel = IModel<td::cmplx, IStatic>;
/// @brief Convenience alias for a real-valued dynamic model.
using IRealDynamicModel = IModel<double, IDynamic>;


//returns default terminal (consolve/cout) logger
/// @brief Returns the default console logger that writes to standard output.
/// @return Pointer to the console logger instance.
MODELSOLVER_API sc::ILog* getConsoleLogger();

//creates real static model which returns IStatic solver interface in real valued domain
/// @brief Creates a real-valued static model using the specified problem type.
/// @param problemType Type of static problem (NLE or WLS).
/// @param pLog Optional logger for diagnostic output.
/// @param maxIter Maximum number of solver iterations.
/// @return Pointer to the created model; caller must call release() when done.
MODELSOLVER_API IRealStaticModel* createRealStaticModel(IStatic::Problem problemType, ILog* pLog = nullptr, unsigned int maxIter = 1000);

//creates complex static model which returns IStatic solver interface in complex valued domain
/// @brief Creates a complex-valued static model using the specified problem type.
/// @param problemType Type of static problem (NLE or WLS).
/// @param pLog Optional logger for diagnostic output.
/// @param maxIter Maximum number of solver iterations.
/// @return Pointer to the created model; caller must call release() when done.
MODELSOLVER_API ICmplxStaticModel* createCmplxStaticModel(IStatic::Problem problemType, ILog* pLog = nullptr, unsigned int maxIter = 1000);

//creates real dynamic model which returns IDynamic solver interface in real valued domain
/// @brief Creates a real-valued dynamic model using the specified problem type.
/// @param problemType Type of dynamic problem (ODE or DAE).
/// @param pLog Optional logger for diagnostic output.
/// @param maxIter Maximum number of solver iterations.
/// @return Pointer to the created model; caller must call release() when done.
MODELSOLVER_API IRealDynamicModel* createRealDynamicModel(IDynamic::Problem problemType, ILog* pLog = nullptr, unsigned int maxIter = 1000);

}
