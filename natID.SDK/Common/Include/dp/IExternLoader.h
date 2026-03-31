// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

namespace dp
{
	class IDataSet;

	class IExternLoader
	{
	public:
		//should return true if rowa <= rowb
		virtual bool populateDP(dp::IDataSet* pRS) const = 0;
	};
}
