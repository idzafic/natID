#pragma once
#include <chrono>

template<class Cont>
class Stopwatch_
{
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    TimePoint _lastTimePoint;
    bool _running = false;
public:

    void start()
    {
        _lastTimePoint = Clock::now();
        _running = true;
    }

    Cont elapsed()
    {
        if (!_running)
            start();

        TimePoint end = Clock::now();
        auto duration = end - _lastTimePoint;
        _lastTimePoint = end;

        return std::chrono::duration<Cont, std::milli>(duration).count();
    }
};

using Stopwatch = Stopwatch_<float>;
