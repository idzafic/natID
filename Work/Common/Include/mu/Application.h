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
//#include <mu/ApplicationEnvironment.h>
#include <td/String.h>
#include <mu/Logger.h>
#include <mu/DbgLogger.h>
#include <cnt/PushBackVector.h>
#include <mu/IApplication.h>

#ifdef MU_WINDOWS
#include <Windows.h>
#include <mem/LeakDetector.h>
#endif

namespace mu
{	
	class Application : public IApplication
	{
	public:
	protected:
		//mu::ApplicationEnvironment _env;
		mu::Logger _log;
		mu::DbgLogger _logDbg;		
	public:
		Application(int argc, const char** argv, const char* applicationGroup = "ba.natID", [[maybe_unused]] const char* dbgLogFileName = nullptr, bool createLogFile = true)
//#ifdef _DEBUG
//			: IApplication(argc, argv, "IDz", "R:\\")
//			, _logDbg(dbgLogFileName)
//#else
			//: IApplication(argc, argv, ".IDz", "+")
            : IApplication(argc, argv, applicationGroup, "+", createLogFile)
			, _logDbg(nullptr)
//#endif					
		{
#ifdef MU_WINDOWS
			::SetConsoleOutputCP( CP_UTF8 );
#endif			
		}

		~Application()
		{
		}

        mu::ILogger* getLogger() override
		{
			return &_log;
		}

         mu::ILogger* getDbgLogger() override
		{
			if (_logDbg.isOk())
				return &_logDbg;
			return nullptr;
		}
	};
}
