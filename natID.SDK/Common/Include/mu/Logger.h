// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Logger.h
    @brief Concrete file-based logger that writes timestamped messages to a trace output file. */
#pragma once
#include <mu/ILogger.h>
#include <mu/mu.h>
#include <mu/TxtOutFile.h>
#include <mu/IAppSettings.h>
#include <td/DateTime.h>
#include <mu/Utils.h>


namespace mu
{
	/// @brief File-based logger implementation that writes Info, Warning, and Error messages with timestamps.
	class Logger : public ILogger
	{
	public:
		/// @brief Returns false because this logger operates non-interactively (writes to file).
		/// @return Always false.
		bool isInteractive() const override
		{
			return false;
		}

		/// @brief Writes a timestamped message of the given type to the trace output file.
		/// @param type The message severity type (Info, Warning, or Error).
		/// @param header A short header string identifying the source or context.
		/// @param msg The message body text.
		/// @return true if the tracer is available and the message was written; false otherwise.
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

		/// @brief Writes a message using td::String header and message, forwarding to the typed overload.
		/// @param header A short header string identifying the source or context.
		/// @param msg The message body text.
		/// @param type The message severity type; defaults to Error.
		/// @return true if the message was written successfully; false otherwise.
		bool show(const td::String& header, const td::String& msg, MsgType type = MsgType::Error) const override
		{
			return show(type, header, msg);
		}

		/// @brief Writes a message using raw C-string header and message.
		/// @param header Null-terminated C-string identifying the source or context.
		/// @param msg Null-terminated C-string with the message body.
		/// @param type The message severity type; defaults to Error.
		/// @return true if the message was written successfully; false otherwise.
		bool show(const char* header, const char* msg, MsgType type = MsgType::Error) const override
		{
			return show(type, td::String(header), td::String(msg));
		}

        /// @brief Closes the underlying trace output file.
        void close() override
        {
            mu::TxtOutFile<std::ofstream>* pOstr = mu::getTracerPtr();
            if (pOstr)
                pOstr->close();
        }
	};
}
