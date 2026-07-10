// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IDatabase.h @brief Abstract database interface and transaction helper for the data provider layer. */
#pragma once
#include <dp/DataProviderLib.h>
#include <dp/IStatement.h>
#include <dp/IDataSet.h>
#include <fo/FileOperations.h>
#include <cnt/SafeFullVector.h>

//PRAVILA RADA SA BAZAMA PODATAKA
/*
1. koristiti uvijek mala slova. Postgresql koristi mala slova iskljucivo
2. Ne koristiti INPUT/OUTPUT Parametre, ne rade na svim serverima. U select statementu mogu biti samo input parametri. Firebird koristi
recordset za vracanje output parametara
3. U Stored procedurama nikad ne koristiti SELECT *, nego navesti sve elemente selekcije. Firebird ne dozvoljava ovo.
4. U SP ne mijesati output i select (vidi 2.). Moguce opcije:
-SP sa input parametrima i select recordsetom,
-SP sa input i output parametrima bez selekta
*/

namespace mu
{
	class ILogger;
}

namespace dp
{
	class Transaction;

	/// @brief Abstract interface representing a database connection with statement and dataset factory methods.
	class IDatabase
	{
		friend class Transaction;
	protected:
		/// @brief Begins a database transaction.
		/// @return true on success, false on failure.
		virtual bool beginTransation() = 0;
		/// @brief Commits the current transaction.
		/// @return true on success, false on failure.
		virtual bool commit() = 0;
		/// @brief Rolls back the current transaction.
		/// @return true on success, false on failure.
		virtual bool rollback() = 0;
	public:
		/// @brief Describes server capabilities and metadata.
		typedef struct _si
		{
			td::String name;                         ///< Server product name
			td::String version;                      ///< Server version string
			int maxConcurentActivities;              ///< Maximum number of concurrent statements
			bool columnsCanBeRetrievedInAnyOrder;    ///< Whether result columns may be read out-of-order
			bool columnCanBeRetrievedBeforeLastBound;///< Whether a column can be read before all columns are bound
		} ServerInfo;

		/// @brief Connection type enumeration.
		enum class ConnType { CT_ODBC, CT_CLI, CT_OCI, CT_PG, CT_SQLITE};
		/// @brief Server type enumeration.
        enum class ServerType { SER_NA = 0, SER_MSSQL = 1, SER_ORACLE, SER_DB2, SER_FIREBIRD, SER_POSTGRE, SER_MYSQL, SER_SYBASE, SER_TIMESTEN, SER_SQLITE3, SER_SQLITE_4 };
		/// @brief Database option type enumeration.
		enum class OptionType { OPTION_DATE_AS_NATIVE = 0, OPTION_TIME_AS_NATIVE, OPTION_DECIMAL_AS_BIGINT, OPTION_AUTOCOMMIT };
		/// @brief Microsoft SQL Server version enumeration.
        enum class VerMSSQLServer { MS_SQLNA = 0, MS_SQL2000 = 1, MS_SQL2005 = 5, MS_SQL2008 = 8, MS_SQL2012 = 12, MS_SQL2014 = 14, MS_SQL2016 = 16 };
		/// @brief Oracle server version enumeration.
        enum class VerORACLE{ ORA_NA = 0, ORA_10 = 10, ORA_11 = 11, ORA_12 = 12};
		/// @brief PostgreSQL server version enumeration.
        enum class VerPOSTGRE{ PG_LAST};
		//static ISQLDatabase* createSQLProvider(Type type, ServerType serverType);
		/// @brief Performs driver/library initialization required before connecting.
		/// @return true on success.
		virtual bool initialize() = 0;
		/// @brief Sets the connection timeout.
		/// @param msTimeOut Timeout in milliseconds.
		virtual void setConnectionTimeOut(int msTimeOut) = 0;
		/// @brief Connects to the database server.
		/// @param connectionString Connection string.
		/// @param pUserName Optional user name.
		/// @param pPassword Optional password.
		/// @return true on success.
		virtual bool connect(const td::String& connectionString, const td::String* pUserName = nullptr, const td::String* pPassword = nullptr) = 0;
		/// @brief Checks whether the connection is currently active.
		/// @return true if connected.
		virtual bool isConnected() const = 0;
		/// @brief Returns the connection type.
		/// @return ConnType enum value.
		virtual ConnType getConnType() const = 0;

		/// @brief Executes an SQL statement directly without creating a statement object.
		/// @param pStatement SQL string to execute.
		/// @return true on success.
		virtual bool execDirectly(const char* pStatement) = 0;
		/// @brief Creates a typed statement object for the given SQL string.
		/// @param statementType Type of the statement (SELECT, INSERT, etc.).
		/// @param pStatement SQL string.
		/// @return Pointer to the new IStatement; caller must release it.
		virtual IStatement* createStatement(IStatement::Type statementType, const char* pStatement) = 0;
		/// @brief Creates a typed statement object for the given SQL string (td::String overload).
		/// @param statementType Type of the statement.
		/// @param strStatement SQL string.
		/// @return Pointer to the new IStatement; caller must release it.
		virtual IStatement* createStatement(IStatement::Type statementType, const td::String& strStatement) = 0;
		/// @brief Creates a statement with auto-detected type.
		/// @param pStatement SQL string; prefix with 'r:' for stored procedures returning a result set.
		/// @return Pointer to the new IStatement; caller must release it.
		virtual IStatement* createStatement(const char* pStatement) = 0;  //in case of stored procedure returning RecordSet put 'r:' in front of statement in pStatement string
		/// @brief Creates a statement with auto-detected type (td::String overload).
		/// @param strStatament SQL string; prefix with 'r:' for stored procedures returning a result set.
		/// @return Pointer to the new IStatement; caller must release it.
		virtual IStatement* createStatement(const td::String& strStatament) = 0;  //in case of stored procedure returning RecordSet put 'r:' in front of the statement in pStatement string

//		///noKeyElems has to be the first elements in vDesc which create the primary key of table tableName
//		virtual IStatement* createSecureInsertStatement(IStatement::SecureInsertType type, const char* tableName, cnt::SafeFullVector<td::VariantDesc>& vDesc, int noKeyElems) = 0;
//		//first noKeyElems are stored in vKeyElems, the rest in vDesc
//		virtual IStatement* createSecureInsertStatement(IStatement::SecureInsertType type, const char* tableName, cnt::SafeFullVector<td::VariantDesc>& vKeyElems, int noKeyElems, cnt::SafeFullVector<td::VariantDesc>& vDesc) = 0;

		/// @brief Retrieves the last error message as a string.
		/// @param strErr Output string to receive the error description.
		virtual void getErrorStr(td::String& strErr) const = 0;
		/// @brief Returns the last error message as a td::String value.
		/// @return Error description string.
        virtual td::String getErrorStr() const = 0;
		/// @brief Clears the last stored error.
		virtual void cleanError() = 0;
		/// @brief Releases the database object and frees all associated resources.
		virtual void release() = 0;
		/// @brief Retrieves server capability information.
		/// @param si Output ServerInfo structure to populate.
		virtual void getServerInfo(ServerInfo& si) = 0;
		/// @brief Retrieves C++ column descriptors for a given table, excluding specified columns.
		/// @param tableName Name of the database table.
		/// @param excludeColumns Set of column names to exclude from the result.
		/// @param cppColumns Output vector of CPPColumnDesc descriptors.
		/// @return true on success.
		virtual bool getCPPColumnDescriptions(const char* tableName, const std::set<td::String>& excludeColumns, cnt::SafeFullVector<CPPColumnDesc>& cppColumns) = 0;
		/// @brief Retrieves variant attribute descriptors for a given table.
		/// @param tableName Name of the database table.
		/// @param vDesc Output vector of VariantDesc descriptors.
		/// @return true on success.
		virtual bool getAttribDescriptions(const char* tableName, cnt::SafeFullVector<td::VariantDesc>& vDesc) = 0;
		/// @brief Returns a raw pointer to the underlying driver handle.
		/// @return Opaque driver-specific pointer.
		virtual void* getRawPtr() = 0;
		/// @brief Sets a database-level option.
		/// @param opt Option type to set.
		/// @param optValue Integer value for the option.
		virtual void setOption(IDatabase::OptionType opt, int optValue) = 0;
		//virtual void attachTranslationDB(IDatabase* pDBTranslation) = 0;
		//virtual IDatabase* getTranslationDB() = 0;

//		virtual IRecordset* createRecordset(IDataSet::MemPool memPoolSize, const char* xmlConfigFile, int selPos = 0, IDataSet::Execution loadingType = IDataSet::Execution::EX_NO, bool readOnly = false) = 0;

		/// @brief Creates a data set backed by a memory pool of the specified size.
		/// @param memPoolSize Pool size hint.
		/// @param strStatement SQL SELECT statement.
		/// @param loadingType Execution/loading mode.
		/// @param readOnly true if the dataset is read-only.
		/// @return Pointer to the new IDataSet; caller must release it.
		virtual IDataSet* createDataSet(IDataSet::Size memPoolSize, const td::String& strStatement, IDataSet::Execution loadingType = IDataSet::Execution::EX_SINGLE, bool readOnly = true) = 0;

		/// @brief Creates a data set limited to a single row.
		/// @param strStatement SQL SELECT statement expected to return at most one row.
		/// @param loadingType Execution/loading mode.
		/// @param readOnly true if the dataset is read-only.
		/// @return Pointer to the new single-row IDataSet; caller must release it.
		virtual IDataSet* createSingleRowDataSet(const td::String& strStatement, IDataSet::Execution loadingType = IDataSet::Execution::EX_SINGLE, bool readOnly = true) = 0;

        ///small size data set
		/// @brief Creates a small-capacity data set.
		/// @param strStatement SQL SELECT statement.
		/// @param loadingType Execution/loading mode.
		/// @param readOnly true if the dataset is read-only.
		/// @return Pointer to the new IDataSet; caller must release it.
		virtual IDataSet* createSmallDataSet(const td::String& strStatement, IDataSet::Execution loadingType = IDataSet::Execution::EX_SINGLE, bool readOnly = true) = 0;

        ///medium size data set
		/// @brief Creates a medium-capacity data set.
		/// @param strStatement SQL SELECT statement.
		/// @param loadingType Execution/loading mode.
		/// @param readOnly true if the dataset is read-only.
		/// @return Pointer to the new IDataSet; caller must release it.
		virtual IDataSet* createDataSet(const td::String& strStatement, IDataSet::Execution loadingType = IDataSet::Execution::EX_SINGLE, bool readOnly = true) = 0;

        ///large size data set
		/// @brief Creates a large-capacity data set.
		/// @param strStatement SQL SELECT statement.
		/// @param loadingType Execution/loading mode.
		/// @param readOnly true if the dataset is read-only.
		/// @return Pointer to the new IDataSet; caller must release it.
		virtual IDataSet* createLargeDataSet(const td::String& strStatement, IDataSet::Execution loadingType = IDataSet::Execution::EX_SINGLE, bool readOnly = true) = 0;

		//virtual IRecordset* createDataProvider(IRecordset::MemoryPool memPoolSize, const char* xmlConfigFile, int selPos = 0, IRecordset::DataLoad loadingType = IRecordset::IRS_NO_DATALOAD, bool readOnly = false) = 0;

		//treba biti premjesteno u myUtils.dll --ODAVDE---

		//virtual IRecordset::ConfigFormat getDPConfigFormat() const = 0;
		//virtual void setLogger(const mu::ILogger* pLogger) = 0;
		/// @brief Returns the logger associated with this database connection.
		/// @return Const pointer to the ILogger instance; may be nullptr.
		virtual const mu::ILogger* getLogger() const = 0;
		//treba biti premjesteno u myUtils.dll --DO OVDJE---
	};

	typedef mem::PointerReleaser<dp::IDatabase> IDatabasePtr; ///< RAII smart pointer that calls release() on destruction.

	/// @brief RAII wrapper that automatically commits or rolls back a database transaction.
	class Transaction
	{
		dp::IDatabase* _pDB;       ///< Pointer to the owning database
		bool _transactionActive;   ///< true while the transaction is open
	public:
		/// @brief Begins a transaction on the given database.
		/// @param pDB Raw database pointer.
		Transaction(IDatabase* pDB)
			: _pDB(pDB)
			, _transactionActive(true)
		{
			_transactionActive = pDB->beginTransation();
		}

		/// @brief Begins a transaction using a smart pointer to the database.
		/// @param pDB Smart pointer wrapping the database.
		Transaction(IDatabasePtr& pDB)
			: _pDB(pDB.ptr())
			, _transactionActive(true)
		{
			_transactionActive = pDB->beginTransation();
		}

		/// @brief Commits the transaction.
		/// @return true on success.
		bool commit()
		{
			assert(_transactionActive);
			if (!_transactionActive)
				return false;
			_transactionActive = false;
			return _pDB->commit();
		}

		/// @brief Rolls back the transaction.
		/// @return true on success.
		bool rollBack()
		{
			assert(_transactionActive);
			if (!_transactionActive)
				return false;
			_transactionActive = false;
			return _pDB->rollback();
		}

		/// @brief Destructor — automatically rolls back if commit() was not called.
		~Transaction()
		{
			if (_transactionActive)
				_pDB->rollback();
		}
	};

	/// @brief Factory function that creates a database object for the given connection and server type.
	/// @param connType Connection technology to use.
	/// @param serverType Target database server.
	/// @return Pointer to the new IDatabase; caller must call release() when done.
	DATAPROVIDER_API IDatabase* create(IDatabase::ConnType connType, IDatabase::ServerType serverType);
	/// @brief Stores a global main database pointer accessible application-wide.
	/// @param pDB Database pointer to register as main.
	DATAPROVIDER_API void setMainDatabase(IDatabase* pDB);
	/// @brief Returns the previously registered main database pointer.
	/// @return Pointer to the main IDatabase; may be nullptr.
	DATAPROVIDER_API IDatabase* getMainDatabase();
	/// @brief Creates and connects an empty SQLite database file.
	/// @param fileName Path to the SQLite database file.
	/// @return Pointer to the new IDatabase; caller must call release() when done.
    DATAPROVIDER_API IDatabase* createEmptySQLiteDatabase(const td::String& fileName);
	/// @brief Creates a database object and immediately connects to the server.
	/// @param connectioonError Output string receiving error description on failure.
	/// @param connType Connection technology to use.
	/// @param serverType Target database server type.
	/// @param serverVersion Server version identifier.
	/// @param serverNamedInstanceOrDBLocation Server name, instance, or file path.
	/// @param dataBaseName Database (catalog) name.
	/// @param trustedConnection true to use Windows trusted authentication.
	/// @param userName Optional user name (used when trustedConnection is false).
	/// @param pwd Optional password.
	/// @param odbcVersion ODBC driver version number.
	/// @return Pointer to the connected IDatabase, or nullptr on failure.
    DATAPROVIDER_API IDatabase* createAndConnect(td::String& connectioonError, IDatabase::ConnType connType, IDatabase::ServerType serverType, int serverVersion, const char* serverNamedInstanceOrDBLocation, const char* dataBaseName, bool trustedConnection = true, const char* userName = nullptr, const char* pwd = nullptr, int odbcVersion=17);
	/// @brief Creates a connectionless (in-memory) data set with the specified pool size.
	/// @param memPoolSize Memory pool size hint for the dataset.
	/// @return Pointer to the new IDataSet; caller must release it.
	DATAPROVIDER_API dp::IDataSet* createConnectionlessDataSet(IDataSet::Size memPoolSize);
} //namespace dp
