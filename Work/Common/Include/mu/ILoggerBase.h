// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <sstream>
#include <string>

namespace mu
{
	class ILoggerBase
	{
	protected:
		std::stringstream _ss;
	public:
		enum class MsgType : unsigned char { Info = 0, Warning, Error};
		virtual bool show(const char* header, const char* msg, MsgType type = MsgType::Error) const = 0;
		
		template <typename TIN>
		ILoggerBase& operator << (const TIN val)
		{
			_ss << val;
			return *this;		
		}

		template <size_t size>
		ILoggerBase& operator << (const char(&msg)[size])
		{
			_ss << msg;
			return *this;
		}

		template <size_t size>
		void showFromBuffer(const char (&header)[size], MsgType type = MsgType::Error)
		{
			std::string str (_ss.str());
			if (str.length() > 0)
			{
				show(&header[0], str.c_str(), type);
				_ss.str("");
			}
		}
	};
}
