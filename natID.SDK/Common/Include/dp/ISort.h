// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include "td/VariantBase.h"

namespace dp
{
	class ISort
	{
	public:		
		//should return true if rowa <= rowb
		virtual bool compare(const td::VariantBase* rowa, const td::VariantBase* rowb) const = 0;
	};
}
