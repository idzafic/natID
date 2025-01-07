// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include "ILoggerBase.h"
#include <td/String.h>
#include <mu/muLib.h>

namespace mu
{
	class ILogger : public ILoggerBase
	{		
	public:	
		using ILoggerBase::show;
		virtual bool show(MsgType type, const td::String& header, const td::String& msg) const = 0;
        virtual bool show(const td::String& header, const td::String& msg, MsgType type = MsgType::Error ) const = 0;
		virtual bool isInteractive() const = 0;
        virtual void close() = 0;

        bool showInfo(const char* header, const char* msg) const
        {
            return show(MsgType::Info, header, msg);
        }
        bool showWarning(const char* header, const char* msg) const
        {
            return show(MsgType::Warning, header, msg);
        }
        
        bool showError(const char* header, const char* msg) const
        {
            return show(MsgType::Error, header, msg);
        }
        
		void showFromBuffer(const td::String& header, MsgType type = MsgType::Error)
		{
			td::String str (_ss.str());
			if (str.length() > 0)
			{
				show(type, header, str);
				_ss.str("");
			}
		}
	};

	MAINUTILS_API void logInfo(const char* header, const char* msg, unsigned int level = 100);
	MAINUTILS_API void logWarning(const char* header, const char* msg, unsigned int level = 100);
	MAINUTILS_API void logError(const char* header, const char* msg, unsigned int level = 100);
	MAINUTILS_API void setLogLevel(unsigned int showLogsBelowThisLevel);

}
