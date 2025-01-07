// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <chrono>

namespace td
{

template <bool RestartTimer = false>
class Timer
{
    std::chrono::high_resolution_clock::time_point _start;
public:
    Timer()
    : _start(std::chrono::high_resolution_clock::now())
    {}
    
    void start()
    {
        _start = std::chrono::high_resolution_clock::now();
    }
    
    double getDurationInSeconds()
    {
        // Get the current time point again
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

        // Calculate the time difference
        std::chrono::duration<double> duration = end - _start;
        if (RestartTimer)
            _start = end;

        // Get the time difference in seconds as a double
        double secondsPassed = duration.count();
        
        return secondsPassed;
    }
    
    double getDurationInMilliseconds()
    {
        // Get the current time point again
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

        // Calculate the time difference in nanoseconds
//        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - _start);
        auto duration = end - _start;
        if (RestartTimer)
            _start = end;

        // Convert the duration to milliseconds as a double value
        double milliseconds = std::chrono::duration<double, std::milli>(duration).count();
        
        return milliseconds;
    }
    
    double getDurationInMicroSeconds()
    {
        // Get the current time point again
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

        // Calculate the time difference in nanoseconds
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - _start);
        
        if (RestartTimer)
            _start = end;

        // Convert the duration to milliseconds as a double value
        double microseconds = std::chrono::duration<double, std::micro>(duration).count();
        
        return microseconds;
    }
    
};

} //namespace td


