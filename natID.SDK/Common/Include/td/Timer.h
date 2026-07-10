// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Timer.h
    @brief Provides a high-resolution elapsed-time measurement utility. */
#pragma once
#include <chrono>

namespace td
{

/// @brief High-resolution elapsed-time timer using std::chrono.
/// @tparam RestartTimer If true, the start point is reset to the last measurement point after each query.
template <bool RestartTimer = false>
class Timer
{
    std::chrono::high_resolution_clock::time_point _start; ///< Time point when the timer was started.
    std::chrono::high_resolution_clock::time_point _end;   ///< Time point of the last stop or query.
    bool _running = false; ///< True while the timer has been started but not stopped.
public:
    /// @brief Constructs a Timer and starts it at construction time.
    Timer()
    : _start(std::chrono::high_resolution_clock::now())
    , _end(_start)
    {}

    /// @brief Starts (or restarts) the timer.
    void start()
    {
        _start = std::chrono::high_resolution_clock::now();
        _end = _start;
        _running = true;
    }

    /// @brief Stops the timer, recording the stop time.
    void stop()
    {
        _end = std::chrono::high_resolution_clock::now();
        _running = false;
    }

    /// @brief Returns the elapsed time in seconds.
    /// @return Elapsed duration as a double in seconds.
    double getDurationInSeconds()
    {
        // Get the current time point again
        if (_running)
            _end = std::chrono::high_resolution_clock::now();

        // Calculate the time difference
        std::chrono::duration<double> duration = _end - _start;
        if (RestartTimer)
            _start = _end;

        // Get the time difference in seconds as a double
        double secondsPassed = duration.count();

        return secondsPassed;
    }

    /// @brief Returns the elapsed time in milliseconds.
    /// @return Elapsed duration as a double in milliseconds.
    double getDurationInMilliseconds()
    {
        // Get the current time point again
        if (_running)
            _end = std::chrono::high_resolution_clock::now();

        // Calculate the time difference in nanoseconds
//        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - _start);
        auto duration = _end - _start;
        if (RestartTimer)
            _start = _end;

        // Convert the duration to milliseconds as a double value
        double milliseconds = std::chrono::duration<double, std::milli>(duration).count();

        return milliseconds;
    }

    /// @brief Returns the elapsed time in microseconds.
    /// @return Elapsed duration as a double in microseconds.
    double getDurationInMicroSeconds()
    {
        // Get the current time point again
        if (_running)
            _end = std::chrono::high_resolution_clock::now();

        // Calculate the time difference in nanoseconds
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(_end - _start);

        if (RestartTimer)
            _start = _end;

        // Convert the duration to milliseconds as a double value
        double microseconds = std::chrono::duration<double, std::micro>(duration).count();

        return microseconds;
    }

};

} //namespace td


