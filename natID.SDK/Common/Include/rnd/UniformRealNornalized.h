// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <iostream>
#include <random>
#include <chrono>

namespace rnd
{
	class UniformRealNornalized
	{
		std::uniform_real_distribution<double> _unifRealDistribution;
		std::mt19937_64 _rndGenerator;		
	public:
		UniformRealNornalized()
			: _unifRealDistribution(0, 1)
		{
			reset();
		}

		void reset()
		{
			// initialize the random number generator with time-dependent seed
			uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
			std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
			_rndGenerator.seed(ss);
		}

		double next()
		{
			double val = _unifRealDistribution(_rndGenerator);
			return val;
		}
	};
}
