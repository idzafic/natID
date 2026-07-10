// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SolutionOptions.h
 *  @brief Enumerations and option structures describing solver configuration and solution status.
 */
#pragma once
#include <td/Types.h>

namespace sc
{

/// @brief Solver configuration options controlling convergence and time-stepping.
using SolutionOptions = struct so
{
    double eps = 1e-6;   ///< Convergence tolerance (epsilon) for iterative solvers.
    double maxIter = 1000; ///< Maximum number of solver iterations.
    double t0 = 0;       ///< Simulation start time.
    double dT = 1e-3;    ///< Integration step size.
};

//enum class SourceType : unsigned char {XMLFile = 0, XMLMemory, ModlFile};

/// @brief Outcome of a single solver pass.
enum class Solution : unsigned char {OK=0, BaseOK, Iterating, LimitFix, NewPass, EmptyMatrix, FactorizationError, SolutionError, Oscillation, NoImprovement, MaximumIteration, SubmodelFailed};
/// @brief Human-readable names corresponding to each Solution enum value.
constexpr const char* SolutionNames[] = {"OK", "BaseOK", "IterBase", "LimitFix", "NewPass", "EmptyMatrix", "FactorizationError", "SolutionError", "Oscillation", "NoImprovement", "MaxIters",  "SubmodelFailed"};

/// @brief Controls which intermediate variable values are included in the solver output.
enum class OutFilter : unsigned char {Minimum=0, InitialValues = 1, ValuesForIteration=2, InitialAndIterationValues=3};
/// @brief Frequency at which intermediate iteration results are reported.
enum class ReportRate : unsigned char {None = 0, InitialAndFinal, InitialAndIterationsAndFinal};

//enum class SolverType : unsigned char {NL=0, WLS, DAE};
/// @brief Numerical method implemented by a solver instance.
enum class SolverType : td::BYTE {Deriv = 0, NL, WLS, ODE, DAE, Unknown};
/// @brief Human-readable names corresponding to each SolverType enum value.
constexpr const char* SolverTypeNames[] = {"Deriv", "NL", "WLS", "ODE", "DAE"};

/// @brief Level of detail included in a solution report.
enum class Report : td::BYTE {Solved=0, All, AllDetails};

} //namspace sc

