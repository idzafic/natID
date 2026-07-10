// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ILogger.h
 *  @brief Extended logger interface supporting interactive display and buffered message output.
 */
#pragma once
#include "ILoggerBase.h"
#include <td/String.h>
#include <mu/muLib.h>

namespace mu
{
	/// @brief Extends ILoggerBase with interactive display and per-type convenience methods.
	class ILogger : public ILoggerBase
	{
	public:
		using ILoggerBase::show;

		/// @brief Displays a message of the given type with a header and body, using td::String arguments.
		/// @param type Category of the message (Info, Warning, or Error).
		/// @param header Title or source label for the message dialog.
		/// @param msg Body text of the message.
		/// @return True if the message was successfully shown; false otherwise.
		virtual bool show(MsgType type, const td::String& header, const td::String& msg) const = 0;

		/// @brief Displays a message with header and body, with type defaulting to Error.
		/// @param header Title or source label for the message dialog.
		/// @param msg Body text of the message.
		/// @param type Category of the message; defaults to MsgType::Error.
		/// @return True if the message was successfully shown; false otherwise.
        virtual bool show(const td::String& header, const td::String& msg, MsgType type = MsgType::Error ) const = 0;

		/// @brief Returns true if this logger is capable of interactive (UI) display.
		/// @return True for interactive loggers; false for file-only or silent loggers.
		virtual bool isInteractive() const = 0;

		/// @brief Closes the logger and releases any associated output resources.
        virtual void close() = 0;

		/// @brief Convenience method to show an informational message.
		/// @param header Null-terminated title or source label.
		/// @param msg Null-terminated message body.
		/// @return True if the message was shown; false otherwise.
        bool showInfo(const char* header, const char* msg) const
        {
            return show(MsgType::Info, header, msg);
        }

		/// @brief Convenience method to show a warning message.
		/// @param header Null-terminated title or source label.
		/// @param msg Null-terminated message body.
		/// @return True if the message was shown; false otherwise.
        bool showWarning(const char* header, const char* msg) const
        {
            return show(MsgType::Warning, header, msg);
        }

		/// @brief Convenience method to show an error message.
		/// @param header Null-terminated title or source label.
		/// @param msg Null-terminated message body.
		/// @return True if the message was shown; false otherwise.
        bool showError(const char* header, const char* msg) const
        {
            return show(MsgType::Error, header, msg);
        }

		/// @brief Flushes the internal string stream to the logger display and clears the buffer.
		/// @param header Title or source label to use when displaying the buffered content.
		/// @param type Message type for the display; defaults to MsgType::Error.
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

	/// @brief Logs an informational message at the given level via the application logger.
	/// @param header Null-terminated category or source label.
	/// @param msg Null-terminated message text.
	/// @param level Numeric log level; messages are emitted only when level is below the threshold.
	MAINUTILS_API void logInfo(const char* header, const char* msg, unsigned int level = 100);

	/// @brief Logs a warning message at the given level via the application logger.
	/// @param header Null-terminated category or source label.
	/// @param msg Null-terminated message text.
	/// @param level Numeric log level; messages are emitted only when level is below the threshold.
	MAINUTILS_API void logWarning(const char* header, const char* msg, unsigned int level = 100);

	/// @brief Logs an error message at the given level via the application logger.
	/// @param header Null-terminated category or source label.
	/// @param msg Null-terminated message text.
	/// @param level Numeric log level; messages are emitted only when level is below the threshold.
	MAINUTILS_API void logError(const char* header, const char* msg, unsigned int level = 100);

	/// @brief Sets the maximum log level threshold; messages with a level at or above this value are suppressed.
	/// @param showLogsBelowThisLevel All messages with level strictly below this value will be shown.
	MAINUTILS_API void setLogLevel(unsigned int showLogsBelowThisLevel);

}
