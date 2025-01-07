// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/Variant.h>

namespace dp
{
	class IRow
	{
	public:		
		virtual bool setValue(size_t colPosition, const td::Variant& val) = 0;
		virtual const td::Variant& getValue(size_t colPosition) const = 0;
	};
}
