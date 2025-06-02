// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

namespace sc
{

class Naming;
class TokenizedModel;

enum class SourceType : unsigned char {XMLFile = 0, XMLMemory, ModlFile};

enum class Solution : unsigned char {OK=0, BaseOK, Iterating, LimitFix, NewPass, EmptyMatrix, FactorizationError, SolutionError, Oscillation, MaximumIteration, SubmodelFailed};
constexpr const char* SolutionNames[] = {"OK", "BaseOK", "IterBase", "LimitFix", "NewPass", "EmptyMatrix", "FactorizationError", "SolutionError", "Oscillation", "MaxIters",  "SubmodelFailed"};

enum class OutFilter : unsigned char {Minimum=0, InitialValues = 1, ValuesForIteration=2, InitialAndIterationValues=3};
enum class ReportRate : unsigned char {None = 0, InitialAndFinal, InitialAndIterationsAndFinal};

//enum class SolverType : unsigned char {NL=0, WLS, DAE};
enum class SolverType : td::BYTE {Deriv = 0, NL, WLS, ODE, DAE, Unknown};
constexpr const char* SolverTypeNames[] = {"Deriv", "NL", "WLS", "ODE", "DAE"};

enum class Report : td::BYTE {Solved=0, All, AllDetails};

inline sc::Report toReport(const td::String& str)
{
    if (str.compare("Solved", 6))
        return sc::Report::Solved;
    else if (str.compare("All", 3))
        return sc::Report::All;
    return sc::Report::AllDetails;
}

enum class SubModelType : td::BYTE {Main=0, InitsParent, SubModel}; //controlled through attribute initMain=true/false

//attribs that are of interest to outside
using SolverAttribs = struct _SolAttr
{
    sc::SubModelType subModelType = SubModelType::Main; //controlled through attribute initMain=true/false
    sc::SolverType type = SolverType::Unknown; //Constructor of solver
    td::BYTE alwaysOn = 0; //controlled through attribute alwaysOn=true/false
};

template <class TSolType>
class ISolver
{
public:
    using ResultInfo = struct _ri
    {
        const TSolType* pSymbolValues;
        const cnt::SafeFullVector<sc::SymbolInfo>* pSymbolInfo;
        cnt::SafeFullVector<td::INT4> outSymbols;
        double requiredEps;
        double achievedEps;
        double t;
        const td::String* pGroupName = nullptr;
        td::UINT4 nVars;
        td::UINT4 nParams;
        td::UINT4 repeatNumber = 0;
        td::UINT4 nSuccessfulRepeats = 0;
        td::UINT4 totalIterations = 0;
        td::UINT2 outerIteration = 0;
        td::UINT2 baseIteration = 0;
        SolverType solverType;
        Solution solutionStatus;
        sc::ReportRate reportRate = sc::ReportRate::InitialAndIterationsAndFinal;
    };
    
    virtual void setResultsInfo(ResultInfo* pRI) = 0;
    
    virtual int getNumberVariables() = 0;
    virtual int getNumberParameters() = 0;
    virtual void getResults(TSolType* results) = 0;
    virtual void getParams(TSolType* results) = 0;

    virtual TSolType* getSymbolValuesPtr() = 0;
    virtual TSolType* getVariablesPtr() = 0;
    virtual TSolType* getParamsPtr() = 0;
    virtual const char* getSymbolName(int index) const = 0;
    virtual const char* getParameterName(int index) = 0;
    virtual const char* getVariableName(int index) = 0;
    virtual int getVariableIndex(const char* varName) = 0;
    virtual int setVariableValue(int indexVar, TSolType value) = 0;
    virtual int	setVariableValue(const char* varName, TSolType value) = 0;
    virtual int getVariableValue(int indexVar, TSolType& value) = 0;
    virtual int getVariableValue(const char* varName, TSolType& value) = 0;
    virtual int getParamIndex(const char* varName) = 0;
    virtual int setParamValue(int indexVar, TSolType value) = 0;
    virtual int setParamValue(const char* paramName, TSolType value) = 0;
    
    virtual bool getSymbolValue(int indexVar, TSolType& value) const = 0;
    virtual int getParamValue(int indexVar, TSolType& value) = 0;
    virtual int getParamValue(const char* parName, TSolType& value) = 0;
    
    virtual bool init(const sc::Naming* pNaming, const sc::TokenizedModel* pTokenizedModelif, bool bMainModel = true) = 0;
    virtual int init(const td::String& fileNameOrXmlContent, SourceType source = SourceType::XMLFile) =0;
    //single solution
    virtual sc::Solution solve() = 0;
    
    virtual sc::Solution getSolutionStatus() const = 0;
    
    virtual const char* getSolutionStatusStr() const = 0;
    
    //solve in interval t0:dT:t1 and report every tRep, invoke ISolutionConsumer
    virtual bool solve(double t0, double dT, double t1, ISolutionBuffer* pConsumer, int repDivider = 1) = 0;
    virtual void stop() = 0;
    
    virtual void setPrecision(double eps) = 0;
    virtual double getSolvedPrecision() = 0;
    virtual int getIterationsNo() = 0;
    virtual void setMaxIterations(int maxIteration) = 0;
    virtual int getMaxIterations() = 0;
    virtual const char* getDerivationStr(int pos, int &EquationNo, int &VariableNo) = 0;
    virtual int getNumberOfDerivations() = 0;
    virtual void getDerDescription(char* buffer, size_t buffLen, int EquationNo, int VariableNo) = 0;
    //virtual td::String convertDAEToNL(const td::String& fileName) = 0;
    
    //postproc data
//    virtual void evaluatePostProcEquations() = 0;
//    virtual size_t getNumberOfPostProcEquations() const = 0;
//    virtual size_t getNumberOfPreProcEquations() const = 0;
//    virtual const char* getPostProcName(size_t iPos) const = 0;
//    virtual TSolType getPostProcValue(size_t iPos) const = 0;
    
    virtual td::String getLastErrorStr() const = 0;
    virtual const td::UINT4 getNumberOfModels() const = 0;
    virtual const td::String& getModelName(td::UINT4 modelID) const = 0;
    
    virtual void setSignalHandler(sc::ISignal* pISignal) = 0;
    
    virtual void setDerivTestMode() = 0;
    
    virtual void setConsumer(sc::ISolutionBuffer* pConsumer) = 0;
    
    virtual void setReportResultsPerNRIteration(sc::ReportRate reportPerNRIteration) = 0;
    virtual sc::ReportRate getReportResultsPerNRIteration() = 0;
    
    virtual bool requiresConjugateVars() const = 0;
    
    virtual void setLogInfo(bool showDerivs, bool forCPP, bool showInitialValue) = 0;
    
    virtual ITime* getITime() = 0;
    
    virtual size_t getNoOfOutSymbols() const = 0;
    virtual bool getOutSymbols(cnt::SafeFullVector<td::INT4>&) const = 0;
    
    virtual const math::StatsManager& getStatsManager() const = 0;
    
    virtual cnt::SafeFullVector<td::String> getFunctionNames() const = 0;
    virtual cnt::SafeFullVector<td::String> getLogicOperators() const = 0;
    
    virtual const cnt::SafeFullVector<sc::SymbolInfo>& getSymbolInfo() const = 0;
    virtual const cnt::SafeFullVector<sc::SymbolInfo>* getSymbolInfoPtr() const = 0;
    
    virtual void addLog(const sc::ILog::Entry&, sc::ILog::Type type = sc::ILog::Type::Error) = 0;
    
    virtual SolverType getType() const = 0;
    virtual const SolverAttribs& getAttribs() const = 0;
    
    virtual void setMaxRepetitions(td::INT4 maxRepeats) = 0;
    //controlled through initMain=true/false
//    virtual void setSubModelType(SubModelType subModelType) = 0;
    
    virtual void setOutFilter(OutFilter outFilter) = 0;
    virtual void release() = 0;
};


inline const char* getSolutionStatusStr(sc::Solution sol)
{
    return SolutionNames[int(sol)];
}

using IDblSolver = ISolver<double>;
using ICmplxSolver = ISolver<td::cmplx>;

using DblResultInfo = ISolver<double>::ResultInfo;
using CmplxResultInfo = ISolver<td::cmplx>::ResultInfo;

} //namspace sc

