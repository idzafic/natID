// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SCLib.h
 *  @brief Public library API for the symbolic computation (sc) module: solver factory functions and naming setup utilities.
 */
#pragma once
#include <compiler/Definitions.h>

#if defined(SYMBOLICCOMP_STATIC)

    // Static library build/use
    #define SYMBOLICOMP_API
#else
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
#endif


#include "ISolver.h"
#include "Token.h"
#include "ILog.h"

namespace sc
{

/// @brief Identifies a header-level model parameter by its enumeration position.
enum class HeaderParam : td::BYTE {MaxIter=0, StartTime, DeltaTime, EndTime, Report, MaxReps, CreateTxtOutput, TxtOutputFileName, CreateNewTabForEachFigure, DrawPlots, Last}; //see Types.h

/// @brief Distinguishes the file type of a model artefact.
enum class FileType : td::BYTE {DigitalModel=0, Visualizer};

/// @brief Controls which Unicode symbol is used in the solution-OK report string.
enum class SolutionOKReportStringType : td::BYTE {OK=0, ThumbsUp, OKHand, OKButton};

/// @brief Set the symbol style used when reporting a successful solution.
/// @param solutionOKReportStringType The desired report string style.
SYMBOLICOMP_API void setSolutionOKReportStringType(SolutionOKReportStringType solutionOKReportStringType);
/// @brief Return the currently active solution-OK report string type.
/// @param solutionOKReportStringType Currently active style (used for lookup).
/// @return The active SolutionOKReportStringType.
SYMBOLICOMP_API SolutionOKReportStringType getSolutionOKReportStringType(SolutionOKReportStringType solutionOKReportStringType);
/// @brief Return the string displayed when a solution is reported as OK.
/// @return Const reference to the solution-OK display string.
SYMBOLICOMP_API const td::String& getSolutionOKReportString();

/// @brief Pre-allocate storage for the given number of naming positions.
/// @param nNamingPositions Number of naming positions to reserve.
SYMBOLICOMP_API void reserveNamings(td::UINT2 nNamingPositions);
/// @brief Register a set of function names for a specific naming position using an initializer list.
/// @param fnSet Function set category.
/// @param namingPosition Target naming position index.
/// @param lblNames Initializer list of null-terminated function name strings.
SYMBOLICOMP_API void setCommonFunctions(sc::FunctionSet fnSet, td::UINT2 namingPosition, const std::initializer_list<const char*>& lblNames);
/// @brief Register a set of function names for a specific naming position using a plain array.
/// @param fnSet Function set category.
/// @param namingPosition Target naming position index.
/// @param lblNames Array of null-terminated function name strings.
/// @param nLabels Number of entries in lblNames.
SYMBOLICOMP_API void setCommonFunctions(sc::FunctionSet fnSet, td::UINT2 namingPosition, const char* const lblNames[], td::UINT4 nLabels);
/// @brief Register real function names together with optional alternative spellings.
/// @param namingPosition Target naming position index.
/// @param lblRealAndCmplxFnNames Combined real/complex function name array.
/// @param nRealFnLabels Number of real function entries.
/// @param alternativeNames Initializer list of alternative name descriptors.
SYMBOLICOMP_API void setCommonRealFunctions(td::UINT2 namingPosition, const char* const lblRealAndCmplxFnNames[], td::UINT4 nRealFnLabels, const std::initializer_list<AlternativeRealFnNames>& alternativeNames);
/// @brief Register both real and complex function names for a naming position.
/// @param namingPosition Target naming position index.
/// @param lblRealAndCmplxFnNames Combined real/complex function name array.
/// @param nRealFnLabels Number of real function entries.
/// @param lblCmplxFnNames Array of complex-only function names.
/// @param nCmplxFnLabels Number of complex function entries.
SYMBOLICOMP_API void setCommonComplexFunctions(td::UINT2 namingPosition, const char* const lblRealAndCmplxFnNames[], td::UINT4 nRealFnLabels,  const char* const lblCmplxFnNames[], td::UINT4 nCmplxFnLabels);

/// @brief Register a range of attribute ID names for the given naming position.
/// @param namingPosition Target naming position index.
/// @param fromAttIDInclusive First attribute ID to register (inclusive).
/// @param toAttIDNonInclusive One-past-last attribute ID to register.
SYMBOLICOMP_API void setCommonAttribNames(td::UINT2 namingPosition, sc::AttribID fromAttIDInclusive, sc::AttribID toAttIDNonInclusive);

/// @brief Register a range of reserved keyword names for the given naming position.
/// @param namingPosition Target naming position index.
/// @param fromResIDInclusive First reserved ID to register (inclusive).
/// @param toResIDNonInclusive One-past-last reserved ID to register.
SYMBOLICOMP_API void setCommonReservedNames(td::UINT2 namingPosition, sc::ReservedID fromResIDInclusive, sc::ReservedID toResIDNonInclusive);

/// @brief Register a range of group keyword names for the given naming position.
/// @param namingPosition Target naming position index.
/// @param fromGrouupInclusive First group to register (inclusive).
/// @param toGroupNonInclusive One-past-last group to register.
SYMBOLICOMP_API void setCommonGroupNames(td::UINT2 namingPosition, sc::Group fromGrouupInclusive, sc::Group toGroupNonInclusive);

/// @brief Register a selected set of group keyword names using an initializer list.
/// @param namingPosition Target naming position index.
/// @param groups List of specific group values to register.
SYMBOLICOMP_API void setCommonGroupNames(td::UINT2 namingPosition, const std::initializer_list<sc::Group>& groups);

/// @brief Return the canonical name string for a header parameter.
/// @param headerParam The header parameter whose name is requested.
/// @return Null-terminated parameter name.
SYMBOLICOMP_API const char* const getHeaderParamName(HeaderParam headerParam);

/// @brief Register real function names from a fixed-size array (template helper).
/// @tparam N Array length (deduced).
/// @param labelID Function set category.
/// @param namingPosition Target naming position index.
/// @param lblNames Fixed-size array of null-terminated function name strings.
template <size_t N>
inline void setCommonFunctions(sc::FunctionSet labelID, td::UINT2 namingPosition, const char* const (&lblNames)[N])
{
    setCommonFunctions(labelID, namingPosition, lblNames, static_cast<td::UINT4>(N));
}

/// @brief Register real function names from a fixed-size array with alternative names (template helper).
/// @tparam NREAL Array length (deduced).
/// @param namingPosition Target naming position index.
/// @param lblRealAndCmplxNames Fixed-size real/complex name array.
/// @param alternativeNames Initializer list of alternative name descriptors.
template <size_t NREAL>
inline void setCommonRealFunctions(td::UINT2 namingPosition, const char* const (&lblRealAndCmplxNames)[NREAL], const std::initializer_list<AlternativeRealFnNames>& alternativeNames)
{
    setCommonRealFunctions(namingPosition, lblRealAndCmplxNames, static_cast<td::UINT4>(NREAL), alternativeNames);
}

/// @brief Register real and complex function names from fixed-size arrays (template helper).
/// @tparam NREAL Length of real/complex name array (deduced).
/// @tparam NCMPLX Length of complex-only name array (deduced).
/// @param namingPosition Target naming position index.
/// @param lblRealAndCmplxNames Fixed-size real/complex name array.
/// @param lblCmplxNames Fixed-size complex-only name array.
template <size_t NREAL, size_t NCMPLX>
inline void setCommonComplexFunctions(td::UINT2 namingPosition, const char* const (&lblRealAndCmplxNames)[NREAL], const char* const (&lblCmplxNames)[NCMPLX])
{
    setCommonComplexFunctions(namingPosition, lblRealAndCmplxNames, static_cast<td::UINT4>(NREAL), lblCmplxNames, static_cast<td::UINT4>(NCMPLX));
}

/// @brief Populate all naming positions with their default function and keyword sets.
SYMBOLICOMP_API void setDefaultDataForAllNamings();
/// @brief Populate a single naming position with its default function and keyword sets.
/// @param namingPosition Index of the naming position to initialise.
SYMBOLICOMP_API void setDefaultDataForNaming(td::UINT2 namingPosition);

//SYMBOLICOMP_API void setModelLabelPos(td::UINT2 nNamingPosition, int lblPosModel, int lblPosSubModel);
//SYMBOLICOMP_API void setVarAndParamAndTFsLabelPos(td::UINT2 nNamingPosition, int lblPosVars, int lblPosParams, int lblPosDistribs, int lblPosStats, int lblTFsPos);

/// @brief Set the attribute enum position used to identify the complex-domain attribute.
/// @param nNamingPosition Target naming position index.
/// @param lblCmplxPos Position index of the complex-domain attribute enum.
SYMBOLICOMP_API void setComplexDomainAttribEnumPos(td::UINT2 nNamingPosition, int lblCmplxPos);

//for dbg purposes
/// @brief Return the display name of a function at a given position (debug helper).
/// @param lblType Function set category.
/// @param pos Zero-based position within the function set.
/// @param namingPosition Naming position index.
/// @return Function name string.
SYMBOLICOMP_API td::String getFunctionName(sc::FunctionSet lblType, td::UINT4 pos, td::UINT2 namingPosition);

/// @brief Return the string representation of an attribute enum value at the given position.
/// @param pos Zero-based attribute enum position.
/// @return Attribute enum value string.
SYMBOLICOMP_API td::String getAttribEnumValue(td::UINT4 pos);

//comon for all naming positions
/// @brief Register special mathematical constants (e.g. Pi, e) shared across all naming positions.
/// @param constants Initializer list of special constant descriptors.
SYMBOLICOMP_API void setSpecialConstants(const std::initializer_list<SpecialConstantInfo>& constants);
/// @brief Register logical keyword labels (true/false tokens) for all naming positions.
/// @param logicalLabels Initializer list of token label descriptors.
SYMBOLICOMP_API void setLogicalLabels(const std::initializer_list<TokenLabel>& logicalLabels);
/// @brief Register numeric type keyword labels for all naming positions.
/// @param numericTypeLabels Initializer list of numeric type label descriptors.
SYMBOLICOMP_API void setNumericTypeLabels(const std::initializer_list<NumericTypeLabel>& numericTypeLabels);

/// @brief Look up the FunctionSet category for a label string.
/// @param label The label string to search.
/// @return The matching FunctionSet value.
SYMBOLICOMP_API sc::FunctionSet getCommonLabel(const td::String& label);

/// @brief Create a real ODE solver with the given logger.
/// @param pLog Logger for diagnostic messages.
/// @return Pointer to a newly created IDblSolver for ODE problems.
SYMBOLICOMP_API IDblSolver* createDblODESolver(sc::ILog* pLog);
/// @brief Create a real DAE solver with the given logger and iteration limit.
/// @param pLog Logger for diagnostic messages.
/// @param maxIter Maximum number of iterations.
/// @return Pointer to a newly created IDblSolver for DAE problems.
SYMBOLICOMP_API IDblSolver* createDblDAESolver(sc::ILog* pLog, int maxIter);
/// @brief Create a real Newton-Raphson (NL) solver with the given logger and iteration limit.
/// @param pLog Logger for diagnostic messages.
/// @param maxIter Maximum number of iterations.
/// @return Pointer to a newly created IDblSolver for NL problems.
SYMBOLICOMP_API IDblSolver* createDblNRSolver(sc::ILog* pLog, int maxIter);
/// @brief Create a real Weighted Least-Squares (WLS) solver with the given logger and iteration limit.
/// @param pLog Logger for diagnostic messages.
/// @param maxIter Maximum number of iterations.
/// @return Pointer to a newly created IDblSolver for WLS problems.
SYMBOLICOMP_API IDblSolver* createDblWLSSolver(sc::ILog* pLog, int maxIter);

/// @brief Create a complex ODE solver with the given logger.
/// @param pLog Logger for diagnostic messages.
/// @return Pointer to a newly created ICmplxSolver for ODE problems.
SYMBOLICOMP_API ICmplxSolver* createCmplxODESolver(sc::ILog* pLog);
/// @brief Create a complex DAE solver with the given logger and iteration limit.
/// @param pLog Logger for diagnostic messages.
/// @param maxIter Maximum number of iterations.
/// @return Pointer to a newly created ICmplxSolver for DAE problems.
SYMBOLICOMP_API ICmplxSolver* createCmplxDAESolver(sc::ILog* pLog, int maxIter);
/// @brief Create a complex Newton-Raphson (NL) solver with the given logger and iteration limit.
/// @param pLog Logger for diagnostic messages.
/// @param maxIter Maximum number of iterations.
/// @return Pointer to a newly created ICmplxSolver for NL problems.
SYMBOLICOMP_API ICmplxSolver* createCmplxNRSolver(sc::ILog* pLog, int maxIter);
/// @brief Create a complex Weighted Least-Squares (WLS) solver with the given logger and iteration limit.
/// @param pLog Logger for diagnostic messages.
/// @param maxIter Maximum number of iterations.
/// @return Pointer to a newly created ICmplxSolver for WLS problems.
SYMBOLICOMP_API ICmplxSolver* createCmplxWLSSolver(sc::ILog* pLog, int maxIter);

}

