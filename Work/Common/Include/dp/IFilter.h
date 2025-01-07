// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/VariantBase.h>

namespace dp
{
	class IFilter
	{
	public:	
		//returns true when row content matches given criteria
		virtual bool filter(const td::VariantBase* row) const = 0;
	};
}
