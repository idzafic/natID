// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Console.h
    @brief Thread-safe console output class with color support, log levels, and a streaming interface. */
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
/// @brief Thread-safe console logger with color-coded log levels and an optional timestamp prefix.
class MAINUTILS_API Console
{
    friend class ConsoleHelper;
public:
    /// @brief Enumerates the visual color categories available for console output.
    enum class Color {Normal, ///< Default terminal color.
                      Log,    ///< Plain log output color.
                      Debug,  ///< Debug message color (cyan).
                      Info,   ///< Informational message color (blue).
                      Warning,///< Warning message color (yellow).
                      Error,  ///< Error message color (red).
                      Ok      ///< Success message color (green).
    };
private:
    static std::mutex logMutex; ///< Mutex protecting concurrent console writes.
    static bool colorSupport; ///< Whether the terminal supports ANSI color codes.
    static char* buffer; ///< Shared formatting buffer for printf-style messages.
    static int bufferSize; ///< Size of the shared formatting buffer in bytes.
    static bool showTimeStamp; ///< Whether each log line is prefixed with a timestamp.
private:

    /// @brief Returns the current time as a formatted timestamp string.
    /// @return A string representing the current time (HH:MM:SS.mmm).
    static std::string getTimestamp();

    /// @brief Writes a formatted log entry to standard output with an optional color prefix.
    /// @param level Null-terminated label string (e.g., "INFO", "ERROR").
    /// @param color ANSI escape sequence for the color, or nullptr for no color.
    /// @param message The message text to print.
    static void printLog(const char* level, const char* color, const std::string& message);

public:

    /// @brief Initializes the console, detects color support, and optionally enables timestamps.
    /// @param showTimeStamp If true, every log line is preceded by a timestamp.
    /// @return true on successful initialization.
    static bool init(bool showTimeStamp);

    /// @brief Prints a plain string message to the console with the specified color.
    /// @param message The text to display.
    /// @param color The Color category determining the ANSI color used.
    static void print(const td::String& message, Color color);

    //// Basic printf-like function
    /// @brief Logs a formatted message at the default LOG level.
    /// @tparam Args Variadic argument types for the format string.
    /// @param format printf-style format string.
    /// @param args Arguments matching the format specifiers.
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
    /// @brief Logs a formatted informational message in blue.
    /// @tparam Args Variadic argument types for the format string.
    /// @param format printf-style format string.
    /// @param args Arguments matching the format specifiers.
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
    /// @brief Logs a formatted warning message in yellow.
    /// @tparam Args Variadic argument types for the format string.
    /// @param format printf-style format string.
    /// @param args Arguments matching the format specifiers.
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
    /// @brief Logs a formatted error message in red.
    /// @tparam Args Variadic argument types for the format string.
    /// @param format printf-style format string.
    /// @param args Arguments matching the format specifiers.
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
    /// @brief Logs a formatted success message in green.
    /// @tparam Args Variadic argument types for the format string.
    /// @param format printf-style format string.
    /// @param args Arguments matching the format specifiers.
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
    /// @brief Logs a formatted debug message in cyan.
    /// @tparam Args Variadic argument types for the format string.
    /// @param format printf-style format string.
    /// @param args Arguments matching the format specifiers.
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
    /// @brief Provides a cout-style streaming interface that emits a colored log line on destruction.
    class ColorStream
    {
    private:
        std::stringstream ss; ///< Internal string buffer accumulating the streamed message.
        const char* level; ///< Log level label (e.g., "INFO").
        const char* color; ///< ANSI escape sequence for the output color, or nullptr.

    public:
        /// @brief Constructs a ColorStream with the given level label and ANSI color code.
        /// @param lvl Log level label string.
        /// @param col ANSI color escape sequence, or nullptr for no color.
        ColorStream(const char* lvl, const char* col)
            : level(lvl), color(col)
        {}

        /// @brief Destructor; flushes the accumulated message to standard output with the configured color.
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

        /// @brief Streams a value into the internal buffer.
        /// @tparam T Type of the value to stream.
        /// @param value The value to append to the message buffer.
        /// @return Reference to this ColorStream for chaining.
        template<typename T>
        ColorStream& operator<<(const T& value)
        {
            ss << value;
            return *this;
        }
    };

    /// @brief Creates a ColorStream for plain LOG-level output.
    /// @return A ColorStream configured with the LOG level and no color.
    static ColorStream log() { return ColorStream("LOG", nullptr); }
    /// @brief Creates a ColorStream for INFO-level output (blue).
    /// @return A ColorStream configured with the INFO level and blue color.
    static ColorStream info() { return ColorStream("INFO", "\033[1;34m"); }
    /// @brief Creates a ColorStream for ERROR-level output (red).
    /// @return A ColorStream configured with the ERROR level and red color.
    static ColorStream error() { return ColorStream("ERROR", "\033[1;31m"); }
    /// @brief Creates a ColorStream for WARNING-level output (yellow).
    /// @return A ColorStream configured with the WARNING level and yellow color.
    static ColorStream warning() { return ColorStream("WARNING", "\033[1;33m"); }
    /// @brief Creates a ColorStream for OK/success-level output (green).
    /// @return A ColorStream configured with the OK level and green color.
    static ColorStream ok() { return ColorStream("OK", "\033[1;32m"); }
    /// @brief Creates a ColorStream for DEBUG-level output (cyan).
    /// @return A ColorStream configured with the DEBUG level and cyan color.
    static ColorStream debug() { return ColorStream("DEBUG", "\033[1;36m"); }
    /// @brief Creates a ColorStream for OpenGL-related output (magenta).
    /// @return A ColorStream configured with the OPENGL level and magenta color.
    static ColorStream openGL() { return ColorStream("OPENGL", "\033[1;35m"); }
    /// @brief Creates a ColorStream for Vulkan-related output (magenta).
    /// @return A ColorStream configured with the VULKAN level and magenta color.
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
