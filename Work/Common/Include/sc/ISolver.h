// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

#include <td/Types.h>
#include <td/String.h>
#include "ISolutionBuffer.h"
#include "ISignal.h"
#include <complex>
#include "ITime.h"
#include <cnt/SafeFullVector.h>
#include <math/StatsManager.h>

namespace sc
{

enum class Solution : unsigned char {OK=0, FactorizationError, SolutionError, Oscillation, MaximumIteration, SubmodelFailed};
enum class OutFilter : unsigned char {Minimum=0, InitialValues = 1, ValuesForIteration=2, InitialAndIterationValues=3};
enum class ReportRate : unsigned char {None = 0, InitialAndFinal, InitialAndIterationsAndFinal};

template <class TSolType>
class ISolver
{
public:
    enum class SourceType : unsigned char {File = 0, Memory};

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
    
    virtual int init(const td::String& fileNameOrXmlContent, SourceType source = SourceType::File) =0;
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
    
    virtual void setLogInfo(std::ostream* pStream, bool showDerivs, bool forCPP, bool showInitialValue) = 0;
    
    virtual ITime* getITime() = 0;
    
    virtual size_t getNoOfOutSymbols() const = 0;
    virtual bool getOutSymbols(cnt::SafeFullVector<td::INT4>&) const = 0;
    
    virtual const math::StatsManager& getStatsManager() const = 0;
    
    virtual cnt::SafeFullVector<td::String> getFunctionNames() const = 0;
    virtual cnt::SafeFullVector<td::String> getLogicOperators() const = 0;
    
    virtual void setOutFilter(OutFilter outFilter) = 0;
    virtual void release() = 0;
};

typedef ISolver<double> IDblSolver;
typedef ISolver<std::complex<double>> ICmplxSolver;

} //namspace sc

