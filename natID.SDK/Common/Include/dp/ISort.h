// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ISort.h
    @brief Interface for defining custom row comparison logic used in sorting operations. */
#pragma once
#include "td/VariantBase.h"

namespace dp
{
/// @brief Abstract interface for comparing two data rows, enabling custom sort order implementations.
	class ISort
	{
	public:
		//should return true if rowa <= rowb
		/// @brief Compares two rows and determines their relative order.
		/// @param rowa Pointer to the variant array representing the first row.
		/// @param rowb Pointer to the variant array representing the second row.
		/// @return true if rowa is less than or equal to rowb in the defined sort order.
		virtual bool compare(const td::VariantBase* rowa, const td::VariantBase* rowb) const = 0;
	};
}
