// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <mu/mu.h>
#include <mu/muLib.h>
#include <td/Types.h>
#include <string>
#include <mutex>
#include <iostream>
#include <sstream>
#include <td/String.h>

namespace mu
{

class ConsoleHelper;
// Thread-safe logging with timestamp and log levels
class MAINUTILS_API Console
{
    friend class ConsoleHelper;
public:
    enum class Color {Normal, Log, Debug, Info, Warning, Error, Ok};
private:
    static std::mutex logMutex;
    static bool colorSupport;
    static char* buffer;
    static int bufferSize;
    static bool showTimeStamp;
private:
    
    static std::string getTimestamp();

    static void printLog(const char* level, const char* color, const std::string& message);
    
public:
    
    static bool init(bool showTimeStamp);
    
    static void print(const td::String& message, Color color);
    
    //// Basic printf-like function
    template<typename... Args>
    static void log(const char* format, Args... args)
    {
        if (buffer)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            int size = SNPRINTF(buffer, bufferSize, _TRUNCATE, format, args...);
            if (size > 0 && size < bufferSize)
                printLog("LOG", nullptr, buffer);
        }
    }

    // Info logging (blue)
    template<typename... Args>
    static void info(const char* format, Args... args)
    {
        if (buffer)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            int size = SNPRINTF(buffer, bufferSize, _TRUNCATE, format, args...);
            if (size > 0 && size < bufferSize)
                printLog("INFO", "\033[1;34m", buffer);
        }
    }
    
    // Warning logging (yellow)
    template<typename... Args>
    static void warning(const char* format, Args... args)
    {
        if (buffer)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            int size = SNPRINTF(buffer, bufferSize, _TRUNCATE, format, args...);
            if (size > 0 && size < bufferSize)
                printLog("WARNING", "\033[1;33m", buffer);
        }
    }
    
    // Error logging (red)
    template<typename... Args>
    static void error(const char* format, Args... args)
    {
        if (buffer)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            int size = SNPRINTF(buffer, bufferSize, _TRUNCATE, format, args...);
            if (size > 0 && size < bufferSize)
                printLog("ERROR", "\033[1;31m", buffer);
        }
    }
    
    
    // Success logging (green)
    template<typename... Args>
    static void success(const char* format, Args... args)
    {
        if (buffer)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            int size = SNPRINTF(buffer, bufferSize, _TRUNCATE, format, args...);
            if (size > 0 && size < bufferSize)
                printLog("OK", "\033[1;32m", buffer);
        }
    }
    
    // Debug logging (cyan)
    template<typename... Args>
    static void debug(const char* format, Args... args)
    {
        if (buffer)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            int size = SNPRINTF(buffer, bufferSize, _TRUNCATE, format, args...);
            if (size > 0 && size < bufferSize)
                printLog("DEBUG", "\033[1;36m", buffer);
        }
       
    }
    
    // Simple cout-like streaming interface
    class ColorStream
    {
    private:
        std::stringstream ss;
        const char* level;
        const char* color;
        
    public:
        ColorStream(const char* lvl, const char* col)
            : level(lvl), color(col)
        {}
        
        ~ColorStream()
        {
            std::lock_guard<std::mutex> lock(logMutex);
            
            if (showTimeStamp)
            {
                std::cout << "[" << getTimestamp() << "] ";
            }
            
            // Only use colors if both color is specified AND colorSupport is enabled
            if (color && colorSupport)
            {
                std::cout << color << "[" << level << "]\033[0m " << ss.str() << std::endl;
            }
            else
            {
                std::cout << "[" << level << "] " << ss.str() << std::endl;
            }
        }
        
        template<typename T>
        ColorStream& operator<<(const T& value)
        {
            ss << value;
            return *this;
        }
    };
    
    static ColorStream log() { return ColorStream("LOG", nullptr); }
    static ColorStream info() { return ColorStream("INFO", "\033[1;34m"); }
    static ColorStream error() { return ColorStream("ERROR", "\033[1;31m"); }
    static ColorStream warning() { return ColorStream("WARNING", "\033[1;33m"); }
    static ColorStream ok() { return ColorStream("OK", "\033[1;32m"); }
    static ColorStream debug() { return ColorStream("DEBUG", "\033[1;36m"); }
    static ColorStream openGL() { return ColorStream("OPENGL", "\033[1;35m"); }
    static ColorStream vkLog() { return ColorStream("VULKAN", "\033[1;35m"); }
};

// Convenience macros (optional)
#define MU_CONSOLE_PRINT(fmt, ...) mu::Console::log(fmt, ##__VA_ARGS__)
#define MU_CONSOLE_INFO(fmt, ...) mu::Console::info(fmt, ##__VA_ARGS__)
#define MU_CONSOLE_ERROR(fmt, ...) mu::Console::error(fmt, ##__VA_ARGS__)
#define MU_CONSOLE_WARNING(fmt, ...) mu::Console::warning(fmt, ##__VA_ARGS__)
#define MU_CONSOLE_SUCCESS(fmt, ...) mu::Console::success(fmt, ##__VA_ARGS__)
#define MU_CONSOLE_DEBUG(fmt, ...) mu::Console::debug(fmt, ##__VA_ARGS__)
#define MU_CONSOLE_VK(fmt, ...) mu::Console::vkLog(fmt, ##__VA_ARGS__)
#define MU_CONSOLE_GLX(fmt, ...) mu::Console::glxLog(fmt, ##__VA_ARGS__)
    
} // namespace mu
