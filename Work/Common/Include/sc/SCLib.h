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
    #ifdef SYMBOLICCOMP_EXPORTS
        #define SYMBOLICOMP_API __declspec(dllexport)
    #else
        #define SYMBOLICOMP_API __declspec(dllimport)
    #endif
#else
    #ifdef SYMBOLICCOMP_EXPORTS
        #define SYMBOLICOMP_API __attribute__((visibility("default")))
    #else
        #define SYMBOLICOMP_API
    #endif
#endif


#include "ISolver.h"
#include "Token.h"
#include "ILog.h"

namespace sc
{

enum class HeaderParam : td::BYTE {MaxIter=0, StartTime, DeltaTime, EndTime, Report, MaxReps, CreateTxtOutput, TxtOutputFileName, CreateNewTabForEachFigure, Last}; //see Types.h

enum class FileType : td::BYTE {DigitalModel=0, Visualizer};

enum class SolutionOKReportStringType : td::BYTE {OK=0, ThumbsUp, OKHand, OKButton};

SYMBOLICOMP_API void setSolutionOKReportStringType(SolutionOKReportStringType solutionOKReportStringType);
SYMBOLICOMP_API SolutionOKReportStringType getSolutionOKReportStringType(SolutionOKReportStringType solutionOKReportStringType);
SYMBOLICOMP_API const td::String& getSolutionOKReportString();

SYMBOLICOMP_API void reserveNamings(td::UINT2 nNamingPositions);
SYMBOLICOMP_API void setCommonFunctions(sc::FunctionSet fnSet, td::UINT2 namingPosition, const std::initializer_list<const char*>& lblNames);
SYMBOLICOMP_API void setCommonFunctions(sc::FunctionSet fnSet, td::UINT2 namingPosition, const char* const lblNames[], td::UINT4 nLabels);
SYMBOLICOMP_API void setCommonRealFunctions(td::UINT2 namingPosition, const char* const lblRealAndCmplxFnNames[], td::UINT4 nRealFnLabels, const std::initializer_list<AlternativeRealFnNames>& alternativeNames);
SYMBOLICOMP_API void setCommonComplexFunctions(td::UINT2 namingPosition, const char* const lblRealAndCmplxFnNames[], td::UINT4 nRealFnLabels,  const char* const lblCmplxFnNames[], td::UINT4 nCmplxFnLabels);

SYMBOLICOMP_API void setCommonAttribNames(td::UINT2 namingPosition, sc::AttribID fromAttIDInclusive, sc::AttribID toAttIDNonInclusive);

SYMBOLICOMP_API void setCommonReservedNames(td::UINT2 namingPosition, sc::ReservedID fromResIDInclusive, sc::ReservedID toResIDNonInclusive);

SYMBOLICOMP_API void setCommonGroupNames(td::UINT2 namingPosition, sc::Group fromGrouupInclusive, sc::Group toGroupNonInclusive);

SYMBOLICOMP_API void setCommonGroupNames(td::UINT2 namingPosition, const std::initializer_list<sc::Group>& groups);

SYMBOLICOMP_API const char* const getHeaderParamName(HeaderParam headerParam);

template <size_t N>
inline void setCommonFunctions(sc::FunctionSet labelID, td::UINT2 namingPosition, const char* const (&lblNames)[N])
{
    setCommonFunctions(labelID, namingPosition, lblNames, static_cast<td::UINT4>(N));
}

template <size_t NREAL>
inline void setCommonRealFunctions(td::UINT2 namingPosition, const char* const (&lblRealAndCmplxNames)[NREAL], const std::initializer_list<AlternativeRealFnNames>& alternativeNames)
{
    setCommonRealFunctions(namingPosition, lblRealAndCmplxNames, static_cast<td::UINT4>(NREAL), alternativeNames);
}

template <size_t NREAL, size_t NCMPLX>
inline void setCommonComplexFunctions(td::UINT2 namingPosition, const char* const (&lblRealAndCmplxNames)[NREAL], const char* const (&lblCmplxNames)[NCMPLX])
{
    setCommonComplexFunctions(namingPosition, lblRealAndCmplxNames, static_cast<td::UINT4>(NREAL), lblCmplxNames, static_cast<td::UINT4>(NCMPLX));
}

SYMBOLICOMP_API void setDefaultDataForAllNamings();
SYMBOLICOMP_API void setDefaultDataForNaming(td::UINT2 namingPosition);

//SYMBOLICOMP_API void setModelLabelPos(td::UINT2 nNamingPosition, int lblPosModel, int lblPosSubModel);
//SYMBOLICOMP_API void setVarAndParamAndTFsLabelPos(td::UINT2 nNamingPosition, int lblPosVars, int lblPosParams, int lblPosDistribs, int lblPosStats, int lblTFsPos);

SYMBOLICOMP_API void setComplexDomainAttribEnumPos(td::UINT2 nNamingPosition, int lblCmplxPos);

//for dbg purposes
SYMBOLICOMP_API td::String getFunctionName(sc::FunctionSet lblType, td::UINT4 pos, td::UINT2 namingPosition);

SYMBOLICOMP_API td::String getAttribEnumValue(td::UINT4 pos);

//comon for all naming positions
SYMBOLICOMP_API void setSpecialConstants(const std::initializer_list<SpecialConstantInfo>& constants);
SYMBOLICOMP_API void setLogicalLabels(const std::initializer_list<TokenLabel>& logicalLabels);
SYMBOLICOMP_API void setNumericTypeLabels(const std::initializer_list<NumericTypeLabel>& numericTypeLabels);

SYMBOLICOMP_API sc::FunctionSet getCommonLabel(const td::String& label);

SYMBOLICOMP_API IDblSolver* createDblODESolver(sc::ILog* pLog);
SYMBOLICOMP_API IDblSolver* createDblDAESolver(sc::ILog* pLog, int maxIter);
SYMBOLICOMP_API IDblSolver* createDblNRSolver(sc::ILog* pLog, int maxIter);
SYMBOLICOMP_API IDblSolver* createDblWLSSolver(sc::ILog* pLog, int maxIter);

SYMBOLICOMP_API ICmplxSolver* createCmplxODESolver(sc::ILog* pLog);
SYMBOLICOMP_API ICmplxSolver* createCmplxDAESolver(sc::ILog* pLog, int maxIter);
SYMBOLICOMP_API ICmplxSolver* createCmplxNRSolver(sc::ILog* pLog, int maxIter);
SYMBOLICOMP_API ICmplxSolver* createCmplxWLSSolver(sc::ILog* pLog, int maxIter);

}

