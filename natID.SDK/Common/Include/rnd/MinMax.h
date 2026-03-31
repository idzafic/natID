// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <random>
#include <chrono>

namespace rnd
{
	template <typename T>
	class MinMax
	{
	private:		
		std::random_device _rd;
		std::mt19937_64 _gen;
		std::uniform_int_distribution<T> _dis;
	public:
		MinMax(T minValue, T maxValue)				
			: _gen(_rd())
			, _dis(minValue, maxValue)
		{	
		}

		void reSeed()
		{
             std::mt19937_64 generator((unsigned int) std::chrono::high_resolution_clock::now().time_since_epoch().count());
            _gen = generator;
		}

		T next()
		{
			T val = _dis(_gen);
			return val;
		}
	};
}
