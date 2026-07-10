// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ISolver.h
 *  @brief Template interface for numerical solvers (NL, WLS, ODE, DAE) over arbitrary value types.
 */
#pragma once
//#include "Naming.h"
//#include "TokenizedModel.h"
#include <td/Types.h>
#include <td/String.h>
#include "ISolutionBuffer.h"
#include "ISignal.h"
#include <complex>
#include "ITime.h"
#include <cnt/SafeFullVector.h>
#include <math/StatsManager.h>
#include "ILog.h"
#include <sc/Token.h>
#include "SolutionOptions.h"
#include <mu/IProgressor.h>

namespace sc
{

class Naming;
class TokenizedModel;

/// @brief Convert a report-level string to its Report enum value.
/// @param str String containing "Solved", "All", or "AllDetails".
/// @return Corresponding sc::Report value.
inline sc::Report toReport(const td::String& str)
{
    if (str.compare("Solved", 6))
        return sc::Report::Solved;
    else if (str.compare("All", 3))
        return sc::Report::All;
    return sc::Report::AllDetails;
}

/// @brief Indicates whether a solver instance is the main model or a sub-model.
enum class SubModelType : td::BYTE {Main=0, InitsParent, SubModel}; //controlled through attribute initMain=true/false

//attribs that are of interest to outside
/// @brief Solver attributes exposed to external callers.
using SolverAttribs = struct _SolAttr
{
    sc::SubModelType subModelType = SubModelType::Main; ///< Role of this solver in the model hierarchy.//controlled through attribute initMain=true/false
    sc::SolverType type = SolverType::Unknown;          ///< Numerical method type of this solver.//Constructor of solver
    td::BYTE alwaysOn = 0;                              ///< Non-zero if the solver runs unconditionally.//controlled through attribute alwaysOn=true/false
};

/// @brief Template interface for a numerical solver operating on values of type TSolType.
template <class TSolType>
class ISolver
{
public:
    /// @brief Aggregate of result metadata produced after each solve step.
    using ResultInfo = struct _ri
    {
        const TSolType* pSymbolValues;                              ///< Pointer to the flat symbol value array.
        const cnt::SafeFullVector<sc::SymbolInfo>* pSymbolInfo;     ///< Metadata for each symbol.
        cnt::SafeFullVector<td::UINT4> outSymbols;                  ///< Indices of symbols designated as outputs.
        double requiredEps;                                         ///< Convergence tolerance requested by the caller.
        double achievedEps;                                         ///< Actual convergence tolerance reached.
        double t;                                                   ///< Current simulation time.
        const td::String* pGroupName = nullptr;                     ///< Name of the model group currently being solved.
        td::UINT4 nVars;                                            ///< Number of state variables.
        td::UINT4 nParams;                                          ///< Number of parameters.
        td::UINT4 repeatNumber = 0;                                 ///< Current repeat index (0-based).
        td::UINT4 nSuccessfulRepeats = 0;                           ///< Count of successfully completed repeats.
        td::UINT4 totalIterations = 0;                              ///< Cumulative iteration count across all repeats.
        td::UINT2 outerIteration = 0;                               ///< Current outer (base) iteration number.
        td::UINT2 baseIteration = 0;                                ///< Current base iteration number within an outer pass.
        SolverType solverType;                                      ///< Numerical method used by this solver.
        Solution solutionStatus;                                    ///< Outcome of the most recent solve call.
        sc::ReportRate reportRate = sc::ReportRate::InitialAndIterationsAndFinal; ///< Frequency at which results are reported.
    };

    /// @brief Attach a ResultInfo structure that will be populated during solving.
    /// @param pRI Pointer to the ResultInfo to fill.
    virtual void setResultsInfo(ResultInfo* pRI) = 0;

    /// @brief Return the number of variables in the model.
    /// @return Variable count.
    virtual int getNumberVariables() = 0;
    /// @brief Return the number of parameters in the model.
    /// @return Parameter count.
    virtual int getNumberParameters() = 0;
    /// @brief Copy current variable values into the provided buffer.
    /// @param results Output buffer of at least getNumberVariables() elements.
    virtual void getResults(TSolType* results) = 0;
    /// @brief Copy current parameter values into the provided buffer.
    /// @param results Output buffer of at least getNumberParameters() elements.
    virtual void getParams(TSolType* results) = 0;

    /// @brief Return a pointer to the internal flat symbol value array.
    /// @return Pointer to symbol values.
    virtual TSolType* getSymbolValuesPtr() = 0;
    /// @brief Return a pointer to the internal variable sub-array.
    /// @return Pointer to variable values.
    virtual TSolType* getVariablesPtr() = 0;
    /// @brief Return a pointer to the internal parameter sub-array.
    /// @return Pointer to parameter values.
    virtual TSolType* getParamsPtr() = 0;
    /// @brief Return the name of a symbol by its flat index.
    /// @param index Zero-based symbol index.
    /// @return Null-terminated symbol name.
    virtual const char* getSymbolName(int index) const = 0;
    /// @brief Return the name of a parameter by its index.
    /// @param index Zero-based parameter index.
    /// @return Null-terminated parameter name.
    virtual const char* getParameterName(int index) = 0;
    /// @brief Return the name of a variable by its index.
    /// @param index Zero-based variable index.
    /// @return Null-terminated variable name.
    virtual const char* getVariableName(int index) = 0;
    /// @brief Find the index of a variable by name.
    /// @param varName Null-terminated variable name.
    /// @return Zero-based index, or negative if not found.
    virtual int getVariableIndex(const char* varName) = 0;
    /// @brief Set a variable value by index.
    /// @param indexVar Zero-based variable index.
    /// @param value New value to assign.
    /// @return 0 on success, negative on failure.
    virtual int setVariableValue(int indexVar, TSolType value) = 0;
    /// @brief Set a variable value by name.
    /// @param varName Null-terminated variable name.
    /// @param value New value to assign.
    /// @return 0 on success, negative on failure.
    virtual int	setVariableValue(const char* varName, TSolType value) = 0;
    /// @brief Retrieve a variable value by index.
    /// @param indexVar Zero-based variable index.
    /// @param value Output: receives the variable value.
    /// @return 0 on success, negative on failure.
    virtual int getVariableValue(int indexVar, TSolType& value) = 0;
    /// @brief Retrieve a variable value by name.
    /// @param varName Null-terminated variable name.
    /// @param value Output: receives the variable value.
    /// @return 0 on success, negative on failure.
    virtual int getVariableValue(const char* varName, TSolType& value) = 0;
    /// @brief Find the index of a parameter by name.
    /// @param varName Null-terminated parameter name.
    /// @return Zero-based index, or negative if not found.
    virtual int getParamIndex(const char* varName) = 0;
    /// @brief Set a parameter value by index.
    /// @param indexVar Zero-based parameter index.
    /// @param value New value to assign.
    /// @return 0 on success, negative on failure.
    virtual int setParamValue(int indexVar, TSolType value) = 0;
    /// @brief Set a parameter value by name.
    /// @param paramName Null-terminated parameter name.
    /// @param value New value to assign.
    /// @return 0 on success, negative on failure.
    virtual int setParamValue(const char* paramName, TSolType value) = 0;

    /// @brief Retrieve a symbol value by flat index.
    /// @param indexVar Zero-based symbol index.
    /// @param value Output: receives the symbol value.
    /// @return True on success, false if index is out of range.
    virtual bool getSymbolValue(int indexVar, TSolType& value) const = 0;
    /// @brief Retrieve a parameter value by index.
    /// @param indexVar Zero-based parameter index.
    /// @param value Output: receives the parameter value.
    /// @return 0 on success, negative on failure.
    virtual int getParamValue(int indexVar, TSolType& value) = 0;
    /// @brief Retrieve a parameter value by name.
    /// @param parName Null-terminated parameter name.
    /// @param value Output: receives the parameter value.
    /// @return 0 on success, negative on failure.
    virtual int getParamValue(const char* parName, TSolType& value) = 0;

    /// @brief Attach a progress reporter for the initialisation phase.
    /// @param pProgressor Pointer to the progress reporter.
    /// @param progPosition Position identifier within the progress display.
    virtual void setInitProgressor(mu::IProgressor* pProgressor, td::BYTE progPosition) = 0;

    /// @brief Initialise the solver from a Naming and TokenizedModel.
    /// @param pNaming Symbol naming registry.
    /// @param pTokenizedModelif Tokenised model description.
    /// @param bMainModel True if this solver represents the main (top-level) model.
    /// @return True if initialisation succeeded.
    virtual bool init(const sc::Naming* pNaming, const sc::TokenizedModel* pTokenizedModelif, bool bMainModel = true) = 0;
    //virtual int init(const td::String& fileNameOrXmlContent, SourceType source = SourceType::XMLFile) =0;

    //single solution (for steady state models)
    /// @brief Solve the steady-state model and return the solution status.
    /// @return Solution enum indicating the outcome.
    virtual sc::Solution solve() = 0;

    /// @brief Return the solution status of the most recent solve call.
    /// @return Solution enum value.
    virtual sc::Solution getSolutionStatus() const = 0;

    /// @brief Return a human-readable string for the current solution status.
    /// @return Null-terminated status string.
    virtual const char* getSolutionStatusStr() const = 0;

    //solve in interval t0:dT:t1 and report every tRep, invoke ISolutionConsumer
    /// @brief Solve a dynamic model over a time interval, reporting results to a consumer.
    /// @param t0 Start time.
    /// @param dT Integration step size.
    /// @param t1 End time.
    /// @param pConsumer Solution buffer that receives each reported step.
    /// @param repDivider Report every repDivider steps (default 1 = every step).
    /// @return True if the run completed without fatal errors.
    virtual bool solve(double t0, double dT, double t1, ISolutionBuffer* pConsumer, int repDivider = 1) = 0;

    /// @brief Return the current integration step size.
    /// @return Step size in time units.
    virtual double getIntegrationStepSize() const = 0;
    /// @brief Override the integration step size.
    /// @param dT New step size.
    virtual void setIntegrationStepSize(double dT) = 0;

    /// @brief Request that an in-progress solve be stopped at the next safe point.
    virtual void stop() = 0;

    //for dynamic models (ML: similar to the gym(nasium) python module)
    /// @brief Reset the dynamic model to the given initial time.
    /// @param t0 Start time (default 0).
    /// @return True if the reset succeeded.
    virtual bool reset(double t0 = 0) = 0;
    /// @brief Advance the simulation by the given number of steps.
    /// @param nSteps Number of integration steps to perform (default 1).
    /// @return Solution status after the advance.
    virtual sc::Solution step(unsigned int nSteps=1) = 0;

    /// @brief Set the convergence tolerance for iterative solvers.
    /// @param eps Desired precision.
    virtual void setPrecision(double eps) = 0;
    /// @brief Return the precision actually achieved in the last solve.
    /// @return Achieved epsilon value.
    virtual double getSolvedPrecision() = 0;
    /// @brief Return the number of iterations performed in the last solve.
    /// @return Iteration count.
    virtual int getIterationsNo() = 0;
    /// @brief Set the maximum number of iterations before the solver gives up.
    /// @param maxIteration Maximum iteration limit.
    virtual void setMaxIterations(int maxIteration) = 0;

    /// @brief Apply a complete set of solution options to the solver.
    /// @param options Options structure with eps, maxIter, t0, and dT settings.
    virtual void setOptions(const sc::SolutionOptions& options) = 0;

    /// @brief Return the current maximum iteration limit.
    /// @return Maximum iteration count.
    virtual int getMaxIterations() = 0;
    /// @brief Return a string description of a specific Jacobian derivation entry.
    /// @param pos Derivation index.
    /// @param EquationNo Output: equation number for this entry.
    /// @param VariableNo Output: variable number for this entry.
    /// @return Null-terminated derivation string.
    virtual const char* getDerivationStr(int pos, int &EquationNo, int &VariableNo) = 0;
    /// @brief Return the total number of Jacobian derivation entries.
    /// @return Derivation count.
    virtual int getNumberOfDerivations() = 0;
    /// @brief Fill a buffer with a human-readable description of a Jacobian entry.
    /// @param buffer Output character buffer.
    /// @param buffLen Capacity of the buffer in bytes.
    /// @param EquationNo Equation index.
    /// @param VariableNo Variable index.
    virtual void getDerDescription(char* buffer, size_t buffLen, int EquationNo, int VariableNo) = 0;
    //virtual td::String convertDAEToNL(const td::String& fileName) = 0;

    //postproc data
//    virtual void evaluatePostProcEquations() = 0;
//    virtual size_t getNumberOfPostProcEquations() const = 0;
//    virtual size_t getNumberOfPreProcEquations() const = 0;
//    virtual const char* getPostProcName(size_t iPos) const = 0;
//    virtual TSolType getPostProcValue(size_t iPos) const = 0;

    /// @brief Return the last error message as a string.
    /// @return Error description string.
    virtual td::String getLastErrorStr() const = 0;
    /// @brief Return the number of sub-models managed by this solver.
    /// @return Model count.
    virtual const td::UINT4 getNumberOfModels() const = 0;
    /// @brief Return the name of a sub-model by its identifier.
    /// @param modelID Zero-based model index.
    /// @return Reference to the model name string.
    virtual const td::String& getModelName(td::UINT4 modelID) const = 0;

    /// @brief Register a signal handler for limit and rounding events.
    /// @param pISignal Pointer to the signal handler implementation.
    virtual void setSignalHandler(sc::ISignal* pISignal) = 0;

    /// @brief Enable derivation test mode (logs symbolic Jacobian entries).
    virtual void setDerivTestMode() = 0;

    /// @brief Set the solution buffer that receives output during a dynamic solve.
    /// @param pConsumer Pointer to the ISolutionBuffer implementation.
    virtual void setConsumer(sc::ISolutionBuffer* pConsumer) = 0;

    /// @brief Control how often intermediate Newton-Raphson results are reported.
    /// @param reportPerNRIteration Report rate enum value.
    virtual void setReportResultsPerNRIteration(sc::ReportRate reportPerNRIteration) = 0;
    /// @brief Return the current Newton-Raphson intermediate report rate.
    /// @return Current ReportRate setting.
    virtual sc::ReportRate getReportResultsPerNRIteration() = 0;

    /// @brief Indicate whether this solver requires conjugate variable pairs.
    /// @return True if conjugate variables are needed.
    virtual bool requiresConjugateVars() const = 0;

    /// @brief Configure diagnostic logging of symbolic expressions.
    /// @param showDerivs True to log derivative expressions.
    /// @param forCPP True to emit C++ compatible syntax.
    /// @param showInitialValue True to include initial values in the log.
    virtual void setLogInfo(bool showDerivs, bool forCPP, bool showInitialValue) = 0;

    /// @brief Return the time-stepping interface for dynamic solvers.
    /// @return Pointer to the ITime implementation.
    virtual ITime* getITime() = 0;

    /// @brief Return the number of symbols designated as outputs.
    /// @return Output symbol count.
    virtual size_t getNoOfOutSymbols() const = 0;
    /// @brief Populate a vector with the flat indices of all output symbols.
    /// @param outVec Vector to fill with output symbol indices.
    /// @return True if output symbols exist.
    virtual bool getOutSymbols(cnt::SafeFullVector<td::UINT4>&) const = 0;

    /// @brief Return statistical performance data collected during the solve.
    /// @return Const reference to the stats manager.
    virtual const math::StatsManager<double>& getStatsManager() const = 0;

    /// @brief Return the names of all built-in functions available to the model.
    /// @return Vector of function name strings.
    virtual cnt::SafeFullVector<td::String> getFunctionNames() const = 0;
    /// @brief Return the names of all supported logical operators.
    /// @return Vector of operator name strings.
    virtual cnt::SafeFullVector<td::String> getLogicOperators() const = 0;

    /// @brief Return metadata for every symbol in the model.
    /// @return Const reference to the symbol info vector.
    virtual const cnt::SafeFullVector<sc::SymbolInfo>& getSymbolInfo() const = 0;
    /// @brief Return a pointer to the symbol info vector.
    /// @return Const pointer to the symbol info vector.
    virtual const cnt::SafeFullVector<sc::SymbolInfo>* getSymbolInfoPtr() const = 0;

    /// @brief Return the logger attached to this solver.
    /// @return Pointer to the ILog instance.
    virtual sc::ILog* getLogger() = 0;
    /// @brief Attach a logger to this solver.
    /// @param pLog Pointer to the ILog implementation.
    virtual void setLogger(sc::ILog* pLog) = 0;

    /// @brief Forward a log entry through the solver's attached logger.
    /// @param entry Log entry to forward.
    /// @param type Severity level (default: Error).
    virtual void addLog(const sc::ILog::Entry&, sc::ILog::Type type = sc::ILog::Type::Error) = 0;

    /// @brief Return the numerical method type of this solver.
    /// @return SolverType enum value.
    virtual SolverType getType() const = 0;
    /// @brief Return the attribute bundle associated with this solver.
    /// @return Const reference to SolverAttribs.
    virtual const SolverAttribs& getAttribs() const = 0;

    /// @brief Set the maximum number of repetition passes the solver may perform.
    /// @param maxRepeats Maximum repeat count (-1 for unlimited).
    virtual void setMaxRepetitions(td::INT4 maxRepeats) = 0;
    //controlled through initMain=true/false
//    virtual void setSubModelType(SubModelType subModelType) = 0;


    /// @brief Configure the output filter controlling which variable values are reported.
    /// @param outFilter Desired output filter setting.
    virtual void setOutFilter(OutFilter outFilter) = 0;
    /// @brief Release the solver and free all associated resources.
    virtual void release() = 0;
};


/// @brief Return a C-string name for the given solution status.
/// @param sol The solution status to describe.
/// @return Null-terminated status name string.
inline const char* getSolutionStatusStr(sc::Solution sol)
{
    return SolutionNames[int(sol)];
}

using IDblSolver = ISolver<double>;         ///< ISolver specialisation for real (double) arithmetic.
using ICmplxSolver = ISolver<td::cmplx>;    ///< ISolver specialisation for complex arithmetic.

using DblResultInfo = ISolver<double>::ResultInfo;      ///< ResultInfo type for real solvers.
using CmplxResultInfo = ISolver<td::cmplx>::ResultInfo; ///< ResultInfo type for complex solvers.

} //namspace sc

