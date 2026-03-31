// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

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

	class IStatement
	{
	public:
        enum class Type : unsigned char { DBS_DELETE=0, DBS_INSERT, DBS_SELECT, DBS_UPDATE, DBS_SP_NORET, DBS_SP_RET, DBS_SP_NORET_NOPARAMS, DBS_FN, DBS_UNKNOWN } ;
        enum class SecureInsertType : unsigned char { SIT_INSERTONLY = 0, SIT_INSERTIFNOTEXIST, SIT_UPDATEORINSERT } ;
		enum class SecureInsertInfo : unsigned char { SII_ALREADYEXIST = 1, SII_INSERTED, SII_UPDATED_SI_INSERTEDASNEWNUMBER, SII_NONE } ;
		typedef struct _cd
		{
			td::String name;
			td::INT2 dbType;
			td::INT2 precision;
			td::INT2 length;
			td::INT2 scale;
			//td::DataType cppType;
		} DBColumnDesc;

		virtual void setBulkSize(int size) = 0;
		virtual bool execute() = 0;
		virtual bool moveNext() = 0;
		virtual bool close() = 0;
		//virtual bool bindParam(BindDesc& dbParam) = 0;
		//virtual void bindColumns(Column* pColumns, int nColumns) = 0;
		virtual cnt::SafeFullVector<Column>& allocBindColumns(size_t nColumns) = 0;
		virtual cnt::SafeFullVector<Param>& allocParams() = 0;
		virtual size_t getNoOfParams() const = 0;
		virtual size_t getNoOfColumns() const = 0;
		virtual void getErrorStr(td::String& strErr) const = 0;
		virtual void cleanError() = 0;
		virtual bool execDirectly(const char* pStatement) = 0;
		virtual void showColDecription(std::ostream* o) = 0;
		virtual const cnt::SafeFullVector<DBColumnDesc>& getDBColumnDescription() const = 0;
		virtual int getColumnPos(const char* colName) const = 0;
		virtual void getCPPColumnDescriptions(cnt::SafeFullVector<CPPColumnDesc>& cppColDesc) const = 0;
		virtual const cnt::SafeFullVector<Column>* getColumns() const = 0;		
		virtual bool isOk() const = 0;
		virtual bool areParamsBound() const = 0;
		virtual bool areColumnsBound() const = 0;
		//binds output columns without additional sql request 
		virtual bool prepareColumnsToExport(const std::set<td::String>& excludeColumns) = 0; //has to be called before exportXX calls
		virtual bool prepareColumnsToExport() = 0; //has to be called before exportXX calls, all columns will be exported
		//binds output columns with additional sql request
		virtual bool prepareColumnsToExport(const char* tableName, const std::set<td::String>& excludeColumns) = 0; //has to be called before exportXX calls
		//binds output columns using prepared column description
		virtual void prepareColumnsToExport(cnt::SafeFullVector<CPPColumnDesc>& cppColDesc) = 0; //needs to be called before exportXX calls
		virtual bool exportToXML(xml::Writer& w, const char* nodeName, int numberOfRows = 0) = 0; //exports the whole content, execute is called inside, numberOfRows - if 0 all rows will be exported, otherwise numberOfRows
		virtual void exportRowToXML(xml::Writer& w) = 0; ///exports single raw as attribs, user should call execute, and moveNext outside
		virtual void release() = 0;

		//only for SQLite provider
		virtual int getParamBindPos(const char* paramName) const = 0;

		virtual SecureInsertInfo getSecurityInsertInfo() = 0;

		//virtual IDatabase* getTranslationDB() = 0;

//		virtual IDatabase* getParentDB() = 0;

		virtual bool showLog(mu::ILogger::MsgType logType, const td::String& header, const td::String& msg) const = 0;
	};

	typedef mem::PointerReleaser<dp::IStatement> IStatementPtr;
}
