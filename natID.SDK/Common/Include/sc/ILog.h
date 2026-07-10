// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file ILog.h
 *  @brief Interface for logging messages and errors within the solver framework.
 */
#pragma once
#include <td/Types.h>
#include <td/String.h>

namespace sc
{

using TranslateFunction = td::String (*)(const char*);

/// @brief Abstract interface for collecting, translating, and displaying log entries.
class ILog
{
public:
    /// @brief Severity level of a log entry.
    enum class Type : unsigned char {Info=0, Warning, Error};
    /// @brief A single log entry containing the message text and source location.
    using Entry = struct LE
    {
        td::String str; ///< Human-readable log message text.
//        td::UINT4 location;
        td::UINT4 lineNo; //1-based ///< Source line number (1-based).
        td::UINT4 colNo; //1-based ///< Source column number (1-based).
    };

    /// @brief Remove all existing log entries.
    virtual void clean() = 0;
    /// @brief Set the current module position identifier used when tagging new entries.
    /// @param pos Module position index.
    virtual void setModulePosition(td::UINT2) = 0;
    /// @brief Enable or disable log entry collection.
    /// @param bCollectLogs True to start collecting, false to stop.
    virtual void enableCollecting(bool bCollectLogs) = 0;
    /// @brief Append a pre-built log entry with the given severity.
    /// @param entry The log entry to add.
    /// @param type Severity level (default: Error).
    virtual void add(const Entry&, Type type = Type::Error) = 0;
    /// @brief Register a translation function used to localise message keys.
    /// @param fn Pointer to the translation function.
    virtual void setTranslationFn(TranslateFunction) = 0;
    /// @brief Append a message string directly, bypassing translation.
    /// @param str The message string to log.
    /// @param type Severity level (default: Info).
    virtual void addWoTranslation(const td::String& str, Type type = Type::Info) = 0;
    /// @brief Translate a message key and append the result to the log.
    /// @param pMsg Null-terminated message key.
    /// @param type Severity level (default: Error).
    virtual void translateAndAdd(const char* pMsg, Type type = Type::Error) = 0;
    /// @brief Translate a message key and append it together with source location info.
    /// @param pMsg Null-terminated message key.
    /// @param lineNo Source line number (1-based).
    /// @param colNo Source column number (1-based).
    /// @param type Severity level (default: Error).
    virtual void translateAndAdd(const char* pMsg, td::UINT4 lineNo, td::UINT4 colNo, Type type = Type::Error) = 0;
    //last one is used to send params that will be formatted (param placeholder is @
    /// @brief Translate a message key with positional parameters and append to the log.
    /// @param pMsg Null-terminated message key (use '@' as placeholder).
    /// @param lineNo Source line number (1-based).
    /// @param colNo Source column number (1-based).
    /// @param type Severity level.
    /// @param param1 First format parameter (variadic).
    virtual void translateAndAdd(const char* pMsg, td::UINT4 lineNo, td::UINT4 colNo, Type type, const char* param1, ...) = 0;
    /// @brief Notify the log to refresh or flush its display.
    virtual void refresh() = 0;
    /// @brief Return the total number of error-level entries currently in the log.
    /// @return Count of error entries.
    virtual td::UINT4 getNoOfErrors() const = 0;
};

/// @brief RAII helper that enables log collection on construction and disables it on destruction.
class LogEnabler
{
    sc::ILog* _pLog; ///< Pointer to the managed log instance.
    LogEnabler() = delete;
public:
    /// @brief Construct and start collecting log entries.
    /// @param pLog Pointer to the ILog instance to manage.
    LogEnabler(sc::ILog* pLog)
    : _pLog(pLog)
    {
        if (_pLog)
        {
            _pLog->clean();
            _pLog->enableCollecting(true);
        }

    }
    /// @brief Destruct and stop collecting log entries, then refresh the display.
    ~LogEnabler()
    {
        if (_pLog)
        {
            _pLog->enableCollecting(false);
            _pLog->refresh();
        }
    }
};

} //sc

