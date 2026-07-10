// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IFilter.h
    @brief Interface for row-level filtering predicates used in data processing. */
#pragma once
#include <td/VariantBase.h>

namespace dp
{
/// @brief Abstract interface representing a predicate that tests whether a data row satisfies given criteria.
	class IFilter
	{
	public:
		//returns true when row content matches given criteria
		/// @brief Evaluates whether the given row satisfies the filter criteria.
		/// @param row Pointer to an array of variant values representing a single data row.
		/// @return true if the row matches the filter criteria, false otherwise.
		virtual bool filter(const td::VariantBase* row) const = 0;
	};
}
