// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Int.h
 *  @brief Template class for generating random integers with optional masking and bit-shift strategies.
 */
#pragma once
//#include <iostream>
//#include <limits>
#include <random>

namespace rnd
{
/// @brief Generates random integers of type T using a Mersenne Twister engine, with support for bit-mask and bit-shift filtering.
template <typename T>
class Int
{
private:
    T _minVal;                             ///< Minimum acceptable generated value.
    T _mask;                               ///< Bitmask applied to each candidate value (0 means no mask).
    int _shift;                            ///< Left-shift count applied to each candidate value (0 means no shift).
    std::random_device _rd;               ///< Non-deterministic entropy source used to seed the engine.
    std::mt19937_64 _gen;                 ///< 64-bit Mersenne Twister pseudo-random engine.
    std::uniform_int_distribution<T> _dis; ///< Uniform integer distribution over type T.

    //boost::minstd_rand _random;
    /// @brief Draws the next raw random value from the distribution.
    /// @return A raw random value of type T from the uniform distribution.
    inline T _random()
    {
        return _dis(_gen);
    }
public:
    /// @brief Constructs an Int generator with an explicit minimum value, bitmask, and shift count.
    /// @param minValue Minimum acceptable generated value.
    /// @param mask     Bitmask applied to each candidate (use 0 to disable masking).
    /// @param shift    Left-shift count applied to each candidate (use 0 to disable shifting).
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

    /// @brief Constructs an Int generator with a minimum value and no masking or shifting.
    /// @param minValue Minimum acceptable generated value (defaults to 0).
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

    /// @brief Generates the next random value without applying any bitmask.
    /// @return A random value >= _minVal drawn directly from the distribution.
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

    /// @brief Generates the next random value after applying the configured bitmask.
    /// @return A random value >= _minVal that also satisfies the bitmask constraint.
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

    /// @brief Generates the next random value using a left bit-shift instead of a bitmask.
    /// @return A left-shifted random value >= _minVal.
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
