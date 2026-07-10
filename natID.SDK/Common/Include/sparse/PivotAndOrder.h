// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PivotAndOrder.h
    @brief Defines enumerations for sparse matrix pivoting strategies, reordering algorithms, and symmetry types. */
#pragma once

#include <sparse/Format.h>

namespace sparse
{
    /// @brief Enumeration of pivoting strategies used during sparse matrix factorization.
    enum class Pivoting : unsigned char
    {
        No=0, ///< No pivoting applied
        DiagonalSinglePass, ///< Single-pass diagonal pivoting
        DiagonalMultiPass, ///< Multi-pass diagonal pivoting
        MarkowitzSinglePass, ///< Single-pass Markowitz fill-reducing pivoting
        MarkowitzMultiPass, ///< Multi-pass Markowitz fill-reducing pivoting
        AlterMatrixIfIndefinite ///< Modify the matrix to handle indefinite systems
    };

    /// @brief Enumeration of matrix reordering algorithms to reduce fill-in during factorization.
    enum class Ordering : unsigned char
    {
        Own=0, ///< Internal default ordering
        OwnRadial, ///< Internal radial ordering heuristic
        ExternSym, ///< External symmetric reordering
        ExternAtPlusA, ///< External reordering based on A^T + A
        ExternAtMulA, ///< External reordering based on A^T * A
        ExternAMulAt ///< External reordering based on A * A^T
    };


    /// @brief Enumeration of matrix symmetry types used to select specialized factorization paths.
    enum class Symmetry : unsigned char
    {
        NonSymmetric=0, ///< General non-symmetric matrix
        SymmetricPosDef, ///< Symmetric positive-definite matrix
        SymmetricGeneral, ///< Symmetric matrix (general, possibly indefinite)
        SymmetricIndef, ///< Symmetric indefinite matrix
        HermitianPosDef, ///< Hermitian positive-definite matrix (complex)
        HermitianIndef ///< Hermitian indefinite matrix (complex)
    };
}
