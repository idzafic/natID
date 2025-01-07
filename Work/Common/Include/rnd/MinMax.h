// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
//#include <iostream>
#include <random>

namespace rnd
{
	template <typename T>
	class MinMax
	{
	private:
		//T _minVal;
		//T _maxVal;		
		std::random_device _rd;
		std::mt19937_64 _gen;
		std::uniform_int_distribution<T> _dis;
	public:
		MinMax(T minValue, T maxValue/*, int rndSeed = 0*/)
			//: _minVal(minValue)
			//, _maxVal(maxValue)					
			: _gen(_rd())
			, _dis(minValue, maxValue)
		{	
			//rndSeed;
		}

		T next()
		{
			T val = _dis(_gen);
			return val;
		}
	};
}
