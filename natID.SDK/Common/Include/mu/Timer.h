// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Timer.h
    @brief High-resolution multi-clock profiling timer with per-clock accumulation and reporting. */
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
/// @brief High-resolution profiling timer supporting multiple named clocks and cumulative measurements.
/// @tparam SIZE Number of additional clocks beyond the default clock (index 0). Total clocks = SIZE+1.
template <td::UINT4 SIZE = 0>
class Timer
{

    /// @brief Internal structure holding accumulated time and call count for a single clock.
    /// @tparam T1 Type used for accumulated total time.
    /// @tparam T2 Type used for the last start time snapshot.
    template <typename T1, typename T2>
    struct TimerEntry
    {
        const char* name = nullptr; ///< Optional human-readable name for this clock.
        T1 totalTime = 0;           ///< Accumulated elapsed time in platform-specific units.
        T2 lastStartTime;           ///< Timestamp recorded at the last call to measure().
        td::UINT4 nCalls = 0;       ///< Number of times measure() has been called for this clock.
        td::UINT4 status = 0;       ///< 0 = paused/stopped, 1 = currently measuring.
    };

#ifdef MU_WINDOWS
    using TVar = td::LINT8;
    using TVar2 = td::LINT8;
#else
    using TVar = td::LINT8;
    using TVar2 = std::chrono::high_resolution_clock::time_point;
#endif

protected:
    cnt::Array< struct TimerEntry<TVar, TVar2>, SIZE + 1> _times; ///< Array of timer entries, one per clock.
#ifdef MU_WINDOWS
    TVar liFreq; ///< QueryPerformanceFrequency value used to convert ticks to seconds on Windows.
#endif

    /// @brief Returns the current high-resolution timestamp.
    /// @return Platform-specific time snapshot.
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
    /// @brief Returns the elapsed time for the specified clock in milliseconds.
    /// @tparam T Clock identifier type satisfying the ClockID concept.
    /// @param iClock The index or enum value identifying the clock.
    /// @return Elapsed time in milliseconds.
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

    /// @brief Returns the elapsed time for clock 0 in milliseconds.
    /// @return Elapsed time in milliseconds for the default clock.
    inline double getTimeInMilliSec() const
    {
        return getTimeInMilliSec(td::UINT4(0));
    }

protected:
    /// @brief Base case for recursive cumulative millisecond sum (single clock).
    /// @tparam T Clock identifier type.
    /// @param iClock The clock whose time to return.
    /// @return Elapsed time in milliseconds for the given clock.
    template <ClockID T, typename... Args>
    inline double getCumTimeInMilliSec(T iClock) const
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        assert(clockIndex <= SIZE);
        double totalTime = getTimeInMilliSec(iClock);
        return totalTime;
    }

    /// @brief Recursive accumulation of millisecond times across multiple clocks.
    /// @tparam T Clock identifier type.
    /// @tparam Args Additional clock identifier types.
    /// @param iFirstClock The first clock in the variadic list.
    /// @param args Remaining clock identifiers.
    /// @return Sum of elapsed milliseconds for all specified clocks.
    template <ClockID T, typename... Args>
    inline double getCumTimeInMilliSec(T iFirstClock, Args... args) const
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iFirstClock);
        assert(clockIndex <= SIZE);
        double totalTime = getTimeInMilliSec(iFirstClock) + getCumTimeInMilliSec(args...);
        return totalTime;
    }

    /// @brief Base case for recursive cumulative second sum (single clock).
    /// @tparam T Clock identifier type.
    /// @param iClock The clock whose time to return.
    /// @return Elapsed time in seconds for the given clock.
    template <ClockID T, typename... Args>
    inline double getCumTimeInSec(T iClock) const
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        assert(clockIndex <= SIZE);
        double totalTime = getTimeInSec(iClock);
        return totalTime;
    }

    /// @brief Recursive accumulation of second times across multiple clocks.
    /// @tparam T Clock identifier type.
    /// @tparam Args Additional clock identifier types.
    /// @param iFirstClock The first clock in the variadic list.
    /// @param args Remaining clock identifiers.
    /// @return Sum of elapsed seconds for all specified clocks.
    template <ClockID T, typename... Args>
    inline double getCumTimeInSec(T iFirstClock, Args... args) const
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iFirstClock);
        assert(clockIndex <= SIZE);
        double totalTime = getTimeInSec(iFirstClock) + getCumTimeInSec(args...);
        return totalTime;
    }
public:
    /// @brief Returns the elapsed time for the specified clock in seconds.
    /// @tparam T Clock identifier type satisfying the ClockID concept.
    /// @param iClock The index or enum value identifying the clock.
    /// @return Elapsed time in seconds.
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

    /// @brief Returns the elapsed time for clock 0 in seconds.
    /// @return Elapsed time in seconds for the default clock.
    inline double getTimeInSec() const
    {
        return getTimeInSec(td::UINT4(0));
    }

protected:

    /// @brief Formats and writes a clock's timing summary in seconds to an output stream.
    /// @tparam TSTR Stream type supporting operator<<.
    /// @param s The output stream to write to.
    /// @param iClock Index of the clock being reported.
    /// @param pName Name label for the clock; "NA" is used if nullptr.
    /// @param totalTime Accumulated time in seconds.
    /// @param nCalls Number of times measure() was called.
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

    /// @brief Formats and writes a clock's timing summary in milliseconds to an output stream.
    /// @tparam TSTR Stream type supporting operator<<.
    /// @param s The output stream to write to.
    /// @param iClock Index of the clock being reported.
    /// @param pName Name label for the clock; "NA" is used if nullptr.
    /// @param totalTime Accumulated time in milliseconds.
    /// @param nCalls Number of times measure() was called.
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
    /// @brief Default constructor; initializes the performance counter frequency and names clock 0 "TOTAL".
    Timer()
    {
#ifdef MU_WINDOWS
        LARGE_INTEGER tmp;
        QueryPerformanceFrequency(&tmp);
        liFreq = tmp.QuadPart;
#endif
        _times[0].name = "TOTAL";
    }

    /// @brief Constructs a Timer with a name for clock 0 and optionally starts measuring immediately.
    /// @param timerName Name label assigned to clock 0.
    /// @param startMeasuring If true, measurement begins immediately on construction.
    explicit Timer(const char* timerName, bool startMeasuring = true)
    {
#ifdef MU_WINDOWS
        LARGE_INTEGER tmp;
        QueryPerformanceFrequency(&tmp);
        liFreq = tmp.QuadPart;
#endif
        setName(0, timerName, startMeasuring);
    }

    /// @brief Assigns a name to the specified clock and optionally starts measuring.
    /// @tparam T Clock identifier type.
    /// @param iClock The clock to name.
    /// @param clockName The name string to assign.
    /// @param startMeasuring If true, begins measurement immediately after naming.
    template <ClockID T>
    void setName(T iClock, const char* clockName, bool startMeasuring = false)
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        assert(clockIndex <= SIZE);
        _times[clockIndex].name = clockName;
        if (startMeasuring)
            measure(iClock);
    }

    /// @brief Pauses accumulation for the specified clock without resetting it.
    /// @tparam T Clock identifier type.
    /// @param iClock The clock to pause.
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

    /// @brief Pauses accumulation for clock 0.
    inline void pause()
    {
        pause(td::UINT4(0));
    }

    /// @brief Pauses accumulation for all clocks.
    inline void pauseAll()
    {
        for (td::UINT4 i=0; i<= SIZE; ++i)
            pause(i);
    }

    /// @brief Starts or resumes accumulation for the specified clock.
    /// @tparam T Clock identifier type.
    /// @param iClock The clock to start measuring.
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

    /// @brief Resets the accumulated time and call count for the specified clock.
    /// @tparam T Clock identifier type.
    /// @param iClock The clock to reset.
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

    /// @brief Resets clock 0 (total accumulated time and call count).
    inline void reset()
    {
        reset(td::UINT4{0});
    }

    /// @brief Starts measuring on clock 0.
    inline void measure()
    {
        measure(td::UINT4{0});
    }

    /// @brief Resets all clocks.
    inline void resetAll()
    {
        for (td::UINT4 i = 0; i <= SIZE; ++i)
            reset(i);
    }

    /// @brief Starts measuring on all clocks.
    inline void measureAll()
    {
        for (td::UINT4 i = 0; i <= SIZE; ++i)
            measure(i);
    }

    /// @brief Writes the elapsed milliseconds for the specified clock to the given stream.
    /// @tparam T Clock identifier type.
    /// @tparam TSTREAM Output stream type.
    /// @param s The output stream.
    /// @param iClock The clock to report (default: first value of T).
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

    /// @brief Writes the elapsed seconds for the specified clock to the given stream.
    /// @tparam T Clock identifier type.
    /// @tparam TSTREAM Output stream type.
    /// @param s The output stream.
    /// @param iClock The clock to report (default: first value of T).
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

    /// @brief Writes elapsed milliseconds for all clocks to the given stream.
    /// @tparam TSTREAM Output stream type.
    /// @param o The output stream.
    template <class TSTREAM>
    void showAllTimesInMillisec(TSTREAM& o) const
    {
        for (td::UINT4 i = 0; i <= SIZE; ++i)
        {
            showTimeInMillisec(o, i);
        }
    }

    /// @brief Writes elapsed seconds for all clocks to the given stream.
    /// @tparam TSTREAM Output stream type.
    /// @param o The output stream.
    template <class TSTREAM>
    void showTimesInSec(TSTREAM& o) const
    {
        for (td::UINT4 i = 0; i <= SIZE; ++i)
        {
            showTimeInSec(o, i);
        }
    }

    /// @brief Returns whether the specified clock is currently measuring.
    /// @tparam T Clock identifier type.
    /// @param iClock The clock to query (default: first value of T).
    /// @return true if the clock is actively accumulating time; false if paused or stopped.
    template <ClockID T>
    inline bool isRunning(T iClock = T{0})
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        assert(clockIndex <= SIZE);
        return (_times[clockIndex].status == 1);
    }

    /// @brief Writes the sum of elapsed milliseconds for multiple clocks to the given stream.
    /// @tparam TSTR Output stream type.
    /// @tparam T Clock identifier type of the first clock.
    /// @tparam Args Clock identifier types of additional clocks.
    /// @param s The output stream.
    /// @param lbl Label to display beside the summed time.
    /// @param firstClock The first clock to include in the sum.
    /// @param args Additional clocks to include in the sum.
    template<class TSTR, ClockID T, typename... Args>
    void showSumInMilliseconds(TSTR& s, const char* lbl, T firstClock, Args... args)
    {
        double totalTime = getTimeInMilliSec(firstClock) + getCumTimeInMilliSec(args...);
        printTimeInMillisec(s, 0, lbl, totalTime, 0);
    }

    /// @brief Writes the sum of elapsed seconds for multiple clocks to the given stream.
    /// @tparam TSTR Output stream type.
    /// @tparam T Clock identifier type of the first clock.
    /// @tparam Args Clock identifier types of additional clocks.
    /// @param s The output stream.
    /// @param lbl Label to display beside the summed time.
    /// @param firstClock The first clock to include in the sum.
    /// @param args Additional clocks to include in the sum.
    template<class TSTR, ClockID T, typename... Args>
    void showSumInSec(TSTR& s, const char* lbl, T firstClock, Args... args)
    {
        double totalTime = getTimeInSec(firstClock) + getCumTimeInSec(args...);
        printTimeInSec(s, 0, lbl, totalTime, 0);
    }

    /// @brief Writes the delta (clock 0 minus all other clocks) in milliseconds to the stream.
    /// @tparam TSTR Output stream type.
    /// @param s The output stream.
    /// @param lbl Label displayed beside the delta value.
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

    /// @brief Writes the delta (clock 0 minus all other clocks) in seconds to the stream.
    /// @tparam TSTR Output stream type.
    /// @param s The output stream.
    /// @param lbl Label displayed beside the delta value.
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
    /// @brief Appends timing info for the specified clock in seconds to a mutable string.
    /// @tparam T Clock identifier type.
    /// @tparam TMUTABLESTR Mutable string type supporting appendFormat.
    /// @param mStr The string to append to.
    /// @param iClock The clock to report (default: first value of T).
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

    /// @brief Appends timing info for the specified clock in milliseconds to a mutable string.
    /// @tparam T Clock identifier type.
    /// @tparam TMUTABLESTR Mutable string type supporting appendFormat.
    /// @param mStr The string to append to.
    /// @param iClock The clock to report (default: first value of T).
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

    /// @brief Dispatches timing info collection to the appropriate unit-specific helper.
    /// @tparam T Clock identifier type.
    /// @tparam TMUTABLESTR Mutable string type.
    /// @param mStr The string to append to.
    /// @param iClock The clock to report.
    /// @param units The time unit (MilliSeconds or Seconds).
    template<ClockID T, class TMUTABLESTR>
    void getInfo(TMUTABLESTR& mStr, T iClock, TimerUnits units) const
    {
        if (units == TimerUnits::MilliSeconds)
            getInfoInMilliSeconds(mStr, iClock);
        else
            getInfoInSeconds(mStr, iClock);
    }

public:

    /// @brief Returns the total number of clocks managed by this Timer instance.
    /// @return SIZE + 1.
    inline constexpr td::UINT4 size() const
    {
        return SIZE+1;
    }

    /// @brief Returns the number of times measure() has been called for the specified clock.
    /// @tparam T Clock identifier type.
    /// @param iClock The clock to query (default: first value of T).
    /// @return Call count for the specified clock.
    template <ClockID T>
    td::UINT4 getNumberOfCalls(T iClock = T{0}) const
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        return _times[clockIndex].nCalls;
    }

    /// @brief Reports timing information for all clocks via a callback function.
    /// @param callBackFn Callback invoked once for each line of the timing report.
    /// @param units The time unit to use in the report (MilliSeconds or Seconds).
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

    /// @brief Reports timing information every N calls for the specified clock.
    /// @tparam T Clock identifier type.
    /// @param callBackFn Callback invoked for each line of the report.
    /// @param step Report interval in number of calls.
    /// @param iClock The clock whose call count is checked against the step.
    /// @param units The time unit to use in the report.
    template <ClockID T>
    inline void reportTimes(TimerCallBack callBackFn, td::UINT4 step, T iClock, TimerUnits units) const
    {
        td::UINT4 nCalls = getNumberOfCalls(iClock);
        if (nCalls % step == 0)
        {
            reportTimes(callBackFn, units);
        }
    }

    /// @brief Formats the elapsed time for the specified clock into a mutable string.
    /// @tparam T Clock identifier type.
    /// @param mStr The mutable string to receive the formatted result.
    /// @param iClock The clock to report.
    template <ClockID T>
    inline void getTime(td::MutableString& mStr, T iClock)
    {
        const td::UINT4 clockIndex = static_cast<td::UINT4>(iClock);
        if (mStr.capacity() < 128)
            mStr.reserve(128);

        mStr.reset();
        double timeInSec = getTimeInSec(iClock);
        if (timeInSec >= 1.0)
            mStr.appendFormat("%s: %.6f seconds", _times[clockIndex].name, timeInSec);
        else
        {
            double timeInMilliSec = getTimeInMilliSec(iClock);
            mStr.appendFormat("%s: %.3f milliseconds", _times[clockIndex].name, timeInMilliSec);
        }
    }

};

} //namespace mu
