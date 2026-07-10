// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DbgTimer.h
 * @brief Debug timer class for time profiling with multiple named clocks.
 */
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

/// @brief Callback function type for timer reporting.
using TimerCallBack = void (*)(const char*);
/// @brief Units used for reporting timer values.
enum class TimerUnits : td::BYTE {Seconds = 0, MilliSeconds=3};

/// Class Timer used for time profiling. It can have more than one clock
/// @brief Template debug timer supporting multiple named clocks for profiling.
template <td::UINT4 SIZE = 0>
class DbgTimer
{
    /// @brief Internal structure holding per-clock timing data.
    template <typename T1, typename T2>
    struct TimerEntry
    {
        const char* name = nullptr; ///< Name label for this clock entry
        T1 totalTime = 0;           ///< Accumulated elapsed time
        T2 lastStartTime;           ///< Timestamp when the last measurement started
        td::UINT4 nCalls = 0;       ///< Number of times measure() was called
        td::UINT4 status = 0;       ///< 0 = paused/stopped, 1 = currently measuring
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
    cnt::Array< struct TimerEntry<TVar, TVar2>, SIZE + 1> _times; ///< Array of timer entries, index 0 is the cumulative total
#ifdef MU_WINDOWS
    TVar liFreq; ///< Performance counter frequency (Windows only)
#endif

    /// @brief Returns the current high-resolution timestamp.
    /// @return Current time as a platform-specific value.
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

    /// @brief Returns the total elapsed time for a clock in milliseconds.
    /// @param iClock Index of the clock (default 0).
    /// @return Elapsed time in milliseconds.
    inline double getTimeInMilliSec([[maybe_unused]] size_t iClock = 0) const
    {
        assert(iClock <= SIZE);
#ifdef MU_WINDOWS
        return (1000.0 * _times[iClock].totalTime) / liFreq;
#else
        return _times[iClock].totalTime / 1000000.0;
#endif
    }

    /// @brief Recursively accumulates milliseconds for a single clock (base case).
    /// @param iClock Clock index.
    /// @return Elapsed time in milliseconds for this clock.
    template <typename T, typename... Args>
    inline double getCumTimeInMilliSec(T iClock) const
    {
        assert(iClock <= SIZE);
        double totalTime = getTimeInMilliSec(iClock);
        return totalTime;
    }

    /// @brief Recursively accumulates milliseconds across multiple clocks.
    /// @param iFirstClock First clock index.
    /// @param args Remaining clock indices.
    /// @return Sum of elapsed times in milliseconds.
    template <typename T, typename... Args>
    inline double getCumTimeInMilliSec(T iFirstClock, Args... args) const
    {
        assert(iFirstClock <= SIZE);
        double totalTime = getTimeInMilliSec(iFirstClock) + getCumTimeInMilliSec(args...);
        return totalTime;
    }

    /// @brief Recursively accumulates seconds for a single clock (base case).
    /// @param iClock Clock index.
    /// @return Elapsed time in seconds.
    template <typename T, typename... Args>
    inline double getCumTimeInSec(T iClock) const
    {
        assert(iClock <= SIZE);
        double totalTime = getTimeInSec(iClock);
        return totalTime;
    }

    /// @brief Recursively accumulates seconds across multiple clocks.
    /// @param iFirstClock First clock index.
    /// @param args Remaining clock indices.
    /// @return Sum of elapsed times in seconds.
    template <typename T, typename... Args>
    inline double getCumTimeInSec(T iFirstClock, Args... args) const
    {
        assert(iFirstClock <= SIZE);
        double totalTime = getTimeInSec(iFirstClock) + getCumTimeInSec(args...);
        return totalTime;
    }

    /// @brief Returns the total elapsed time for a clock in seconds.
    /// @param iClock Index of the clock (default 0).
    /// @return Elapsed time in seconds.
    inline double getTimeInSec([[maybe_unused]] size_t iClock = 0) const
    {
        assert(iClock <= SIZE);
#ifdef MU_WINDOWS
        return (double)_times[iClock].totalTime / liFreq;
#else
        return _times[iClock].totalTime / 1000000000.0;
#endif
    }

    /// @brief Writes a formatted seconds timing line to a stream.
    /// @param s Output stream.
    /// @param iClock Clock index.
    /// @param pName Display name for this clock.
    /// @param totalTime Total elapsed time in seconds.
    /// @param nCalls Number of measurement calls.
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

    /// @brief Writes a formatted milliseconds timing line to a stream.
    /// @param s Output stream.
    /// @param iClock Clock index.
    /// @param pName Display name for this clock.
    /// @param totalTime Total elapsed time in milliseconds.
    /// @param nCalls Number of measurement calls.
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
    /// @brief Default constructor; initializes the performance counter frequency and names clock 0 "TOTAL".
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

    /// @brief Constructor that names clock 0 and optionally starts measuring immediately.
    /// @param timerName Name string for clock 0.
    /// @param startMeasuring If true, begins measuring immediately after construction.
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

    /// @brief Assigns a name to a clock and optionally starts it immediately.
    /// @param iClock Index of the clock to name.
    /// @param clockName Name string to assign.
    /// @param startMeasuring If true, calls measure() after naming.
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

    /// @brief Pauses a running clock and accumulates the elapsed time.
    /// @param iClock Index of the clock to pause (default 0).
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

    /// @brief Pauses all clocks simultaneously.
    inline void pauseAll()
    {
#ifdef MU_USETIMER
        for (td::UINT4 i=0; i<= SIZE; ++i)
            pause(i);
#endif
    }

    /// @brief Starts (or resumes) measuring on a clock.
    /// @param iClock Index of the clock to start (default 0).
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

    /// @brief Resets a clock's accumulated time and call count to zero.
    /// @param iClock Index of the clock to reset (default 0).
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

    /// @brief Resets all clocks.
    inline void resetAll()
    {
#ifdef MU_USETIMER
        for (size_t i = 0; i <= SIZE; ++i)
            reset(i);
#endif
    }

    /// @brief Starts measuring on all clocks.
    inline void measureAll()
    {
#ifdef MU_USETIMER
        for (size_t i = 0; i <= SIZE; ++i)
            measure(i);
#endif
    }


    /// @brief Outputs the elapsed time for a single clock in milliseconds to a stream.
    /// @param s Output stream.
    /// @param iClock Index of the clock (default 0).
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

    /// @brief Outputs the elapsed time for a single clock in seconds to a stream.
    /// @param s Output stream.
    /// @param iClock Index of the clock (default 0).
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

    /// @brief Outputs elapsed times for all clocks in milliseconds to a stream.
    /// @param o Output stream.
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

    /// @brief Outputs elapsed times for all clocks in seconds to a stream.
    /// @param o Output stream.
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

    /// @brief Checks whether a clock is currently measuring.
    /// @param iClock Index of the clock (default 0).
    /// @return True if the clock is running, false otherwise.
    inline bool isRunning([[maybe_unused]] td::UINT4 iClock = 0)
    {
#ifdef MU_USETIMER
        assert(iClock <= SIZE);
        return _times[iClock].status == 1;
#else
        return false;
#endif
    }

    /// @brief Outputs the sum of specified clocks in milliseconds with a label.
    /// @param s Output stream.
    /// @param lbl Label string for the sum.
    /// @param firstClock First clock index.
    /// @param args Additional clock indices.
    template<class TSTR, typename T, typename... Args>
    void showSumInMilliseconds([[maybe_unused]] TSTR& s, [[maybe_unused]] const char*lbl, [[maybe_unused]] T firstClock, Args... args)
    {
#ifdef MU_USETIMER
        double totalTime = getTimeInMilliSec(firstClock) + getCumTimeInMilliSec(args...);
        printTimeInMillisec(s, 0, lbl, totalTime, 0);
#endif
    }

    /// @brief Outputs the sum of specified clocks in seconds with a label.
    /// @param s Output stream.
    /// @param lbl Label string for the sum.
    /// @param firstClock First clock index.
    /// @param args Additional clock indices.
    template<class TSTR, typename T, typename... Args>
    void showSumInSec([[maybe_unused]] TSTR& s, [[maybe_unused]] const char*lbl, [[maybe_unused]] T firstClock, Args... args)
    {
#ifdef MU_USETIMER
        double totalTime = getTimeInSec(firstClock) + getCumTimeInSec(args...);
        printTimeInSec(s, 0, lbl, totalTime, 0);
#endif
    }

    /// @brief Outputs the difference between clock 0 and the sum of all other clocks in milliseconds.
    /// @param s Output stream.
    /// @param lbl Label string (default "Delta").
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

    /// @brief Outputs the difference between clock 0 and the sum of all other clocks in seconds.
    /// @param s Output stream.
    /// @param lbl Label string (default "Delta").
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
    /// @brief Appends timing info for a clock (in seconds) to a mutable string.
    /// @param mStr Output mutable string.
    /// @param iClock Index of the clock.
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

    /// @brief Appends timing info for a clock (in milliseconds) to a mutable string.
    /// @param mStr Output mutable string.
    /// @param iClock Index of the clock.
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

    /// @brief Appends timing info for a clock in the specified units to a mutable string.
    /// @param mStr Output mutable string.
    /// @param iClock Index of the clock.
    /// @param units Units to use (Seconds or MilliSeconds).
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

    /// @brief Returns the total number of clocks (SIZE + 1).
    /// @return Number of clocks.
    inline constexpr td::UINT4 size() const
    {
        return SIZE+1;
    }

    /// @brief Returns the number of times measure() was called on a clock.
    /// @param iClock Index of the clock.
    /// @return Call count.
    td::UINT4 getNumberOfCalls([[maybe_unused]] td::UINT4 iClock) const
    {
#ifdef MU_USETIMER
        return _times[iClock].nCalls;
#else
        return 0;
#endif
    }

    /// @brief Reports timing information for all clocks via a callback function.
    /// @param callBackFn Callback to receive each formatted timing string.
    /// @param units Units to use for reporting.
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

    /// @brief Reports timing information every N calls on a specific clock.
    /// @param callBackFn Callback to receive each formatted timing string.
    /// @param step Report interval (report when nCalls % step == 0).
    /// @param iClock Clock index to check.
    /// @param units Units to use for reporting.
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
