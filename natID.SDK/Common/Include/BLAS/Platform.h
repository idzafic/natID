// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Platform.h
    @brief Defines the Platform enumeration for supported CPU architectures used by BLAS. */
#pragma once
namespace BLAS
{
	/// @brief Enumeration of supported CPU platforms for BLAS optimizations.
	typedef enum {AMD_Athlon=0, ///< AMD Athlon processor
	              AMD_Barcelona=1, ///< AMD Barcelona processor
	              Intel_Pentium4=100, ///< Intel Pentium 4 processor
	              Intel_Core2=101, ///< Intel Core 2 processor
	              Intel_Penryn=102, ///< Intel Penryn processor
	              Intel_Nehalem=103 ///< Intel Nehalem processor
	             } Platform;
}
