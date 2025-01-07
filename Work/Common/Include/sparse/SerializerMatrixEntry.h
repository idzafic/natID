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

namespace sparse
{
	template <typename T>
	struct SMatEntry
	{
		int I; //1-based
		int J; //1-based
		T value;
	};

	typedef struct SMatEntry<double> SerializerDblMatrixEntry;
	typedef struct SMatEntry<td::cmplx> SerializerCmplxMatrixEntry;
}
