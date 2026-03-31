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

namespace sc
{

using SolutionOptions = struct so
{
    double eps = 1e-6;
    double maxIter = 1000;
    double t0 = 0;
    double dT = 1e-3;
};

enum class SourceType : unsigned char {XMLFile = 0, XMLMemory, ModlFile};

enum class Solution : unsigned char {OK=0, BaseOK, Iterating, LimitFix, NewPass, EmptyMatrix, FactorizationError, SolutionError, Oscillation, NoImprovement, MaximumIteration, SubmodelFailed};
constexpr const char* SolutionNames[] = {"OK", "BaseOK", "IterBase", "LimitFix", "NewPass", "EmptyMatrix", "FactorizationError", "SolutionError", "Oscillation", "NoImprovement", "MaxIters",  "SubmodelFailed"};

enum class OutFilter : unsigned char {Minimum=0, InitialValues = 1, ValuesForIteration=2, InitialAndIterationValues=3};
enum class ReportRate : unsigned char {None = 0, InitialAndFinal, InitialAndIterationsAndFinal};

//enum class SolverType : unsigned char {NL=0, WLS, DAE};
enum class SolverType : td::BYTE {Deriv = 0, NL, WLS, ODE, DAE, Unknown};
constexpr const char* SolverTypeNames[] = {"Deriv", "NL", "WLS", "ODE", "DAE"};

enum class Report : td::BYTE {Solved=0, All, AllDetails};

} //namspace sc

