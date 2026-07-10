// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Application.h
    @brief Provides the concrete Application class that wires together logging, product metadata, and the IApplication lifecycle. */
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
	/// @brief Concrete application class that owns a standard logger, a debug logger, and product information.
	class Application : public IApplication
	{
	public:
		/// @brief Holds human-readable metadata that describes the application product.
		using ProductInfo = struct _pi
		{
			td::String displayName;    ///< Human-readable application (product) name.
			td::String maintainer;     ///< Company or individual developer name.
			td::String email;          ///< Contact e-mail address for support.
			td::String homePage;       ///< Support or product home-page URL.
			td::String comment;        ///< Detailed application description.
			td::String version;        ///< Version string of the product.
			td::String regShelExt;     ///< File extensions to register for shell-execute (e.g. ".ext").
			td::String mimeType;       ///< MIME type string for supported file types.
			td::Date versionDate;      ///< Date of the current version release.
			td::Date buildDate;        ///< Date the application binary was built.
			td::INT2 majorVersion = 1; ///< Major version number.
			td::INT2 minorVersion = 0; ///< Minor version number.
		};
	protected:
		//mu::ApplicationEnvironment _env;
		mu::Logger _log; ///< Standard application logger.
		mu::DbgLogger _logDbg; ///< Debug-only file-based logger.
		ProductInfo _productInfo; ///< Product metadata for this application.
	public:
		/// @brief Constructs the application, initializes the logging subsystem, and sets the console code page on Windows.
		/// @param argc Number of command-line arguments.
		/// @param argv Array of command-line argument strings.
		/// @param applicationGroup Reverse-domain identifier for the application group (e.g., "ba.natID").
		/// @param dbgLogFileName Optional file name used for the debug log; pass nullptr to disable.
		/// @param createLogFile If true, the standard log file is created on startup.
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

		/// @brief Destructor; cleans up the application instance.
		~Application()
		{
		}

		/// @brief Returns a pointer to the standard application logger.
		/// @return Pointer to the mu::ILogger instance used for standard output.
        mu::ILogger* getLogger() override
		{
			return &_log;
		}

		/// @brief Returns a pointer to the debug logger if it has been successfully initialized.
		/// @return Pointer to the debug mu::ILogger, or nullptr if debug logging is inactive.
         mu::ILogger* getDbgLogger() override
		{
			if (_logDbg.isOk())
				return &_logDbg;
			return nullptr;
		}

		/// @brief Returns a const reference to the product information structure.
		/// @return Const reference to the ProductInfo held by this application.
		 const ProductInfo& getProductInfo() const
		 {
			 return _productInfo;
		 }

	};
}
