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
	class IParams
	{
	public:		
		virtual bool setParam(size_t position, const td::Variant& val) = 0;
		virtual const td::Variant& getParam(size_t position) const = 0;
	};
}
