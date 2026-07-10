// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file StatsManager.h
    @brief Template manager that owns a collection of named Stats objects and exposes aggregate statistical operations. */
#pragma once
#include <math/Stats.h>
#include <cnt/SafeFullVector.h>

namespace math
{

/// @brief Manages a collection of named statistical accumulators of type Stats<T>.
/// @tparam T The numeric type used for statistical computations (e.g. float, double).
template <typename T>
class StatsManager
{

protected:
    cnt::SafeFullVector<math::Stats<T>> _stats; ///< Underlying vector of Stats objects.

public:
    /// @brief Default constructor; creates an empty StatsManager.
    StatsManager()
    {}

    /// @brief Reserves storage for a given number of Stats objects.
    /// @param nStats Number of stats slots to reserve.
    void reserve(size_t nStats)
    {
        _stats.reserve(nStats);
    }

    /// @brief Finds the index of the Stats object with the given name.
    /// @param name The name to search for.
    /// @return Zero-based index of the matching Stats, or -1 if not found.
    td::INT4 find(const td::StringExt& name) const
    {
        td::INT4 iPos = 0;
        for (const auto& s : _stats)
        {
            if (s.getName() == name)
                return iPos;
            ++iPos;
        }
        return -1;
    }

    /// @brief Finds the index of the Stats object with the given C-string name.
    /// @param pName Null-terminated C-string name to search for.
    /// @return Zero-based index of the matching Stats, or -1 if not found or input is null/empty.
    td::INT4 find(const char* pName) const
    {
        if (!pName)
            return -1;
        auto len = strlen(pName);
        if (len == 0)
            return -1;

        td::INT4 iPos = 0;

        for (const auto& s : _stats)
        {
            const auto& statName = s.getName();
            if (statName.compare(pName, (int)len))
                return iPos;
            ++iPos;
        }
        return -1;
    }

    /// @brief Assigns a name to the Stats at position iPos, provided the name is not already used by an earlier entry.
    /// @tparam TSTR String type compatible with Stats::setName.
    /// @param iPos Zero-based index of the Stats to rename.
    /// @param name New name to assign.
    /// @return true if the name was set successfully, false if a duplicate was found in earlier entries.
    template <class TSTR>
    bool setName(size_t iPos, const TSTR& name)
    {
        for (size_t i=0; i<iPos; ++i)
        {
            if (_stats[i].getName() == name)
                return false;
        }
        _stats[iPos].setName(name);
        return true;
    }

    /// @brief Returns the name of the Stats object at the given position.
    /// @param iPos Zero-based index.
    /// @return Const reference to the name string.
    const td::String& getName(size_t iPos) const
    {
        return _stats[iPos].getName();
    }

    /// @brief Returns a mutable reference to the Stats object at index i.
    /// @tparam TINT Integer index type.
    /// @param i Zero-based index.
    /// @return Reference to the Stats<T> object.
    template <typename TINT>
    Stats<T>& getStats(TINT i)
    {
        return _stats[i];
    }

    /// @brief Returns a const reference to the Stats object at index i.
    /// @tparam TINT Integer index type.
    /// @param i Zero-based index.
    /// @return Const reference to the Stats<T> object.
    template <typename TINT>
    const Stats<T>& getStats(TINT i) const
    {
        return _stats[i];
    }

    /// @brief Inserts a new sample value into the Stats object at index i.
    /// @tparam TINT Integer index type.
    /// @param i Zero-based index of the target Stats object.
    /// @param x Sample value to insert.
    template <typename TINT>
    void insert(TINT i, double x)
    {
        _stats[i].insert(x);
    }

    /// @brief Returns the variance of the Stats object at index i.
    /// @tparam TINT Integer index type.
    /// @param i Zero-based index.
    /// @return Variance as a double.
    template <typename TINT>
    double var(TINT i) const
    {
        return _stats[i].var();
    }

    /// @brief Returns the standard deviation of the Stats object at index i.
    /// @tparam TINT Integer index type.
    /// @param i Zero-based index.
    /// @return Standard deviation as a double.
    template <typename TINT>
    double dev(TINT i) const
    {
        return _stats[i].dev();
    }

    /// @brief Returns the arithmetic mean of the Stats object at index i.
    /// @tparam TINT Integer index type.
    /// @param i Zero-based index.
    /// @return Average as a double.
    template <typename TINT>
    double avg(TINT i) const
    {
        return _stats[i].avg();
    }

    /// @brief Returns the minimum observed value of the Stats object at index i.
    /// @tparam TINT Integer index type.
    /// @param i Zero-based index.
    /// @return Minimum value as a double.
    template <typename TINT>
    double minVal(TINT i) const
    {
        return _stats[i].min();
    }

    /// @brief Returns the maximum observed value of the Stats object at index i.
    /// @tparam TINT Integer index type.
    /// @param i Zero-based index.
    /// @return Maximum value as a double.
    template <typename TINT>
    double maxVal(TINT i) const
    {
        return _stats[i].max();
    }

    /// @brief Returns the number of samples inserted into the Stats object at index i.
    /// @tparam TINT Integer index type.
    /// @param i Zero-based index.
    /// @return Number of entries as size_t.
    template <typename TINT>
    size_t getNoOfEntries(TINT i) const
    {
        return _stats[i].getNoOfEntries();
    }

};

} //namespace math
