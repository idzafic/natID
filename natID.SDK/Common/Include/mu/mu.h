// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file mu.h
    @brief Core mu-module API: locale utilities, system helpers, logging, and global accessors. */
#pragma once
#include <mu/muLib.h>
#include <mu/Machine.h>
#include <td/BaseDateTime.h>
#include <fstream>
#include <mu/ILoggerBase.h>
#include <cstdarg>
#include <filesystem>
#include <locale.h>
#include <stdlib.h>

#ifdef MU_WINDOWS

typedef _locale_t my_locale_t;

namespace no
{
	namespace sock
	{
		class Init;
	}
}
#else
typedef locale_t my_locale_t;
#endif


namespace mem
{
class IBufferAllocator;
}

namespace mu
{

class Regionals;
class ColorNames;
class IAppSettings;

template <class TSTR>
class TxtOutFile;

//locale
MAINUTILS_API extern my_locale_t __c_locale; ///< C-locale handle used for locale-independent number formatting.

/// @brief Converts a C-string to a double using the C locale.
/// @param pStr Null-terminated string to convert.
/// @return Parsed double value, or 0.0 on failure.
MAINUTILS_API double toDouble(const char* pStr);

/// @brief Initialises the C locale handle; must be called once at application startup.
MAINUTILS_API void initCLocale(void);

/// @brief Frees the C locale handle allocated by initCLocale.
MAINUTILS_API void freeCLocale(void);


/// @brief Returns the Regionals instance associated with the main application thread.
/// @return Pointer to the main-thread Regionals object.
MAINUTILS_API Regionals* getMainThreadRegionals();

/// @brief Returns the global application settings object.
/// @return Pointer to the IAppSettings implementation.
MAINUTILS_API IAppSettings* getAppSettings();

/// @brief Retrieves the home directory path of the current user.
/// @param homePath Output parameter set to the resolved home path.
MAINUTILS_API void getHomePath(std::filesystem::path& homePath);

/// @brief Returns the home directory path of the current user.
/// @return Filesystem path to the user's home directory.
MAINUTILS_API std::filesystem::path getHomePath();

/// @brief Returns the operating system process ID of the current process.
/// @return Process identifier.
MAINUTILS_API size_t getProcesID();

#ifdef MU_DEBUG
/// @brief Creates a NatID debug log file at the specified path.
/// @param natIDfbgLogFileName Path to the log file as a C-string.
/// @return true on success, false on failure.
MAINUTILS_API bool createNatIDDbgLogFile(const char* natIDfbgLogFileName);

/// @brief Creates a NatID debug log file at the specified filesystem path.
/// @param logFNPath Filesystem path to the log file.
/// @return true on success, false on failure.
MAINUTILS_API bool createNatIDDbgLogFile(const std::filesystem::path& logFNPath);

/// @brief Initialises the debug console output.
MAINUTILS_API void initDebugConsole();

/// @brief Releases debug console resources.
MAINUTILS_API void cleanDebugConsole();
#endif

/// @brief Initialises the standard console output, optionally prefixing each line with a timestamp.
/// @param showTimeStamp If true, each log line is prefixed with the current time.
MAINUTILS_API void initConsole(bool showTimeStamp);

/// @brief Releases console output resources.
MAINUTILS_API void cleanConsole();

/// @brief Retrieves the current working directory of the application.
/// @param workingPath Output parameter set to the working directory path.
MAINUTILS_API void getWorkingFolderPath(std::filesystem::path& workingPath);

extern MAINUTILS_API int TraceLevel; ///< Global verbosity level controlling trace output.

/// @brief Returns a reference to the global text tracer output file.
/// @return Reference to the tracer TxtOutFile instance.
MAINUTILS_API TxtOutFile<std::ofstream>& getTracer();

/// @brief Returns a pointer to the global text tracer output file, or nullptr if not initialised.
/// @return Pointer to the tracer TxtOutFile instance, or nullptr.
MAINUTILS_API TxtOutFile<std::ofstream>* getTracerPtr();

/// @brief Fills bd with the current local date.
/// @param bd Output parameter receiving the current local date.
MAINUTILS_API void getCurrentLocalDate(td::BaseDate& bd);

/// @brief Fills bt with the current local time.
/// @param bt Output parameter receiving the current local time.
MAINUTILS_API void getCurrentLocalTime(td::BaseTime& bt);

/// @brief Retrieves the current date and time, in local or UTC representation.
/// @param bd  Output parameter receiving the date component.
/// @param bt  Output parameter receiving the time component.
/// @param utc If true, the returned values are in UTC; otherwise they are local.
MAINUTILS_API void getCurrentDateTime(td::BaseDate& bd, td::BaseTime& bt, bool utc);

/// @brief Converts the given date and time between local and UTC representations.
/// @param bd    Date component to convert (modified in place).
/// @param bt    Time component to convert (modified in place).
/// @param toUTC If true, converts from local to UTC; if false, converts from UTC to local.
MAINUTILS_API void convertTime(td::BaseDate& bd, td::BaseTime& bt, bool toUTC);

/// @brief Generates a 16-byte GUID and stores it in the provided buffer.
/// @param guid Output buffer of exactly 16 bytes that receives the generated GUID.
MAINUTILS_API void getGUID(char guid[16]);

/// @brief Computes a 64-bit hash of the given key string.
/// @param strKey Pointer to the key data.
/// @param keyLen Length of the key in bytes.
/// @return 64-bit hash value.
MAINUTILS_API td::LUINT8 calcHash(const char* strKey, int keyLen);

/// @brief Looks up a translation string by file hash and key string.
/// @param hashFile Hash identifying the translation file.
/// @param strKey   Null-terminated key string to look up.
/// @param keyLen   Output parameter receiving the length of the returned string.
/// @return Pointer to the translated C-string.
MAINUTILS_API const char* trc(td::UINT4 hashFile, const char* strKey, size_t& keyLen);

/// @brief Returns a const pointer to the global logger (may be nullptr if not initialised).
/// @return Const pointer to the ILoggerBase implementation.
MAINUTILS_API const mu::ILoggerBase* getLoggerC();

/// @brief Returns a mutable pointer to the global logger (may be nullptr if not initialised).
/// @return Pointer to the ILoggerBase implementation.
MAINUTILS_API mu::ILoggerBase* getLogger();

/// @brief Returns the buffer allocator associated with the main application thread.
/// @return Pointer to the main-thread IBufferAllocator.
MAINUTILS_API mem::IBufferAllocator* getMainThreadBufferAllocator();

//MAINUTILS_API void genRandomString(char* pBuff, size_t strLen);
#ifdef MU_DEBUG
#ifdef MU_MACOS

/// @brief Internal macOS implementation that writes a formatted debug log entry.
/// @param format  printf-style format string.
/// @param argptr  Variable argument list matching the format string.
MAINUTILS_API void __impDbgLog(const char* format, va_list& argptr);

/// @brief Internal macOS implementation that writes a plain debug log message.
/// @param msg Null-terminated message string to log.
MAINUTILS_API void __impDbgLog(const char* msg);

/// @brief Returns true if debug log output should also be written to a file.
/// @return true if file logging is enabled.
MAINUTILS_API bool __logInFileToo();

/// @brief Appends a log string to the debug log file.
/// @param strLog Null-terminated string to append.
MAINUTILS_API void __showLogInFile(const char* strLog);

#else
/// @brief Internal implementation that writes a plain debug log message (non-macOS).
/// @param msg Null-terminated message string to log.
MAINUTILS_API void __impDbgLog(const char* msg);
#endif
#endif //MU_DEBUG
//MAINUTILS_API void getSystemInfo();

/// @brief Emits a formatted debug log message; compiled away in release builds.
/// @param format printf-style format string (unused in release builds).
/// @param ...    Arguments matching the format string.
inline void dbgLog( [[maybe_unused]]  const char* format, ... )
{

#ifdef MU_DEBUG
#ifdef MU_MACOS
    //macOS only
    if (!format)
        return;

    if (strpbrk(format, "%") == nullptr)
    {
        mu::__impDbgLog(format);

        if (mu::__logInFileToo())
        {
            mu::__showLogInFile(format);
        }
        return;
    }
    va_list argptr;
    va_start(argptr, format);
    mu::__impDbgLog(format, argptr);
    if (mu::__logInFileToo())
    {
        char buffer[2048];
        int nLen = vsnprintf(buffer, 2048, format, argptr);
        mu::__showLogInFile(&buffer[0]);
    }
    va_end(argptr);
#else
    //non macOS
    //Windows, Linux
    if (!format)
        return;

    if (strpbrk(format, "%") == nullptr)
    {
        mu::__impDbgLog(format);
        return;
    }

	va_list argptr;
	va_start(argptr, format);
    char buffer[2048];
	int nLen = vsnprintf(buffer, 2048, format, argptr);
	va_end(argptr);

	buffer[2047] = 0;
	//assert(nLen >= 0);

	if (nLen > 0)
	{
		mu::__impDbgLog(&buffer[0]);
	}
#endif
#endif //DEBUG
}

} //namespace mu
