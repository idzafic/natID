// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DebugConsoleLog.h
 * @brief Thread-safe debug console logging with colored output and log levels.
 */
#pragma once
#include <mu/muLib.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <mutex>

#ifdef MU_WINDOWS
#include <windows.h>
#endif
#include <td/Types.h>

namespace mu
{
#ifdef MU_DEBUG
class DebugConsoleHelper;
#endif
    // Thread-safe logging with timestamp and log levels
/// @brief Thread-safe console logger with ANSI color support and timestamp output.
class MAINUTILS_API DebugConsoleLog
{
#ifdef MU_DEBUG
    friend class DebugConsoleHelper;
#endif
private:
#ifdef MU_DEBUG
    static std::mutex logMutex;   ///< Mutex protecting concurrent log writes
    static char* buffer;          ///< Shared format buffer for printf-style messages
    static int bufferSize;        ///< Size of the format buffer in bytes
    static bool colorSupport;     ///< True if the terminal supports ANSI color codes
#endif

    // Enable ANSI color codes on Windows
    /// @brief Enables ANSI virtual terminal processing on Windows consoles.
    static void enableWindowsColors() {
#ifdef MU_DEBUG
#ifdef MU_WINDOWS
        colorSupport = false;
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE)
        {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode))
            {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
                colorSupport = true;
            }
        }
#else
        colorSupport = true;
#endif
#endif
    }

    /// @brief Returns the current wall-clock time as a formatted string (HH:MM:SS.mmm).
    /// @return Timestamp string.
    static std::string getTimestamp()
    {
#ifdef MU_DEBUG
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_time), "%H:%M:%S")
            << '.' << std::setfill('0') << std::setw(3) << now_ms.count();
        return ss.str();
#else
        return std::string();
#endif
    }

    /// @brief Internal helper that writes a prefixed, optionally colored log line to stdout.
    /// @param level Log level label (e.g., "ERROR", "INFO").
    /// @param color ANSI color escape sequence, or nullptr for no color.
    /// @param message The message text to print.
    static void printLog(const char* level, const char* color, const std::string& message) {
        #ifdef MU_DEBUG

        if (!colorSupport)
            enableWindowsColors();

        if (color)
            std::cout << "[" << getTimestamp() << "] "
                    << color << "[" << level << "]\033[0m " << message << std::endl;
        else
            std::cout << "[" << getTimestamp() << "] "
                    << "[" << level <<"] " << message << std::endl;
        #endif
    }

public:
    // Basic printf-like function
    /// @brief Logs a plain message using printf-style formatting.
    /// @param format Printf format string.
    /// @param args Format arguments.
    template<typename... Args>
    static void log(const char* format, Args... args)
    {
        #ifdef MU_DEBUG
        if (buffer)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            int size = SNPRINTF(buffer, bufferSize, _TRUNCATE, format, args...);
            if (size > 0 && size < bufferSize)
                printLog("LOG", nullptr, buffer);
        }
        #endif
    }

    // Error logging (red)
    /// @brief Logs an error message in red.
    /// @param format Printf format string.
    /// @param args Format arguments.
    template<typename... Args>
    static void error(const char* format, Args... args)
    {
        #ifdef MU_DEBUG
        if (buffer)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            int size = SNPRINTF(buffer, bufferSize, _TRUNCATE, format, args...);
            if (size > 0 && size < bufferSize)
                printLog("ERROR", "\033[1;31m", buffer);
        }

        #endif
    }

    // Warning logging (yellow)
    /// @brief Logs a warning message in yellow.
    /// @param format Printf format string.
    /// @param args Format arguments.
    template<typename... Args>
    static void warning(const char* format, Args... args)
    {
        #ifdef MU_DEBUG
        if (buffer)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            int size = SNPRINTF(buffer, bufferSize, _TRUNCATE, format, args...);
            if (size > 0 && size < bufferSize)
                printLog("WARNING", "\033[1;33m", buffer);
        }

        #endif
    }

    /// @brief Logs an informational message in blue.
    /// @param format Printf format string.
    /// @param args Format arguments.
    template<typename... Args>
    static void info(const char* format, Args... args)
    {
#ifdef MU_DEBUG
        if (buffer)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            int size = SNPRINTF(buffer, bufferSize, _TRUNCATE, format, args...);
            if (size > 0 && size < bufferSize)
                printLog("INFO", "\033[1;34m", buffer);
        }

#endif
    }

    // Success logging (green)
    /// @brief Logs a success message in green.
    /// @param format Printf format string.
    /// @param args Format arguments.
    template<typename... Args>
    static void success(const char* format, Args... args) {
        #ifdef MU_DEBUG
        if (buffer)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            int size = SNPRINTF(buffer, bufferSize, _TRUNCATE, format, args...);
            if (size > 0 && size < bufferSize)
                printLog("OK", "\033[1;32m", buffer);
        }
        #endif
    }

    // Debug logging (cyan)
    /// @brief Logs a debug message in cyan.
    /// @param format Printf format string.
    /// @param args Format arguments.
    template<typename... Args>
    static void debug(const char* format, Args... args)
    {
        #ifdef MU_DEBUG
        if (buffer)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            int size = SNPRINTF(buffer, bufferSize, _TRUNCATE, format, args...);
            if (size > 0 && size < bufferSize)
                printLog("DEBUG", "\033[1;36m", buffer);
        }
        #endif
    }

    // Vulkan-specific logging
    /// @brief Logs a Vulkan-related message in magenta.
    /// @param format Printf format string.
    /// @param args Format arguments.
    template<typename... Args>
    static void vkLog(const char* format, Args... args)
    {
        #ifdef MU_DEBUG
        if (buffer)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            int size = SNPRINTF(buffer, bufferSize, _TRUNCATE, format, args...);
            if (size > 0 && size < bufferSize)
                printLog("VULKAN", "\033[1;35m", buffer);
        }
        #endif
    }

    // GLX-specific logging
    /// @brief Logs a GLX-related message in magenta.
    /// @param format Printf format string.
    /// @param args Format arguments.
    template<typename... Args>
    static void glxLog(const char* format, Args... args)
    {
        #ifdef MU_DEBUG
        if (buffer)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            int size = SNPRINTF(buffer, bufferSize, _TRUNCATE, format, args...);
            if (size > 0 && size < bufferSize)
                printLog("GLX", "\033[1;35m", buffer);
        }
        #endif
    }

    // Simple cout-like streaming interface
    /// @brief Stream-based log helper; flushes the accumulated message on destruction.
    class LogStream
    {
    private:
#ifdef MU_DEBUG
        std::stringstream ss; ///< Internal string accumulator
        const char* level;    ///< Log level label
        const char* color;    ///< ANSI color code, or nullptr
#endif

    public:
        /// @brief Constructs a LogStream with the given level and color.
        /// @param lvl Log level label string.
        /// @param col ANSI color escape sequence, or nullptr.
        LogStream(const char* lvl, const char* col)
#ifdef MU_DEBUG
            : level(lvl), color(col)
#endif
        {}

        /// @brief Destructor; flushes the accumulated stream content to the console.
        ~LogStream()
        {
            #ifdef MU_DEBUG
            if (!colorSupport)
                enableWindowsColors();

            std::lock_guard<std::mutex> lock(logMutex);
            if (color)
                std::cout << "[" << getTimestamp() << "] "
                        << color << "[" << level << "]\033[0m " << ss.str() << std::endl;
            else
                std::cout << "[" << getTimestamp() << "] " << "[" << level << "] " << ss.str() << std::endl;

            #endif
        }

        /// @brief Streams a value into the log message buffer.
        /// @param value Value to append.
        /// @return Reference to this LogStream.
        template<typename T>
        LogStream& operator<<(const T& value)
        {
        #ifdef MU_DEBUG
            ss << value;
        #endif
            return *this;
        }
    };

    /// @brief Returns a LogStream for plain log messages.
    /// @return LogStream instance.
    static LogStream log() { return LogStream("LOG", nullptr); }
    /// @brief Returns a LogStream for informational messages (blue).
    /// @return LogStream instance.
    static LogStream info() { return LogStream("INFO", "\033[1;34m"); }
    /// @brief Returns a LogStream for error messages (red).
    /// @return LogStream instance.
    static LogStream error() { return LogStream("ERROR", "\033[1;31m"); }
    /// @brief Returns a LogStream for warning messages (yellow).
    /// @return LogStream instance.
    static LogStream warning() { return LogStream("WARNING", "\033[1;33m"); }
    /// @brief Returns a LogStream for success/ok messages (green).
    /// @return LogStream instance.
    static LogStream ok() { return LogStream("OK", "\033[1;32m"); }
    /// @brief Returns a LogStream for debug messages (cyan).
    /// @return LogStream instance.
    static LogStream debug() { return LogStream("DEBUG", "\033[1;36m"); }
    /// @brief Returns a LogStream for OpenGL messages (magenta).
    /// @return LogStream instance.
    static LogStream openGL() { return LogStream("OPENGL", "\033[1;35m"); }
    /// @brief Returns a LogStream for Vulkan messages (magenta).
    /// @return LogStream instance.
    static LogStream vkLog() { return LogStream("VULKAN", "\033[1;35m"); }
    /// @brief Returns a LogStream for GLX messages (magenta).
    /// @return LogStream instance.
    static LogStream glxLog() { return LogStream("GLX", "\033[1;35m"); }
};

/// @brief Convenience function that logs a plain C string at LOG level.
/// @param txt The text to log.
inline void showDebugConsoleLog(const char* txt)
{
    #ifdef MU_DEBUG
    DebugConsoleLog::log("%s", txt);
    #endif
}

// Convenience macros (optional)
#define MU_DEBUG_LOG(fmt, ...) mu::DebugConsoleLog::log(fmt, ##__VA_ARGS__)
#define MU_DEBUG_LOG_ERROR(fmt, ...) mu::DebugConsoleLog::error(fmt, ##__VA_ARGS__)
#define MU_DEBUG_LOG_INFO(fmt, ...) mu::DebugConsoleLog::info(fmt, ##__VA_ARGS__)
#define MU_DEBUG_LOG_WARNING(fmt, ...) mu::DebugConsoleLog::warning(fmt, ##__VA_ARGS__)
#define MU_DEBUG_LOG_SUCCESS(fmt, ...) mu::DebugConsoleLog::success(fmt, ##__VA_ARGS__)
#define MU_DEBUG_LOG_DEBUG(fmt, ...) mu::DebugConsoleLog::debug(fmt, ##__VA_ARGS__)
#define MU_DEBUG_LOG_VK(fmt, ...) mu::DebugConsoleLog::vkLog(fmt, ##__VA_ARGS__)
#define MU_DEBUG_LOG_GLX(fmt, ...) mu::DebugConsoleLog::glxLog(fmt, ##__VA_ARGS__)


} // namespace mu
