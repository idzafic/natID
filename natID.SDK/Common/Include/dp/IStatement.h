// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IStatement.h
    @brief Abstract interface for database statement execution, parameter binding, and result export. */
#pragma once
#include <dp/Types.h>
#include <cnt/StringBuilder.h>
#include <mem/PointerReleaser.h>
//#include "cnt/SafeFullVectorWithPushBack.h"
//#include "td/Variant.h"
#include <set>
#include <mu/ILogger.h>
#include <dp/Param.h>


namespace xml
{
	class Writer;
}


namespace dp
{
	class IDatabase;

/// @brief Abstract interface representing a prepared database statement with support for parameter binding,
///        result set iteration, and XML export.
	class IStatement
	{
	public:
		/// @brief Identifies the SQL operation type of the statement.
        enum class Type : unsigned char {
            DBS_DELETE=0,           ///< DELETE statement
            DBS_INSERT,             ///< INSERT statement
            DBS_SELECT,             ///< SELECT statement
            DBS_UPDATE,             ///< UPDATE statement
            DBS_SP_NORET,           ///< Stored procedure with no return value
            DBS_SP_RET,             ///< Stored procedure with a return value
            DBS_SP_NORET_NOPARAMS,  ///< Stored procedure with no return value and no parameters
            DBS_FN,                 ///< Database function call
            DBS_UNKNOWN             ///< Unknown or unspecified statement type
        };

		/// @brief Controls insertion behavior when a uniqueness conflict may occur.
        enum class SecureInsertType : unsigned char {
            SIT_INSERTONLY = 0,     ///< Perform a plain INSERT; fail if the record already exists
            SIT_INSERTIFNOTEXIST,   ///< Insert the record only if it does not already exist
            SIT_UPDATEORINSERT      ///< Update the existing record or insert a new one if absent
        };

		/// @brief Reports the outcome of a secure (conflict-aware) insert operation.
		enum class SecureInsertInfo : unsigned char {
            SII_ALREADYEXIST = 1,              ///< Record already existed; no change was made
            SII_INSERTED,                       ///< Record was successfully inserted
            SII_UPDATED_SI_INSERTEDASNEWNUMBER, ///< Record was updated or inserted as a new entry
            SII_NONE                            ///< No information available
        };

		/// @brief Describes a single column as reported by the database driver.
		typedef struct _cd
		{
			td::String name;    ///< Column name as returned by the database
			td::INT2 dbType;    ///< Native database type code
			td::INT2 precision; ///< Numeric precision or maximum character length
			td::INT2 length;    ///< Storage length in bytes
			td::INT2 scale;     ///< Number of digits to the right of the decimal point
			//td::DataType cppType;
		} DBColumnDesc;

		/// @brief Sets the number of rows to process in a single bulk operation.
		/// @param size Number of rows per bulk batch.
		virtual void setBulkSize(int size) = 0;

		/// @brief Executes the prepared statement.
		/// @return true if execution succeeded, false on error.
		virtual bool execute() = 0;

		/// @brief Advances the cursor to the next row in the result set.
		/// @return true if a next row exists, false when the result set is exhausted.
		virtual bool moveNext() = 0;

		/// @brief Closes the statement and releases associated server-side resources.
		/// @return true if the statement was closed successfully.
		virtual bool close() = 0;

		//virtual bool bindParam(BindDesc& dbParam) = 0;
		//virtual void bindColumns(Column* pColumns, int nColumns) = 0;

		/// @brief Allocates and returns the output column binding vector for the statement.
		/// @param nColumns Number of columns to allocate bindings for.
		/// @return Reference to the allocated column binding vector.
		virtual cnt::SafeFullVector<Column>& allocBindColumns(size_t nColumns) = 0;

		/// @brief Allocates and returns the parameter vector for the statement.
		/// @return Reference to the allocated parameter vector.
		virtual cnt::SafeFullVector<Param>& allocParams() = 0;

		/// @brief Returns the number of bound parameters in the statement.
		/// @return Count of parameters.
		virtual size_t getNoOfParams() const = 0;

		/// @brief Returns the number of output columns in the result set.
		/// @return Count of columns.
		virtual size_t getNoOfColumns() const = 0;

		/// @brief Retrieves a human-readable description of the last error.
		/// @param strErr String that receives the error description.
		virtual void getErrorStr(td::String& strErr) const = 0;

		/// @brief Clears the last error state of the statement.
		virtual void cleanError() = 0;

		/// @brief Executes an SQL string directly without using a prepared statement.
		/// @param pStatement Null-terminated SQL string to execute.
		/// @return true if execution succeeded, false on error.
		virtual bool execDirectly(const char* pStatement) = 0;

		/// @brief Writes column metadata descriptions to the given output stream.
		/// @param o Pointer to the output stream; may be null.
		virtual void showColDecription(std::ostream* o) = 0;

		/// @brief Returns the database-level column descriptions for the current result set.
		/// @return Const reference to the vector of DBColumnDesc structures.
		virtual const cnt::SafeFullVector<DBColumnDesc>& getDBColumnDescription() const = 0;

		/// @brief Looks up the zero-based position of a column by name.
		/// @param colName Null-terminated column name to search for.
		/// @return Zero-based column index, or -1 if not found.
		virtual int getColumnPos(const char* colName) const = 0;

		/// @brief Fills the provided vector with C++ column type descriptions derived from the result set.
		/// @param cppColDesc Vector that receives the CPPColumnDesc structures.
		virtual void getCPPColumnDescriptions(cnt::SafeFullVector<CPPColumnDesc>& cppColDesc) const = 0;

		/// @brief Returns a pointer to the bound output columns of the statement.
		/// @return Const pointer to the column binding vector, or null if not bound.
		virtual const cnt::SafeFullVector<Column>* getColumns() const = 0;

		/// @brief Indicates whether the statement is in a valid, usable state.
		/// @return true if the statement is valid and ready for use.
		virtual bool isOk() const = 0;

		/// @brief Indicates whether parameters have been successfully bound to the statement.
		/// @return true if parameters are bound.
		virtual bool areParamsBound() const = 0;

		/// @brief Indicates whether output columns have been successfully bound to the statement.
		/// @return true if columns are bound.
		virtual bool areColumnsBound() const = 0;

		//binds output columns without additional sql request
		/// @brief Prepares a subset of output columns for export, excluding the specified column names.
		/// @param excludeColumns Set of column names to exclude from the export binding.
		/// @return true if columns were successfully prepared for export.
		virtual bool prepareColumnsToExport(const std::set<td::String>& excludeColumns) = 0; //has to be called before exportXX calls

		/// @brief Prepares all output columns for export.
		/// @return true if all columns were successfully prepared for export.
		virtual bool prepareColumnsToExport() = 0; //has to be called before exportXX calls, all columns will be exported

		//binds output columns with additional sql request
		/// @brief Prepares output columns for export based on database table metadata, excluding specified columns.
		/// @param tableName Name of the database table used to fetch column metadata.
		/// @param excludeColumns Set of column names to exclude from the export binding.
		/// @return true if columns were successfully prepared for export.
		virtual bool prepareColumnsToExport(const char* tableName, const std::set<td::String>& excludeColumns) = 0; //has to be called before exportXX calls

		//binds output columns using prepared column description
		/// @brief Prepares output columns for export using a pre-built C++ column description list.
		/// @param cppColDesc Vector of CPPColumnDesc structures describing the columns to bind.
		virtual void prepareColumnsToExport(cnt::SafeFullVector<CPPColumnDesc>& cppColDesc) = 0; //needs to be called before exportXX calls

		/// @brief Executes the statement and exports the result set as XML child elements.
		/// @param w XML writer instance to output into.
		/// @param nodeName Name of the XML element wrapping each exported row.
		/// @param numberOfRows Maximum number of rows to export; 0 means export all rows.
		/// @return true if the export completed successfully.
		virtual bool exportToXML(xml::Writer& w, const char* nodeName, int numberOfRows = 0) = 0; //exports the whole content, execute is called inside, numberOfRows - if 0 all rows will be exported, otherwise numberOfRows

		/// @brief Exports the current row's column values as XML attributes.
		///exports single raw as attribs, user should call execute, and moveNext outside
		/// @param w XML writer instance to output into.
		virtual void exportRowToXML(xml::Writer& w) = 0;

		/// @brief Releases the statement object and frees all associated resources.
		virtual void release() = 0;

		//only for SQLite provider
		/// @brief Returns the bind position index for a named parameter (SQLite-specific).
		/// @param paramName Null-terminated name of the parameter to look up.
		/// @return One-based bind position, or 0 if not found.
		virtual int getParamBindPos(const char* paramName) const = 0;

		/// @brief Returns information about the result of the last secure insert operation.
		/// @return SecureInsertInfo value describing the outcome.
		virtual SecureInsertInfo getSecurityInsertInfo() = 0;

		//virtual IDatabase* getTranslationDB() = 0;

//		virtual IDatabase* getParentDB() = 0;

		/// @brief Logs a message through the associated logger with the given severity and content.
		/// @param logType Severity level of the log message.
		/// @param header Short header or category label for the message.
		/// @param msg Full message body to log.
		/// @return true if the message was successfully logged.
		virtual bool showLog(mu::ILogger::MsgType logType, const td::String& header, const td::String& msg) const = 0;
	};

	/// @brief Smart pointer type that automatically releases an IStatement on destruction.
	typedef mem::PointerReleaser<dp::IStatement> IStatementPtr;
}
