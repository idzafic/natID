// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Format.h
    @brief Defines the Format enumeration for sparse matrix serialization output formats. */
#pragma once
namespace sparse
{
    /// @brief Bit-flag enumeration of supported sparse matrix serialization output formats.
    enum class Format : unsigned int
    {
        Cpp = 1, ///< C++ source code format
        SciLab = 2, ///< Scilab matrix format
        Matlab = 4, ///< MATLAB/Octave matrix format
        LaTeX = 8, ///< LaTeX bmatrix format
        Mtx = 16, ///< Matrix Market exchange format (.mtx)
        Fortran = 32 ///< Fortran array format
    };

}
