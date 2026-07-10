// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IParams.h
    @brief Interface for accessing and mutating positional query parameters. */
#pragma once
#include <td/Variant.h>

namespace dp
{
/// @brief Abstract interface for getting and setting positional parameters used in database statements.
	class IParams
	{
	public:
		/// @brief Sets the value of the parameter at the specified position.
		/// @param position Zero-based index of the parameter to set.
		/// @param val The variant value to assign to the parameter.
		/// @return true if the parameter was set successfully, false otherwise.
		virtual bool setParam(size_t position, const td::Variant& val) = 0;

		/// @brief Retrieves the value of the parameter at the specified position.
		/// @param position Zero-based index of the parameter to retrieve.
		/// @return Const reference to the variant value at the given position.
		virtual const td::Variant& getParam(size_t position) const = 0;
	};
}
