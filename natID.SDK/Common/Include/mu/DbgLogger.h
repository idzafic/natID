// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file DbgLogger.h
    @brief Provides a file-based debug logger that writes timestamped messages using OS-level file I/O. */
#pragma once
#include <mu/mu.h>
#include <mu/TxtOutFile.h>
#include <mu/IAppSettings.h>
#include <td/DateTime.h>
#include <mu/Utils.h>
#include <mu/ILogger.h>
#include <fstream>
#include <fo/FileOperations.h>

namespace mu
{
	/// @brief ILogger implementation that writes log messages to a text file using OS-level file descriptors.
	class DbgLogger : public ILogger
	{
		int _fd = -1; ///< OS file descriptor for the debug log file; -1 means the file is not open.

		//inline const std::ofstream* getFile() const
		//{
		//	return &_f;
		//}
	public:

		/// @brief Opens (or skips) a debug log file in the tracer path returned by IAppSettings.
		/// @param logFileName Base name for the log file, or nullptr to disable logging.
		DbgLogger(const char* logFileName)
		{
			if (logFileName)
			{
				mu::IAppSettings* pAppSet = mu::getAppSettings();
				if (pAppSet)
				{
					td::String tracerPath;
					pAppSet->getTracerPath(tracerPath);

					td::String strFN;
					strFN.format("%s/%s_%d.log", tracerPath.c_str(), logFileName, pAppSet->getProgID());
					_fd = fo::createTxtFileUsingOS(strFN);
				}
			}
		}

		/// @brief Closes the debug log file if it is still open.
		~DbgLogger()
		{
			if (_fd >= 0)
				fo::closeOSFile(_fd);
		}

		/// @brief Returns whether this logger operates interactively (always false for file logging).
		/// @return false, because this logger writes to a file rather than an interactive UI.
		bool isInteractive() const override
		{
			return false;
		}

		/// @brief Writes a timestamped log entry to the debug file.
		/// @param type The message severity (Info, Warning, or Error).
		/// @param header A short header or context label for the message.
		/// @param msg The detailed message text.
		/// @return true if the message was written successfully; false if the file is not open.
		bool show(ILogger::MsgType type, const td::String& header, const td::String& msg) const override
		{
			if (_fd < 0)
				return false;

			td::DateTime dt;
			dt.now();
			char str[64];
			int nTimeLen = dt.formc_str(str, 64);

			if (type == ILogger::MsgType::Info)
				fo::writeToOSFile(_fd, "Info [");
			else if (type == ILogger::MsgType::Warning)
				fo::writeToOSFile(_fd, "Warning [");
			else
				fo::writeToOSFile(_fd, "Error[");

			fo::writeToOSFile(_fd, str, nTimeLen);
			fo::writeToOSFile(_fd, "][");
			fo::writeToOSFile(_fd, header);
			fo::writeToOSFile(_fd, "] Msg: ");
			fo::writeToOSFile(_fd, msg);
			fo::writeToOSFile(_fd, "\n");

			fo::syncOSFile(_fd);
			return true;
		}

		/// @brief Overload that accepts header and message as td::String with a trailing type parameter.
		/// @param header Short context label for the message.
		/// @param msg Detailed message text.
		/// @param type Message severity; defaults to Error.
		/// @return true if the message was written successfully.
		bool show(const td::String& header, const td::String& msg, MsgType type = MsgType::Error) const override
		{
			return show(type, header, msg);
		}

		/// @brief Overload that accepts header and message as plain C strings.
		/// @param header Short context label for the message.
		/// @param msg Detailed message text.
		/// @param type Message severity; defaults to Error.
		/// @return true if the message was written successfully.
		bool show(const char* header, const char* msg, MsgType type = MsgType::Error) const override
		{
			return show(type, header, msg);
		}

		/// @brief Returns whether the debug log file was successfully opened.
		/// @return true if the file descriptor is valid (file is open and ready).
		bool isOk() const
		{
			return _fd >= 0;
		}

		/// @brief Closes the debug log file and resets the file descriptor.
		void close() override
		{
			if (_fd >= 0)
				fo::closeOSFile(_fd);
			_fd = -1;
		}
	};
}
