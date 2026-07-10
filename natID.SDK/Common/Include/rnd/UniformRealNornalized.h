// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file UniformRealNornalized.h
 *  @brief Provides a uniform real-valued random number generator normalised to the [0, 1] interval.
 */
#pragma once
#include <iostream>
#include <random>
#include <chrono>

namespace rnd
{
/// @brief Generates uniformly distributed double values in [0, 1] using a time-seeded Mersenne Twister engine.
class UniformRealNornalized
{
    std::uniform_real_distribution<double> _unifRealDistribution; ///< Uniform real distribution over [0, 1].
    std::mt19937_64 _rndGenerator;                                ///< 64-bit Mersenne Twister pseudo-random engine.
public:
    /// @brief Constructs the generator and seeds it from the current high-resolution clock time.
    UniformRealNornalized()
        : _unifRealDistribution(0, 1)
    {
        reset();
    }

    /// @brief Re-seeds the internal engine with a new time-dependent seed.
    void reset()
    {
        // initialize the random number generator with time-dependent seed
        uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
        _rndGenerator.seed(ss);
    }

    /// @brief Generates the next random double uniformly distributed in [0, 1].
    /// @return A random double value in the range [0, 1].
    double next()
    {
        double val = _unifRealDistribution(_rndGenerator);
        return val;
    }
};
}
