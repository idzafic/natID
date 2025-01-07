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
//#include <limits>
#include <random>

namespace rnd
{
	template <typename T>
	class Int
	{
	private:
		T _minVal;
		T _mask;
		int _shift;
		std::random_device _rd;
		std::mt19937_64 _gen;
		std::uniform_int_distribution<T> _dis;

		//boost::minstd_rand _random;
		inline T _random()
		{
			return _dis(_gen);
		}
	public:
		Int(T minValue, T mask, int shift/*, int rndSeed = 0*/)	
			: _minVal(minValue)
			, _mask(mask)
			, _shift(shift)
			, _gen(_rd())
			, _dis(minValue)
		{
			//if (rndSeed == 0)
			//	rndSeed = (int)time(NULL);

			//_random.seed(rndSeed);
		}

		Int(T minValue = 0)
			: _minVal(minValue)
			, _mask(0)
			, _shift(0)
			, _gen(_rd())
			, _dis(minValue)
		{
			//int rndSeed = (int) time(NULL);
			//_random.seed(rndSeed);
		}

		T nextWithoutMask()
		{
			T val;
			do
			{
				val = _random();
			}
			while (val < _minVal);			
			return val;
		}

		T next()
		{
			T val;
			do
			{
				val = _random();
				val &= _mask;
			}
			while (val < _minVal);
			return val;
		}

		T nextShift( )
		{
			assert(_shift > 0);
			T val;
			do
			{
				val = _random();
				val = val << _shift;
			}
			while (val < _minVal);

			return val;
		}
	};
}
