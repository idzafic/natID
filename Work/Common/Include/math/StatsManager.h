// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <math/Stats.h>
#include <cnt/SafeFullVector.h>

namespace math
{

class StatsManager
{
    
protected:
    cnt::SafeFullVector<math::Stats> _stats;
    
public:
    StatsManager()
    {}
    
    void reserve(size_t nStats)
    {
        _stats.reserve(nStats);
    }
    
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
    
    const td::String& getName(size_t iPos) const
    {
        return _stats[iPos].getName();
    }
    
    template <typename TINT>
    Stats& getStats(TINT i)
    {
        return _stats[i];
    }
    
    template <typename TINT>
    const Stats& getStats(TINT i) const
    {
        return _stats[i];
    }
    
    template <typename TINT>
    void insert(TINT i, double x)
    {
        _stats[i].insert(x);
    }
    
    template <typename TINT>
    double var(TINT i) const
    {
        return _stats[i].var();
    }
    
    template <typename TINT>
    double dev(TINT i) const
    {
        return _stats[i].dev();
    }
    
    template <typename TINT>
    double avg(TINT i) const
    {
        return _stats[i].avg();
    }
    
    template <typename TINT>
    double minVal(TINT i) const
    {
        return _stats[i].min();
    }
    
    template <typename TINT>
    double maxVal(TINT i) const
    {
        return _stats[i].max();
    }
    
    template <typename TINT>
    size_t getNoOfEntries(TINT i) const
    {
        return _stats[i].getNoOfEntries();
    }
    
};

} //namespace math

