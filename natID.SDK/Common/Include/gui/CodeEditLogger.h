// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file CodeEditLogger.h
    @brief A view-based logger that aggregates and displays error and info messages from one or more CodeEdit controls. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once
#include <gui/CodeEdit.h>
#include <gui/View.h>
#include <gui/TableEdit.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <gui/VerticalLayout.h>
#include <cnt/SafeFullVector.h>

namespace gui
{

/// @brief A GUI view that implements the sc::ILog interface for collecting and presenting log entries from multiple CodeEdit editors.
class NATCODE_API CodeEditLogger : public gui::View, public sc::ILog
{
public:
    /// @brief Specifies how informational messages are rendered in the log table.
    enum class InfoType : td::BYTE {Text=0, Emojis};
protected:
    dp::IDataSet* _pDS; //Intern management ///< Internally managed data set for log entries
    cnt::SafeFullVector<CodeEdit*> _pCodeEdits; ///< List of CodeEdit instances tracked by this logger
    cnt::SafeFullVector<td::String> _editorEnumStrings; ///< Display names for each registered editor
    static cnt::SafeFullVector<td::String> typeEnumStrings; ///< Shared display names for log entry types
    gui::VerticalLayout _layout; ///< Layout manager for the logger view
    gui::TableEdit _table; ///< Table control used to display log entries
    td::UINT4 _noOfErrors = 0; ///< Total count of error entries in the log
    td::UINT2 _currentEditorPos = 0; ///< Index of the currently active editor
    td::BYTE _collectLoggs = 0; ///< Flag indicating whether log collection is active
    InfoType _infoType = InfoType::Text; ///< Display style for informational entries
private:
    CodeEditLogger() = delete;
    CodeEditLogger(const CodeEditLogger&) = delete;
    CodeEditLogger& operator =(const CodeEditLogger&) = delete;
public:
    /// @brief Constructs a CodeEditLogger with a data set, editor count, and optional display style.
    /// @param pDS Pointer to the data set used for log storage.
    /// @param nEditors Number of CodeEdit editors to track.
    /// @param infoType Display style for info messages (default: Text).
    CodeEditLogger(dp::IDataSet* pDS, td::UINT2 nEditors, InfoType infoType=InfoType::Text);

    //CodeEdit::ILog interface implementation
    /// @brief Clears all log entries from the logger.
    void clean() override;
    /// @brief Enables or disables log collection.
    /// @param bCollectLogs True to enable collecting, false to pause.
    void enableCollecting(bool bCollectLogs) override;
    /// @brief Sets the position of the module (editor) whose messages follow.
    /// @param editorPos The editor index to activate.
    void setModulePosition(td::UINT2 editorPos) override;
    /// @brief Adds a log entry with the given type.
    /// @param logEntry The log entry data.
    /// @param type The log entry type (default: Error).
    void add(const sc::ILog::Entry& logEntry, sc::ILog::Type type = sc::ILog::Type::Error) override;
    /// @brief Sets a translation function for localizing log messages.
    /// @param fn The translation function to use.
    void setTranslationFn(sc::TranslateFunction) override;
    /// @brief Adds a pre-translated string directly to the log.
    /// @param str The string to add without translation.
    /// @param type The log entry type (default: Info).
    void addWoTranslation(const td::String& str, Type type = Type::Info) override;
    /// @brief Translates and adds a message string to the log.
    /// @param pMsg The message key to translate.
    /// @param type The log entry type (default: Error).
    void translateAndAdd(const char* pMsg, Type type = Type::Error) override;
    /// @brief Translates and adds a message with line and column information.
    /// @param pMsg The message key to translate.
    /// @param lineNo The line number associated with the message.
    /// @param colNo The column number associated with the message.
    /// @param type The log entry type (default: Error).
    void translateAndAdd(const char* pMsg, td::UINT4 lineNo, td::UINT4 colNo, Type type = Type::Error) override;
    /// @brief Translates and adds a formatted message with positional info and parameters.
    /// @param pMsg The message key to translate.
    /// @param lineNo The line number associated with the message.
    /// @param colNo The column number associated with the message.
    /// @param type The log entry type.
    /// @param param1 First format parameter (additional variadic parameters may follow).
    void translateAndAdd(const char* pMsg, td::UINT4 lineNo, td::UINT4 colNo, Type type, const char* param1, ...) override;
    /// @brief Refreshes the log table display.
    void refresh() override;
    /// @brief Sets the display style for informational entries.
    /// @param infoType The InfoType to use.
    void setInfoType(InfoType infoType);
    /// @brief Registers a CodeEdit editor with this logger at the given position.
    /// @param pEdit Pointer to the CodeEdit instance.
    /// @param editorName Display name for the editor.
    /// @param editorPos The index position for this editor.
    void setEditor(CodeEdit* pEdit, const td::String& editorName, td::UINT2 editorPos);
    /// @brief Switches the logger to display messages for the given editor.
    /// @param editorPos The index of the editor to activate.
    void setCurrentEditor(td::UINT2 editorPos);
    /// @brief Serializes all log entries to a single string.
    /// @return A string containing all log entries.
    td::String toString() const;
    /// @brief Returns the total number of error entries in the log.
    /// @return The error count.
    td::UINT4 getNoOfErrors() const override;
    /// @brief Destructor.
    ~CodeEditLogger();
};

} //namespace gui
