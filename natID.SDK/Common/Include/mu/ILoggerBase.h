// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ILoggerBase.h
 *  @brief Base logger interface with stream insertion operators and buffered output.
 */
#pragma once
#include <sstream>
#include <string>

namespace mu
{
	/// @brief Abstract base class for loggers providing stream-style message accumulation and display.
	class ILoggerBase
	{
	protected:
		std::stringstream _ss; ///< Internal string stream buffer for accumulating message content.
	public:
		/// @brief Severity level for log messages.
		enum class MsgType : unsigned char { Info = 0, ///< Informational message.
		                                     Warning,   ///< Warning message.
		                                     Error      ///< Error message.
		                                   };

		/// @brief Displays a plain C-string message with a header and the given severity type.
		/// @param header Null-terminated category or source label.
		/// @param msg Null-terminated message body.
		/// @param type Severity of the message; defaults to MsgType::Error.
		/// @return True if the message was successfully shown; false otherwise.
		virtual bool show(const char* header, const char* msg, MsgType type = MsgType::Error) const = 0;

		/// @brief Appends an arbitrary value to the internal stream buffer.
		/// @tparam TIN Type of the value to stream; must be compatible with std::stringstream.
		/// @param val Value to append to the buffer.
		/// @return Reference to this logger for chained insertions.
		template <typename TIN>
		ILoggerBase& operator << (const TIN val)
		{
			_ss << val;
			return *this;
		}

		/// @brief Appends a compile-time-sized C-string literal to the internal stream buffer.
		/// @tparam size Compile-time length of the string literal (including null terminator).
		/// @param msg String literal to append.
		/// @return Reference to this logger for chained insertions.
		template <size_t size>
		ILoggerBase& operator << (const char(&msg)[size])
		{
			_ss << msg;
			return *this;
		}

		/// @brief Flushes the accumulated stream buffer to the display and clears it.
		/// @tparam size Compile-time length of the header string literal.
		/// @param header String literal used as the message title.
		/// @param type Severity of the message; defaults to MsgType::Error.
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
