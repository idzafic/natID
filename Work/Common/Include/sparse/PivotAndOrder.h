// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

#include <sparse/Format.h>

namespace sparse
{
    enum class Pivoting : unsigned char {No=0, DiagonalSinglePass, DiagonalMultiPass, MarkowitzSinglePass, MarkowitzMultiPass, AlterMatrixIfIndefinite} ;

    enum class Ordering : unsigned char {Own=0, OwnRadial, ExternSym, ExternAtPlusA, ExternAtMulA, ExternAMulAt} ;
	

    enum class Symmetry : unsigned char {NonSymmetric=0, SymmetricPosDef, SymmetricGeneral, SymmetricIndef, HermitianPosDef, HermitianIndef};
}
