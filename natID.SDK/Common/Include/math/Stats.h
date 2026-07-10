// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Stats.h
 * @brief Online statistics accumulator tracking min, max, mean, and variance.
 */
#pragma once
#include <td/String.h>
#include "Limits.h"

namespace math
{

/// @brief Online single-pass statistics collector for a named series of values.
template <typename T>
class Stats
{
protected:
    td::String _name; ///< Optional name identifying this statistics instance.
    T _m2 = 0;        ///< Accumulated sum of squared deviations from the running mean.
    T _avg = 0;       ///< Running mean of inserted values.
    T _min = math::maxVal<T>(); ///< Minimum value seen so far.
    T _max = math::minVal<T>(); ///< Maximum value seen so far.
    size_t _n = 0;    ///< Number of values inserted.

public:
    /// @brief Default constructor; initialises an empty statistics object.
    Stats()
    {}

    /// @brief Sets the name of this statistics instance.
    /// @param str Name to assign (any string-compatible type).
    template <typename TNAME>
    void setName(const TNAME& str)
    {
        _name = str;
    }

    /// @brief Returns the name of this statistics instance.
    /// @return Const reference to the name string.
    const td::String& getName() const
    {
        return _name;
    }

    /// @brief Inserts a new value and updates all running statistics.
    /// @param x Value to insert.
    void insert(T x)
    {
        if (_min > x)
            _min = x;
        if (_max < x)
            _max = x;
        _n += 1;
        T newAvg = _avg + (x-_avg)/_n;
        _m2 += (x - _avg) * (x - newAvg);
        _avg = newAvg;
    }

    /// @brief Returns the population variance of all inserted values.
    /// @return Variance, or 0 if no values have been inserted.
    T var() const
    {
        if (_n == 0)
            return 0;
        return _m2 / _n;
    }

    /// @brief Returns the sample standard deviation of all inserted values.
    /// @return Standard deviation, or 0 if no values have been inserted.
    T dev() const
    {
        if (_n == 0)
            return 0;
        if (_n == 1)
            return std::sqrt(_m2);

        return std::sqrt(_m2 / (_n-1));
    }

    /// @brief Returns the running mean of all inserted values.
    /// @return Current mean value.
    T avg() const
    {
        return _avg;
    }

    /// @brief Returns the minimum value inserted so far.
    /// @return Minimum value.
    T min() const
    {
        return _min;
    }

    /// @brief Returns the maximum value inserted so far.
    /// @return Maximum value.
    T max() const
    {
        return _max;
    }

    /// @brief Returns the total number of values that have been inserted.
    /// @return Count of inserted entries.
    size_t getNoOfEntries() const
    {
        return _n;
    }

    /// @brief Resets all accumulated statistics to their initial state.
    void reset()
    {
        _m2 = 0;
        _avg = 0;
        _min = math::maxVal<T>();
        _max = math::minVal<T>();
        _n = 0;
    }

    /// @brief Writes a formatted summary of the statistics to an output stream.
    /// @param oStr Output stream to write to.
    template <class TSTREAM>
    void show(TSTREAM& oStr) const
    {
        oStr << "Stats";
        if (_name.length() > 0)
            oStr << " for: " << _name;
        oStr << td::endl;
        oStr << "Avg: " << _avg << td::endl;
        oStr << "Min: " << _min << td::endl;
        oStr << "Max: " << _max << td::endl;
        oStr << "Var: " << var() << td::endl;
        oStr << "Dev: " << dev() << td::endl;
        oStr << "Smp: " << _n << td::endl;
    }
};

} //namespace math
