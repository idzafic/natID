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
#include <mu/TxtOutFile.h>
#include <mu/IAppSettings.h>
#include <td/DateTime.h>
#include <mu/Utils.h>
#include <mu/ILogger.h>
#include <fstream>
#include <fo/FileOperations.h>

namespace mu
{
	class DbgLogger : public ILogger
	{
		int _fd = -1;

		//inline const std::ofstream* getFile() const
		//{
		//	return &_f;
		//}
	public:		

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
		~DbgLogger()
		{
			if (_fd >= 0)
				fo::closeOSFile(_fd);
		}

		bool isInteractive() const override
		{
			return false;
		}

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

		bool show(const td::String& header, const td::String& msg, MsgType type = MsgType::Error) const override
		{
			return show(type, header, msg);
		}

		bool show(const char* header, const char* msg, MsgType type = MsgType::Error) const override
		{
			return show(type, header, msg);
		}

		bool isOk() const
		{
			return _fd >= 0;
		}
        
        void close() override
        {
            if (_fd >= 0)
                fo::closeOSFile(_fd);
            _fd = -1;
        }
	};
}
