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
#include <td/Date.h>

#ifdef MU_WINDOWS
#include <Windows.h>
#include <mem/LeakDetector.h>
#endif

namespace mu
{	
	class Application : public IApplication
	{
	public:
		using ProductInfo = struct _pi
		{
			td::String displayName; //human readable app (product) name
			td::String maintainer;	//company or developer
			td::String email;		//contact email
			td::String homePage;	//support home page
			td::String comment;		//detailed app description
			td::String version;
			td::String regShelExt;	//extensions to register for shell execute (.extension)
			td::String mimeType;	//for suppored file types
			td::Date versionDate;
			td::Date buildDate;
			td::INT2 majorVersion = 1;
			td::INT2 minorVersion = 0;
		};
	protected:
		//mu::ApplicationEnvironment _env;
		mu::Logger _log;
		mu::DbgLogger _logDbg;	
		ProductInfo _productInfo;
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

		 const ProductInfo& getProductInfo() const
		 {
			 return _productInfo;
		 }

	};
}
