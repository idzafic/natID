// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once

namespace cnt
{
	template<class TITER, class TFN>
	inline void forEach(TITER& first, const TITER& last, TFN& f) 
	{
		while (first != last)
		{
			f(*first);
			++first;
		}
			
	}
	
	template<class TCNT, class TFN>
	inline void forEach(TCNT& container, TFN& f) 
	{
		auto it = container.begin();
		auto itEnd = container.cend();
		
		while (it != itEnd)
		{
			f(*it);
			++it;
		}			
	}
}