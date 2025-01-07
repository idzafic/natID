// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

	class IDatabase
	{
		friend class Transaction;
	protected:
		virtual bool beginTransation() = 0;
		virtual bool commit() = 0;
		virtual bool rollback() = 0;
	public:
		typedef struct _si
		{
			td::String name;
			td::String version;
			int maxConcurentActivities;
			bool columnsCanBeRetrievedInAnyOrder;
			bool columnCanBeRetrievedBeforeLastBound;
		} ServerInfo;

		enum class ConnType { CT_ODBC, CT_CLI, CT_OCI, CT_PG, CT_SQLITE};
        enum class ServerType { SER_NA = 0, SER_MSSQL = 1, SER_ORACLE, SER_DB2, SER_FIREBIRD, SER_POSTGRE, SER_MYSQL, SER_SYBASE, SER_TIMESTEN, SER_SQLITE3, SER_SQLITE_4 };
		enum class OptionType { OPTION_DATE_AS_NATIVE = 0, OPTION_TIME_AS_NATIVE, OPTION_DECIMAL_AS_BIGINT, OPTION_AUTOCOMMIT };
        enum class VerMSSQLServer { MS_SQLNA = 0, MS_SQL2000 = 1, MS_SQL2005 = 5, MS_SQL2008 = 8, MS_SQL2012 = 12, MS_SQL2014 = 14, MS_SQL2016 = 16 };
        enum class VerORACLE{ ORA_NA = 0, ORA_10 = 10, ORA_11 = 11, ORA_12 = 12};
        enum class VerPOSTGRE{ PG_LAST};
		//static ISQLDatabase* createSQLProvider(Type type, ServerType serverType);
		virtual bool initialize() = 0;
		virtual void setConnectionTimeOut(int msTimeOut) = 0;
		virtual bool connect(const td::String& connectionString, const td::String* pUserName = nullptr, const td::String* pPassword = nullptr) = 0;
		virtual bool isConnected() const = 0;
		virtual ConnType getConnType() const = 0;

		virtual bool execDirectly(const char* pStatement) = 0;
		virtual IStatement* createStatement(IStatement::Type statementType, const char* pStatement) = 0;
		virtual IStatement* createStatement(IStatement::Type statementType, const td::String& strStatement) = 0;
		virtual IStatement* createStatement(const char* pStatement) = 0;  //in case of stored procedure returning RecordSet put 'r:' in front of statement in pStatement string
		virtual IStatement* createStatement(const td::String& strStatament) = 0;  //in case of stored procedure returning RecordSet put 'r:' in front of the statement in pStatement string

//		///noKeyElems has to be the first elements in vDesc which create the primary key of table tableName
//		virtual IStatement* createSecureInsertStatement(IStatement::SecureInsertType type, const char* tableName, cnt::SafeFullVector<td::VariantDesc>& vDesc, int noKeyElems) = 0;
//		//first noKeyElems are stored in vKeyElems, the rest in vDesc
//		virtual IStatement* createSecureInsertStatement(IStatement::SecureInsertType type, const char* tableName, cnt::SafeFullVector<td::VariantDesc>& vKeyElems, int noKeyElems, cnt::SafeFullVector<td::VariantDesc>& vDesc) = 0;

		virtual void getErrorStr(td::String& strErr) const = 0;
        virtual td::String getErrorStr() const = 0;
		virtual void cleanError() = 0;
		virtual void release() = 0;
		virtual void getServerInfo(ServerInfo& si) = 0;
		virtual bool getCPPColumnDescriptions(const char* tableName, const std::set<td::String>& excludeColumns, cnt::SafeFullVector<CPPColumnDesc>& cppColumns) = 0;
		virtual bool getAttribDescriptions(const char* tableName, cnt::SafeFullVector<td::VariantDesc>& vDesc) = 0;
		virtual void* getRawPtr() = 0;
		virtual void setOption(IDatabase::OptionType opt, int optValue) = 0;
		//virtual void attachTranslationDB(IDatabase* pDBTranslation) = 0;
		//virtual IDatabase* getTranslationDB() = 0;		
		
//		virtual IRecordset* createRecordset(IDataSet::MemPool memPoolSize, const char* xmlConfigFile, int selPos = 0, IDataSet::Execution loadingType = IDataSet::Execution::EX_NO, bool readOnly = false) = 0;

		virtual IDataSet* createDataSet(IDataSet::Size memPoolSize, const td::String& strStatement, IDataSet::Execution loadingType = IDataSet::Execution::EX_SINGLE, bool readOnly = true) = 0;

		virtual IDataSet* createSingleRowDataSet(const td::String& strStatement, IDataSet::Execution loadingType = IDataSet::Execution::EX_SINGLE, bool readOnly = true) = 0;
        
        ///small size data set
		virtual IDataSet* createSmallDataSet(const td::String& strStatement, IDataSet::Execution loadingType = IDataSet::Execution::EX_SINGLE, bool readOnly = true) = 0;
        
        ///medium size data set
		virtual IDataSet* createDataSet(const td::String& strStatement, IDataSet::Execution loadingType = IDataSet::Execution::EX_SINGLE, bool readOnly = true) = 0;
        
        ///large size data set
		virtual IDataSet* createLargeDataSet(const td::String& strStatement, IDataSet::Execution loadingType = IDataSet::Execution::EX_SINGLE, bool readOnly = true) = 0;
		
		//virtual IRecordset* createDataProvider(IRecordset::MemoryPool memPoolSize, const char* xmlConfigFile, int selPos = 0, IRecordset::DataLoad loadingType = IRecordset::IRS_NO_DATALOAD, bool readOnly = false) = 0;
						
		//treba biti premjesteno u myUtils.dll --ODAVDE---
		
		//virtual IRecordset::ConfigFormat getDPConfigFormat() const = 0;
		//virtual void setLogger(const mu::ILogger* pLogger) = 0;
		virtual const mu::ILogger* getLogger() const = 0;
		//treba biti premjesteno u myUtils.dll --DO OVDJE---
	};

	typedef mem::PointerReleaser<dp::IDatabase> IDatabasePtr;

	class Transaction
	{
		dp::IDatabase* _pDB;
		bool _transactionActive;
	public:
		Transaction(IDatabase* pDB)
			: _pDB(pDB)
			, _transactionActive(true)
		{
			_transactionActive = pDB->beginTransation();
		}

		Transaction(IDatabasePtr& pDB)
			: _pDB(pDB.ptr())
			, _transactionActive(true)
		{
			_transactionActive = pDB->beginTransation();
		}

		bool commit()
		{
			assert(_transactionActive);
			if (!_transactionActive)
				return false;
			_transactionActive = false;			
			return _pDB->commit();
		}

		bool rollBack()
		{
			assert(_transactionActive);
			if (!_transactionActive)
				return false;
			_transactionActive = false;
			return _pDB->rollback();
		}

		~Transaction()
		{
			if (_transactionActive)
				_pDB->rollback();
		}
	};

	DATAPROVIDER_API IDatabase* create(IDatabase::ConnType connType, IDatabase::ServerType serverType);
	DATAPROVIDER_API void setMainDatabase(IDatabase* pDB);
	DATAPROVIDER_API IDatabase* getMainDatabase();
    DATAPROVIDER_API IDatabase* createEmptySQLiteDatabase(const td::String& fileName);
    DATAPROVIDER_API IDatabase* createAndConnect(td::String& connectioonError, IDatabase::ConnType connType, IDatabase::ServerType serverType, int serverVersion, const char* serverNamedInstanceOrDBLocation, const char* dataBaseName, bool trustedConnection = true, const char* userName = nullptr, const char* pwd = nullptr, int odbcVersion=17);
	DATAPROVIDER_API dp::IDataSet* createConnectionlessDataSet(IDataSet::Size memPoolSize);
} //namespace dp
