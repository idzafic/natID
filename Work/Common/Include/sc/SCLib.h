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
namespace sc
{
SYMBOLICOMP_API IDblSolver* createDblODESolver();
SYMBOLICOMP_API IDblSolver* createDblDAESolver(int maxIter);
SYMBOLICOMP_API IDblSolver* createDblNRSolver(int maxIter);
SYMBOLICOMP_API IDblSolver* createDblWLSSolver(int maxIter);

SYMBOLICOMP_API ICmplxSolver* createCmplxODESolver();
SYMBOLICOMP_API ICmplxSolver* createCmplxDAESolver(int maxIter);
SYMBOLICOMP_API ICmplxSolver* createCmplxNRSolver(int maxIter);
SYMBOLICOMP_API ICmplxSolver* createCmplxWLSSolver(int maxIter);

}

