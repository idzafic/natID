// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file MinMax.h
 *  @brief Template class for generating uniformly distributed random integers within a [min, max] range.
 */
#pragma once
#include <random>
#include <chrono>

namespace rnd
{
/// @brief Generates uniformly distributed random values of type T within a caller-specified [min, max] range.
template <typename T>
class MinMax
{
private:
    std::random_device _rd;                ///< Non-deterministic entropy source used for initial seeding.
    std::mt19937_64 _gen;                  ///< 64-bit Mersenne Twister pseudo-random engine.
    std::uniform_int_distribution<T> _dis; ///< Uniform integer distribution bounded to [minValue, maxValue].
public:
    /// @brief Constructs a MinMax generator for values in the closed interval [minValue, maxValue].
    /// @param minValue Lower bound of the generation range (inclusive).
    /// @param maxValue Upper bound of the generation range (inclusive).
    MinMax(T minValue, T maxValue)
        : _gen(_rd())
        , _dis(minValue, maxValue)
    {
    }

    /// @brief Re-seeds the internal engine using the current high-resolution clock time.
    void reSeed()
    {
         std::mt19937_64 generator((unsigned int) std::chrono::high_resolution_clock::now().time_since_epoch().count());
        _gen = generator;
    }

    /// @brief Generates the next random value within the configured [min, max] range.
    /// @return A uniformly distributed random value of type T.
    T next()
    {
        T val = _dis(_gen);
        return val;
    }
};
}
