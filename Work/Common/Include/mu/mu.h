// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <mu/muLib.h>
#include <mu/Machine.h>
#include <td/BaseDateTime.h>
#include <fstream>
#include <mu/ILoggerBase.h>
#include <cstdarg>
#include <filesystem>

#ifdef MU_WINDOWS
namespace no
{
	namespace sock
	{
		class Init;
	}
}
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
    
MAINUTILS_API Regionals* getMainThreadRegionals();

MAINUTILS_API IAppSettings* getAppSettings();

MAINUTILS_API void getHomePath(std::filesystem::path& homePath);

MAINUTILS_API size_t getProcesID();

#ifdef MU_DEBUG
MAINUTILS_API bool createNatIDDbgLogFile(const char* natIDfbgLogFileName);
MAINUTILS_API bool createNatIDDbgLogFile(const std::filesystem::path& logFNPath);
#endif

MAINUTILS_API void getWorkingFolderPath(std::filesystem::path& workingPath);

extern MAINUTILS_API int TraceLevel;

MAINUTILS_API TxtOutFile<std::ofstream>& getTracer();

MAINUTILS_API TxtOutFile<std::ofstream>* getTracerPtr();

MAINUTILS_API void getCurrentLocalDate(td::BaseDate& bd);

MAINUTILS_API void getCurrentLocalTime(td::BaseTime& bt);

MAINUTILS_API void getCurrentDateTime(td::BaseDate& bd, td::BaseTime& bt, bool utc);
MAINUTILS_API void convertTime(td::BaseDate& bd, td::BaseTime& bt, bool toUTC);

MAINUTILS_API void getGUID(char guid[16]);

MAINUTILS_API td::LUINT8 calcHash(const char* strKey, int keyLen);

MAINUTILS_API const char* trc(td::UINT4 hashFile, const char* strKey, size_t& keyLen);

MAINUTILS_API const mu::ILoggerBase* getLoggerC();

MAINUTILS_API mu::ILoggerBase* getLogger();

MAINUTILS_API mem::IBufferAllocator* getMainThreadBufferAllocator();

//MAINUTILS_API void genRandomString(char* pBuff, size_t strLen);
#ifdef MU_DEBUG
#ifdef MU_MACOS

MAINUTILS_API void __impDbgLog(const char* format, va_list& argptr);

MAINUTILS_API void __impDbgLog(const char* msg);

MAINUTILS_API bool __logInFileToo();
MAINUTILS_API void __showLogInFile(const char* strLog);

#else
MAINUTILS_API void __impDbgLog(const char* msg);
#endif
#endif //MU_DEBUG
//MAINUTILS_API void getSystemInfo();

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

