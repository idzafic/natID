// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IRow.h
    @brief Interface for reading and writing individual column values within a data row. */
#pragma once
#include <td/Variant.h>

namespace dp
{
/// @brief Abstract interface representing a single row in a result set with column-level access.
	class IRow
	{
	public:
		/// @brief Sets the value of the column at the specified position.
		/// @param colPosition Zero-based index of the column whose value is to be set.
		/// @param val The variant value to assign to the column.
		/// @return true if the value was set successfully, false otherwise.
		virtual bool setValue(size_t colPosition, const td::Variant& val) = 0;

		/// @brief Retrieves the value of the column at the specified position.
		/// @param colPosition Zero-based index of the column to read.
		/// @return Const reference to the variant value stored in the column.
		virtual const td::Variant& getValue(size_t colPosition) const = 0;
	};
}
