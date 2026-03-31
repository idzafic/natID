// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <mu/ILogger.h>
#include <mu/mu.h>
#include <mu/TxtOutFile.h>
#include <mu/IAppSettings.h>
#include <td/DateTime.h>
#include <mu/Utils.h>


namespace mu
{
	class Logger : public ILogger
	{
	public:		
		bool isInteractive() const override
		{
			return false;
		}

		bool show(MsgType type, const td::String& header, const td::String& msg) const override
		{
			mu::TxtOutFile<std::ofstream>* pOstr = mu::getTracerPtr();
			if (!pOstr)
				return false;

			td::DateTime dt;
			dt.now();
			char str[64];
			dt.formc_str(str, 64);

			if (type == MsgType::Info)
			{
				*pOstr << "Info [" << str << "][" << header << "]: " << " Msg: " << msg << td::ENDL;
			}
			else if (type == MsgType::Warning)
			{
				*pOstr << "Warning [" << str << "][" << header << "]: " << " Msg: " << msg << td::ENDL;
			}
			else
			{
				*pOstr << "Error [" << str << "][" << header << "]: " << " Msg: " << msg << td::ENDL;
			}
			pOstr->flush();	
			return true;
		}

		bool show(const td::String& header, const td::String& msg, MsgType type = MsgType::Error) const override
		{
			return show(type, header, msg);
		}

		bool show(const char* header, const char* msg, MsgType type = MsgType::Error) const override
		{
			return show(type, td::String(header), td::String(msg));
		}
        
        void close() override
        {
            mu::TxtOutFile<std::ofstream>* pOstr = mu::getTracerPtr();
            if (pOstr)
                pOstr->close();
        }
	};
}
