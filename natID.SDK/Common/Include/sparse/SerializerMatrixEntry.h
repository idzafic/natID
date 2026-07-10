// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SerializerMatrixEntry.h
    @brief Defines a template struct for sparse matrix entries used during serialization. */
#pragma once
#include <td/Types.h>

namespace sparse
{
	/// @brief Represents a single non-zero entry in a sparse matrix, using 1-based row/column indices.
	/// @tparam T Numeric type of the matrix element value (e.g. double, td::cmplx).
	template <typename T>
	struct SMatEntry
	{
		int I; ///< 1-based row index of the matrix entry.
		int J; ///< 1-based column index of the matrix entry.
		T value; ///< Numeric value stored at position (I, J).
	};

	typedef struct SMatEntry<double> SerializerDblMatrixEntry; ///< Sparse matrix entry with a double-precision value.
	typedef struct SMatEntry<td::cmplx> SerializerCmplxMatrixEntry; ///< Sparse matrix entry with a complex value.
}
