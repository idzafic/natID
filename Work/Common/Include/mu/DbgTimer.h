// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <cnt/Array.h>
#include <td/Types.h>
#include <td/MutableString.h>

#ifdef MU_USETIMER
	#ifdef MU_WINDOWS
		#include <windows.h>	
	#else
		#include <chrono>
	#endif
#endif


namespace mu
{

using TimerCallBack = void (*)(const char*);
enum class TimerUnits : td::BYTE {Seconds = 0, MilliSeconds=3};

/// Class Timer used for time profiling. It can have more than one clock
template <td::UINT4 SIZE = 0>
class DbgTimer
{
    template <typename T1, typename T2>
    struct TimerEntry
    {
        const char* name = nullptr;
        T1 totalTime = 0;
        T2 lastStartTime;
        td::UINT4 nCalls = 0;
        td::UINT4 status = 0;
    };
    
#ifdef MU_USETIMER
#ifdef MU_WINDOWS
    using TVar = td::LINT8;
    using TVar2 = td::LINT8;
#else
    using TVar = td::LINT8;
    using TVar2 = std::chrono::high_resolution_clock::time_point;
#endif

protected:
    cnt::Array< struct TimerEntry<TVar, TVar2>, SIZE + 1> _times;
#ifdef MU_WINDOWS
    TVar liFreq;
#endif

    inline TVar2 getCurrentTime() const
    {
#ifdef MU_WINDOWS
        LARGE_INTEGER toRet;
        QueryPerformanceCounter(&toRet);
        return toRet.QuadPart;
#else
        return std::chrono::high_resolution_clock::now();
#endif
    }
    
    inline double getTimeInMilliSec([[maybe_unused]] size_t iClock = 0) const
    {
        assert(iClock <= SIZE);
#ifdef MU_WINDOWS
        return (1000.0 * _times[iClock].totalTime) / liFreq;
#else
        return _times[iClock].totalTime / 1000000.0;
#endif
    }
    
    template <typename T, typename... Args>
    inline double getCumTimeInMilliSec(T iClock) const
    {
        assert(iClock <= SIZE);
        double totalTime = getTimeInMilliSec(iClock);
        return totalTime;
    }
    
    template <typename T, typename... Args>
    inline double getCumTimeInMilliSec(T iFirstClock, Args... args) const
    {
        assert(iFirstClock <= SIZE);
        double totalTime = getTimeInMilliSec(iFirstClock) + getCumTimeInMilliSec(args...);
        return totalTime;
    }
    
    template <typename T, typename... Args>
    inline double getCumTimeInSec(T iClock) const
    {
        assert(iClock <= SIZE);
        double totalTime = getTimeInSec(iClock);
        return totalTime;
    }
    
    template <typename T, typename... Args>
    inline double getCumTimeInSec(T iFirstClock, Args... args) const
    {
        assert(iFirstClock <= SIZE);
        double totalTime = getTimeInSec(iFirstClock) + getCumTimeInSec(args...);
        return totalTime;
    }

    inline double getTimeInSec([[maybe_unused]] size_t iClock = 0) const
    {
        assert(iClock <= SIZE);
#ifdef MU_WINDOWS
        return (double)_times[iClock].totalTime / liFreq;
#else
        return _times[iClock].totalTime / 1000000000.0;
#endif
    }
    
    template <class TSTR>
    inline void printTimeInSec(TSTR& s, size_t iClock, const char* pName, double totalTime, td::UINT4 nCalls) const
    {
        if (pName == nullptr)
            pName = "NA";
        if (nCalls > 0)
        {
            double avgTime = totalTime / nCalls;
            s << iClock << ". " << pName << ": total=" << totalTime << "[s], n=" << nCalls << ", avg=" << avgTime << "[s]" << td::endl;
        }
        else
        {
            if (iClock > 0)
                s << iClock << ". " << pName << ": total=" << totalTime << "[s]" << td::endl;
            else
                s << "C. " << pName << ": total=" << totalTime << "[s]" << td::endl;
        }
    }
    
    template <class TSTR>
    inline void printTimeInMillisec(TSTR& s, size_t iClock, const char* pName, double totalTime, td::UINT4 nCalls) const
    {
        if (pName == nullptr)
            pName = "NA";
        if (nCalls > 0)
        {
            double avgTime = totalTime / nCalls;
            s << iClock << ". " << pName << ": total=" << totalTime << "[ms], n=" << nCalls << ", avg=" << avgTime << "[ms]" << td::endl;
        }
        else
        {
            if (iClock > 0)
                s << iClock << ". " << pName << ": total=" << totalTime << "[ms]" << td::endl;
            else
                s << "C. " << pName << ": total=" << totalTime << "[ms]" << td::endl;
        }
    }
#endif
    
public:
    DbgTimer()
    {
#ifdef MU_USETIMER
#ifdef MU_WINDOWS
        LARGE_INTEGER tmp;
        QueryPerformanceFrequency(&tmp);
        liFreq = tmp.QuadPart;
#endif
        _times[0].name = "TOTAL";
#endif
    }

    template <size_t size>
    explicit DbgTimer([[maybe_unused]] const char(&timerName)[size], [[maybe_unused]] bool startMeasuring = true)
    {
#ifdef MU_USETIMER
#ifdef MU_WINDOWS
        LARGE_INTEGER tmp;
        QueryPerformanceFrequency(&tmp);
        liFreq = tmp.QuadPart;
#endif
        setName(0, timerName, startMeasuring);
        //_times[0].name = &timerName[0];
#endif
    }

    template <size_t size>
    void setName([[maybe_unused]] td::UINT4 iClock, [[maybe_unused]] const char(&clockName)[size], [[maybe_unused]] bool startMeasuring = false)
    {
        
#ifdef MU_USETIMER
        assert(iClock <= SIZE);
        _times[iClock].name = &clockName[0];
        if (startMeasuring)
            measure(iClock);
#endif
        
    }

    inline void pause([[maybe_unused]] td::UINT4 iClock = 0)
    {
#ifdef MU_USETIMER
        assert(iClock <= SIZE);
        if (_times[iClock].status == 0)
            return;
        
        TVar2 countEnd = getCurrentTime();
        _times[iClock].status = 0; //set to pause
#ifdef MU_WINDOWS
        _times[iClock].totalTime += (countEnd - _times[iClock].lastStartTime);
#else
        _times[iClock].totalTime += std::chrono::duration_cast<std::chrono::nanoseconds>(countEnd - _times[iClock].lastStartTime).count();
#endif
#endif
    }

    inline void pauseAll()
    {
#ifdef MU_USETIMER
        for (td::UINT4 i=0; i<= SIZE; ++i)
            pause(i);
#endif
    }

    inline void measure([[maybe_unused]] td::UINT4 iClock = 0)
    {
#ifdef MU_USETIMER
        assert(iClock <= SIZE);
        if (_times[iClock].status != 0)
            return;

        _times[iClock].nCalls++;
        _times[iClock].status = 1; //set to measure
        _times[iClock].lastStartTime = getCurrentTime();
#endif
    }

    inline void reset([[maybe_unused]] td::UINT4 iClock = 0)
    {
#ifdef MU_USETIMER
        assert(iClock <= SIZE);
        _times[iClock].nCalls = 0;
        _times[iClock].status = 0; //set to measure
#ifdef MU_WINDOWS
            _times[iClock].totalTime = 0; //set to measure
#else
            _times[iClock].totalTime = {}; //set to measure
#endif
        _times[iClock].lastStartTime = getCurrentTime();
#endif
    }

    inline void resetAll()
    {
#ifdef MU_USETIMER
        for (size_t i = 0; i <= SIZE; ++i)
            reset(i);
#endif
    }

    inline void measureAll()
    {
#ifdef MU_USETIMER
        for (size_t i = 0; i <= SIZE; ++i)
            measure(i);
#endif
    }
    

    template <class TSTREAM>
    void showTimeInMillisec([[maybe_unused]] TSTREAM& s, [[maybe_unused]] td::UINT4 iClock = 0) const
    {
#ifdef MU_USETIMER
        assert(iClock <= SIZE);
        auto nCalls = _times[iClock].nCalls;
        if (nCalls == 0)
            return;
        double totalTime = getTimeInMilliSec(iClock);
        //showTimeInMillisec(TSTR& s, size_t iClock, const char* pName, double totalTime, td::UINT4 nCalls)
        const char* pName = _times[iClock].name;
        printTimeInMillisec(s, iClock, pName,  totalTime, nCalls);
        
//			double avgTime = totalTime / nCalls;
//
//            if (_times[iClock].name != nullptr)
//                o << iClock << ". " << _times[iClock].name << ": total=" << totalTime << "[ms], n=" << nCalls << ", avg=" << avgTime << "[ms]" << td::endl;
//            else
//                o << iClock << ". NA: total=" << totalTime << "[ms], n=" << nCalls << ", avg=" << avgTime << "[ms]" << td::endl;
#endif
    }

    template <class TSTREAM>
    void showTimeInSec([[maybe_unused]] TSTREAM& s, [[maybe_unused]] td::UINT4 iClock = 0) const
    {
#ifdef MU_USETIMER
        assert(iClock <= SIZE);
        auto nCalls = _times[iClock].nCalls;
        if (nCalls == 0)
            return;
        double totalTime = getTimeInSec(iClock);
        
        printTimeInSec(s, iClock, _times[iClock].name,  totalTime, nCalls);
//
//			double avgTime = totalTime / nCalls;
//
//
//			if (_times[iClock].name != nullptr)
//                o << iClock << ". " << _times[iClock].name << ": total=" << totalTime << "[s], n=" << nCalls << ", avg=" << avgTime << "[s]" << td::endl;
//			else
//                o << iClock << ". NA: total=" << totalTime << "[s], n=" << nCalls << ", avg=" << avgTime << "[s]" << td::endl;
#endif
    }

    template <class TSTREAM>
    void showAllTimesInMillisec([[maybe_unused]]TSTREAM& o) const
    {
#ifdef MU_USETIMER
        for (size_t i = 0; i <= SIZE; ++i)
        {
            showTimeInMillisec(o, i);
        }
#endif
    }

    template <class TSTREAM>
    void showTimesInSec([[maybe_unused]] TSTREAM& o) const
    {
#ifdef MU_USETIMER
        for (size_t i = 0; i <= SIZE; ++i)
        {
            showTimeInSec(o, i);
        }
#endif
    }

    inline bool isRunning([[maybe_unused]] td::UINT4 iClock = 0)
    {
#ifdef MU_USETIMER
        assert(iClock <= SIZE);
        return _times[iClock].status == 1;
#else
        return false;
#endif
    }

    template<class TSTR, typename T, typename... Args>
    void showSumInMilliseconds([[maybe_unused]] TSTR& s, [[maybe_unused]] const char*lbl, [[maybe_unused]] T firstClock, Args... args)
    {
#ifdef MU_USETIMER
        double totalTime = getTimeInMilliSec(firstClock) + getCumTimeInMilliSec(args...);
        printTimeInMillisec(s, 0, lbl, totalTime, 0);
#endif
    }
    
    template<class TSTR, typename T, typename... Args>
    void showSumInSec([[maybe_unused]] TSTR& s, [[maybe_unused]] const char*lbl, [[maybe_unused]] T firstClock, Args... args)
    {
#ifdef MU_USETIMER
        double totalTime = getTimeInSec(firstClock) + getCumTimeInSec(args...);
        printTimeInSec(s, 0, lbl, totalTime, 0);
#endif
    }
    
    template<class TSTR>
    void showDeltaInMilliseconds([[maybe_unused]] TSTR& s, [[maybe_unused]] const char* lbl="Delta")
    {
#ifdef MU_USETIMER
        double totalTime = getTimeInMilliSec(0);
        double other= 0;
        for (size_t iClock = 1; iClock<=SIZE; ++iClock)
            other += getTimeInMilliSec(iClock);
        double delta = totalTime - other;
        printTimeInMillisec(s, 0, lbl, delta, 0);
#endif
    }
    
    template<class TSTR>
    void showDeltaInSeconds([[maybe_unused]] TSTR& s, [[maybe_unused]] const char* lbl="Delta")
    {
#ifdef MU_USETIMER
        double totalTime = getTimeInSec(0);
        double other= 0;
        for (size_t iClock = 1; iClock<=SIZE; ++iClock)
            other += getTimeInSec(iClock);
        double delta = totalTime - other;
        printTimeInSec(s, 0, lbl, delta, 0);
#endif
    }
protected:
    template<class TMUTABLESTR>
    void getInfoInSeconds([[maybe_unused]] TMUTABLESTR& mStr, [[maybe_unused]] td::UINT4 iClock) const
    {
#ifdef MU_USETIMER
//        mStr.reserve(128);
        td::UINT4 nCalls = _times[iClock].nCalls;
        double t = getTimeInSec(iClock);
        const char* clockName = _times[iClock].name;
        if (clockName)
        {
            if (nCalls > 0)
                mStr.appendFormat("%s: t=%.6fs, n=%u, avg=%.6fs\n", clockName, t, nCalls, t / nCalls);
            else
                mStr.appendFormat("%s: t=%.6fs, n=0, avg=0s\n", clockName, t);
        }
        else
        {
            if (nCalls > 0)
                mStr.appendFormat("t=%.6fs, n=%u, avg=%.6fs\n", t, nCalls, t / nCalls);
            else
                mStr.appendFormat("t=%.6fs, n=0, avg=0s\n", t);
        }
        
#endif
    }
    
    template<class TMUTABLESTR>
    void getInfoInMilliSeconds([[maybe_unused]] TMUTABLESTR& mStr, [[maybe_unused]] td::UINT4 iClock) const
    {
#ifdef MU_USETIMER
//        mStr.reserve(128);
        td::UINT4 nCalls = _times[iClock].nCalls;
        double t = getTimeInMilliSec(iClock);
        const char* clockName = _times[iClock].name;
        if (clockName)
        {
            if (nCalls > 0)
                mStr.appendFormat("%s: t=%.6fms, n=%u, avg=%.6fms\n", clockName, t, nCalls, t / nCalls);
            else
                mStr.appendFormat("%s: t=%.6fms, n=0, avg=0ms\n", clockName, t);
        }
        else
        {
            if (nCalls > 0)
                mStr.appendFormat("t=%.6fms, n=%u, avg=%.6fms\n", t, nCalls, t / nCalls);
            else
                mStr.appendFormat("t=%.6fms, n=0, avg=0ms\n", t);
        }
        
#endif
    }
    
    template<class TMUTABLESTR>
    void getInfo([[maybe_unused]] TMUTABLESTR& mStr, [[maybe_unused]] td::UINT4 iClock, [[maybe_unused]]  TimerUnits units) const
    {
#ifdef MU_USETIMER
        if (units == TimerUnits::MilliSeconds)
            getInfoInMilliSeconds(mStr, iClock);
        else
            getInfoInSeconds(mStr, iClock);
#endif
    }
    
public:
    
    inline constexpr td::UINT4 size() const
    {
        return SIZE+1;
    }
    
    td::UINT4 getNumberOfCalls([[maybe_unused]] td::UINT4 iClock) const
    {
#ifdef MU_USETIMER
        return _times[iClock].nCalls;
#else
        return 0;
#endif
    }
    
    inline void reportTimes([[maybe_unused]] TimerCallBack callBackFn, [[maybe_unused]] TimerUnits units) const
    {
#ifdef MU_USETIMER
        td::UINT4 nClocks = size();
        td::MutableString mStr;
        mStr.reserve(64);
        callBackFn("----------------");
        for (td::UINT4 i=0; i<nClocks; ++i)
        {
            getInfo(mStr, i, units);
            const td::String& str = mStr.getString();
            callBackFn(str.c_str());
        }
        callBackFn("----------------");
#endif
    }
    
    inline void reportTimes([[maybe_unused]] TimerCallBack callBackFn,[[maybe_unused]]  td::UINT4 step, [[maybe_unused]] td::UINT4 iClock, TimerUnits units) const
    {
#ifdef MU_USETIMER
        td::UINT4 nCalls = getNumberOfCalls(iClock);
        if (nCalls % step == 0)
        {
            reportTimes(callBackFn, units);
        }
#endif
    }
    
};

} //namespace mu
