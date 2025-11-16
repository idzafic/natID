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
#include <mu/DbgTimer.h>

#include <type_traits>

#ifdef MU_WINDOWS
    #include <windows.h>
#else
    #include <chrono>
#endif


namespace mu
{

// Define ClockID concept to accept both integral types and enums with integral underlying types
template<typename T>
concept ClockID = std::is_integral_v<T> ||
                  (std::is_enum_v<T> && std::is_integral_v<std::underlying_type_t<T>>);


/// Class Timer used for time profiling. It can have more than one clock
template <td::UINT4 SIZE = 0>
class Timer
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
public:
    template<ClockID T>
    inline double getTimeInMilliSec(T iClock) const
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        assert(clockIndex <= SIZE);
#ifdef MU_WINDOWS
        return (1000.0 * _times[clockIndex].totalTime) / liFreq;
#else
        return _times[clockIndex].totalTime / 1000000.0;
#endif
    }
    
    inline double getTimeInMilliSec() const
    {
        return getTimeInMilliSec(td::UINT4(0));
    }
    
protected:
    template <ClockID T, typename... Args>
    inline double getCumTimeInMilliSec(T iClock) const
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        assert(clockIndex <= SIZE);
        double totalTime = getTimeInMilliSec(iClock);
        return totalTime;
    }
    
    template <ClockID T, typename... Args>
    inline double getCumTimeInMilliSec(T iFirstClock, Args... args) const
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iFirstClock);
        assert(clockIndex <= SIZE);
        double totalTime = getTimeInMilliSec(iFirstClock) + getCumTimeInMilliSec(args...);
        return totalTime;
    }
    
    template <ClockID T, typename... Args>
    inline double getCumTimeInSec(T iClock) const
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        assert(clockIndex <= SIZE);
        double totalTime = getTimeInSec(iClock);
        return totalTime;
    }
    
    template <ClockID T, typename... Args>
    inline double getCumTimeInSec(T iFirstClock, Args... args) const
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iFirstClock);
        assert(clockIndex <= SIZE);
        double totalTime = getTimeInSec(iFirstClock) + getCumTimeInSec(args...);
        return totalTime;
    }
public:
    template<ClockID T>
    inline double getTimeInSec(T iClock) const
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        assert(clockIndex <= SIZE);
#ifdef MU_WINDOWS
        return (double)_times[clockIndex].totalTime / liFreq;
#else
        return _times[clockIndex].totalTime / 1000000000.0;
#endif
    }
    
    inline double getTimeInSec() const
    {
        return getTimeInSec(td::UINT4(0));
    }
    
protected:
    
    template <class TSTR>
    inline void printTimeInSec(TSTR& s, td::UINT4 iClock, const char* pName, double totalTime, td::UINT4 nCalls) const
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
    inline void printTimeInMillisec(TSTR& s, td::UINT4 iClock, const char* pName, double totalTime, td::UINT4 nCalls) const
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
    
public:
    Timer()
    {
#ifdef MU_WINDOWS
        LARGE_INTEGER tmp;
        QueryPerformanceFrequency(&tmp);
        liFreq = tmp.QuadPart;
#endif
        _times[0].name = "TOTAL";
    }
    
    explicit Timer(const char* timerName, bool startMeasuring = true)
    {
#ifdef MU_WINDOWS
        LARGE_INTEGER tmp;
        QueryPerformanceFrequency(&tmp);
        liFreq = tmp.QuadPart;
#endif
        setName(0, timerName, startMeasuring);
    }
    
    template <ClockID T>
    void setName(T iClock, const char* clockName, bool startMeasuring = false)
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        assert(clockIndex <= SIZE);
        _times[clockIndex].name = clockName;
        if (startMeasuring)
            measure(iClock);
    }
    
    template <ClockID T>
    inline void pause(T iClock)
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        assert(clockIndex <= SIZE);
        if (_times[clockIndex].status == 0)
            return;
        
        TVar2 countEnd = getCurrentTime();
        _times[clockIndex].status = 0; //set to pause
#ifdef MU_WINDOWS
        _times[clockIndex].totalTime += (countEnd - _times[clockIndex].lastStartTime);
#else
        _times[clockIndex].totalTime += std::chrono::duration_cast<std::chrono::nanoseconds>(countEnd - _times[clockIndex].lastStartTime).count();
#endif
    }
    
    inline void pause()
    {
        pause(td::UINT4(0));
    }

    inline void pauseAll()
    {
        for (td::UINT4 i=0; i<= SIZE; ++i)
            pause(i);
    }

    template <ClockID T>
    inline void measure(T iClock)
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        assert(clockIndex <= SIZE);
        if (_times[clockIndex].status != 0)
            return;

        _times[clockIndex].nCalls++;
        _times[clockIndex].status = 1; //set to measure
        _times[clockIndex].lastStartTime = getCurrentTime();
    }

    template <ClockID T>
    inline void reset(T iClock)
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        assert(clockIndex <= SIZE);
        _times[clockIndex].nCalls = 0;
        _times[clockIndex].status = 0; //set to measure
#ifdef MU_WINDOWS
        _times[clockIndex].totalTime = 0; //set to measure
#else
        _times[clockIndex].totalTime = {}; //set to measure
#endif
        _times[clockIndex].lastStartTime = getCurrentTime();
    }
    
    inline void reset()
    {
        reset(td::UINT4{0});
    }
    
    inline void measure()
    {
        measure(td::UINT4{0});
    }

    inline void resetAll()
    {
        for (td::UINT4 i = 0; i <= SIZE; ++i)
            reset(i);
    }

    inline void measureAll()
    {
        for (td::UINT4 i = 0; i <= SIZE; ++i)
            measure(i);
    }
    
    template <ClockID T, class TSTREAM>
    void showTimeInMillisec(TSTREAM& s, T iClock = T{0}) const
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        assert(clockIndex <= SIZE);
        auto nCalls = _times[clockIndex].nCalls;
        if (nCalls == 0)
            return;
        double totalTime = getTimeInMilliSec(iClock);

        const char* pName = _times[clockIndex].name;
        printTimeInMillisec(s, clockIndex, pName, totalTime, nCalls);
    }

    template <ClockID T, class TSTREAM>
    void showTimeInSec(TSTREAM& s, T iClock = T{0}) const
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        assert(clockIndex <= SIZE);
        auto nCalls = _times[clockIndex].nCalls;
        if (nCalls == 0)
            return;
        double totalTime = getTimeInSec(iClock);
        
        printTimeInSec(s, clockIndex, _times[clockIndex].name, totalTime, nCalls);
    }

    template <class TSTREAM>
    void showAllTimesInMillisec(TSTREAM& o) const
    {
        for (td::UINT4 i = 0; i <= SIZE; ++i)
        {
            showTimeInMillisec(o, i);
        }
    }

    template <class TSTREAM>
    void showTimesInSec(TSTREAM& o) const
    {
        for (td::UINT4 i = 0; i <= SIZE; ++i)
        {
            showTimeInSec(o, i);
        }
    }

    template <ClockID T>
    inline bool isRunning(T iClock = T{0})
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        assert(clockIndex <= SIZE);
        return (_times[clockIndex].status == 1);
    }

    template<class TSTR, ClockID T, typename... Args>
    void showSumInMilliseconds(TSTR& s, const char* lbl, T firstClock, Args... args)
    {
        double totalTime = getTimeInMilliSec(firstClock) + getCumTimeInMilliSec(args...);
        printTimeInMillisec(s, 0, lbl, totalTime, 0);
    }
    
    template<class TSTR, ClockID T, typename... Args>
    void showSumInSec(TSTR& s, const char* lbl, T firstClock, Args... args)
    {
        double totalTime = getTimeInSec(firstClock) + getCumTimeInSec(args...);
        printTimeInSec(s, 0, lbl, totalTime, 0);
    }
    
    template<class TSTR>
    void showDeltaInMilliseconds(TSTR& s, const char* lbl="Delta")
    {
        double totalTime = getTimeInMilliSec(0);
        double other= 0;
        for (td::UINT4 iClock = 1; iClock<=SIZE; ++iClock)
            other += getTimeInMilliSec(iClock);
        double delta = totalTime - other;
        printTimeInMillisec(s, 0, lbl, delta, 0);
    }
    
    template<class TSTR>
    void showDeltaInSeconds(TSTR& s, const char* lbl="Delta")
    {
        double totalTime = getTimeInSec(0);
        double other= 0;
        for (td::UINT4 iClock = 1; iClock<=SIZE; ++iClock)
            other += getTimeInSec(iClock);
        double delta = totalTime - other;
        printTimeInSec(s, 0, lbl, delta, 0);
    }
protected:
    template<ClockID T, class TMUTABLESTR>
    void getInfoInSeconds(TMUTABLESTR& mStr, T iClock = T{0}) const
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        td::UINT4 nCalls = _times[clockIndex].nCalls;
        double t = getTimeInSec(iClock);
        const char* clockName = _times[clockIndex].name;
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
    }
    
    template<ClockID T, class TMUTABLESTR>
    void getInfoInMilliSeconds(TMUTABLESTR& mStr, T iClock = T{0}) const
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        td::UINT4 nCalls = _times[clockIndex].nCalls;
        double t = getTimeInMilliSec(iClock);
        const char* clockName = _times[clockIndex].name;
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
    }
    
    template<ClockID T, class TMUTABLESTR>
    void getInfo(TMUTABLESTR& mStr, T iClock, TimerUnits units) const
    {
        if (units == TimerUnits::MilliSeconds)
            getInfoInMilliSeconds(mStr, iClock);
        else
            getInfoInSeconds(mStr, iClock);
    }
    
public:
    
    inline constexpr td::UINT4 size() const
    {
        return SIZE+1;
    }
    
    template <ClockID T>
    td::UINT4 getNumberOfCalls(T iClock = T{0}) const
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        return _times[clockIndex].nCalls;
    }
    
    inline void reportTimes(TimerCallBack callBackFn, TimerUnits units) const
    {
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
    }
    
    template <ClockID T>
    inline void reportTimes(TimerCallBack callBackFn, td::UINT4 step, T iClock, TimerUnits units) const
    {
        td::UINT4 nCalls = getNumberOfCalls(iClock);
        if (nCalls % step == 0)
        {
            reportTimes(callBackFn, units);
        }
    }
    
    template <ClockID T>
    inline void getTime(td::MutableString& mStr, T iClock)
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        if (mStr.capacity() < 128)
            mStr.reserve(128);
        
        mStr.reset();
        double timeInSec = getTimeInSec(iClock);
        if (timeInSec >= 1.0)
            mStr.appendFormat("%s: %.3f seconds", _times[clockIndex].name, timeInSec);
        else
        {
            double timeInMilliSec = getTimeInMilliSec(iClock);
            mStr.appendFormat("%s: %.3f milliseconds", _times[clockIndex].name, timeInMilliSec);
        }
    }

};

} //namespace mu
