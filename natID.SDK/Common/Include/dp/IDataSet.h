// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/Variant.h>
#include <cnt/SafeFullVector.h>
#include <mem/PointerReleaser.h>
#include <mu/ILogger.h>
#include <dp/IFilter.h>
#include <dp/ISort.h>
#include <dp/IExternLoader.h>
#include <dp/Param.h>
#include <dp/DSColumns.h>
//#include <dp/IDataSetDelegate.h>
#include <functional>
#include <cnt/Wrapper.h>

namespace dp
{	
	class IDatabase;
    class IDataSetDelegate;
    using RowWrapper = cnt::Wrapper<td::Variant>;

	class IDataSet
	{	
	public:
        enum class Size : td::BYTE { SingleRow, Small, Medium, Large } ;

		enum class Execution : td::UINT4 { EX_SINGLE = 0x00000010, EX_MULT = 0x00000020, EX_NO = 0x00000040 };

		using Row = cnt::SafeFullVector<td::Variant, false>&;

		virtual int getColIndex(const td::String& colName) const =0; //returns -1 if falls

		virtual size_t getNumberOfRows() const =0;

		virtual size_t getNumberOfCols() const =0;

		virtual size_t getNumberOfParams() const = 0;

		virtual size_t getCurrentRowNo() const = 0;

        virtual size_t getPreviousNumberOfRows() const = 0;

		virtual int getColumnPos(const char* columnName) const = 0;

		virtual int getColumnPos(const td::String& columnName) const = 0;

		virtual const td::String& getColName(size_t cIndex) const = 0;

		virtual void removeAll() = 0;

		virtual bool removeRow(size_t row) = 0;

		virtual bool insertRow(size_t row) = 0;

		virtual bool updateRow(size_t row) = 0;
        
        virtual bool updateColumnInRow(size_t row, size_t iCol, bool callDelegate = false) = 0;
        
        virtual bool updateColumnInCurrentRow(size_t iCol, bool callDelegate = false) = 0;

		virtual bool push_back() = 0;
		
		virtual IDataSet::Row getEmptyRow() = 0;
        
        virtual void getErrorStr(td::String& strErr) const = 0;
        
        virtual td::String getErrorStr() const = 0;

        [[nodiscard]] virtual IDataSet::Row getRow(size_t row) = 0;

		virtual bool setCurrentRow(size_t row) = 0;

        [[nodiscard]] virtual IDataSet::Row getCurrentRow() = 0;
        
        virtual bool getRow(size_t row, dp::RowWrapper& rowWrapper) = 0;
        
        virtual bool initRowWrapper(dp::RowWrapper& rowWrapper) = 0;

		//virtual void search(const td::String& startsText, const td::String& containsText) = 0;

		//virtual void clearSearch() = 0;

		///returns true if val is found in column columnPos, startPos will contain row position of first val appereance
		///startPos is input/output parameter
		virtual bool findFirst(const td::Variant& val, size_t columnPos, size_t& startPos) = 0;

		///this method uses first keySize columns (keys) to search for
		virtual bool find(const td::Variant* keys, size_t keySize, size_t& startPos) = 0;

		//get total number of records with specified keys
		virtual size_t getForeignKeyCount(const td::Variant* keys, size_t keySize, size_t& startPos) = 0;

		///this method uses first column as key and finds it (equivalent to find(&key, 1, startPos) from above)
		virtual bool find(const td::Variant& key, size_t& startPos) = 0;

		virtual void markChanged(const td::Variant&  id) = 0;

		virtual void unmarkChanged(const td::Variant&  id) = 0;

		virtual bool isChanged(const td::Variant&  id) const = 0;

		virtual const std::set<td::Variant>& getChangedSet() const = 0;

		virtual const cnt::SafeFullVector<dp::DSColumn>& getColumnDescriptions() const = 0;

		virtual const dp::DSColumn& getColumnDescription(size_t columnPos) const = 0;

		virtual bool setParam(size_t posParam, const td::Variant& val) = 0;		

		virtual const td::Variant& getParam(size_t posParam) const = 0;

		virtual const cnt::SafeFullVector<td::Variant>* getCurrentParams() const = 0;

		virtual const mu::ILogger* getLogger() const = 0;

		virtual bool showLog(mu::ILogger::MsgType, const td::String& header, const td::String& msg) const = 0;

		virtual bool exportToXML(const td::String& fileName, const td::String& strID, bool checkStrings = false) = 0;
        
        virtual bool exportToTXT(const td::String& fileName) = 0;

		virtual td::INT4 importFromXML(const td::String& fileName) = 0;  //returns number of inserted rows, neg number for not ok

		virtual void duplicate(dp::IDataSet* pDS, dp::IFilter* pFilter) = 0;

        virtual dp::IDataSet* duplicate(dp::IFilter* pFilter) = 0;
        
		virtual void remove(dp::IFilter* pFilter) = 0;

		virtual void push_back(const IDataSet* pDS) = 0;

		virtual void sort(dp::ISort* pSort) = 0;
        
        virtual void setUserInfo(const td::Variant& val) = 0;
        virtual const td::Variant& getUserInfo() const = 0;
		
		virtual IDatabase* getDatabase() const = 0;

		virtual bool isOK() const = 0;

		virtual void addRef() = 0;

		virtual void release() = 0;

		virtual void releaseStatements() = 0;

		virtual void getSum(int colPos, td::Variant& sumOut) = 0;

		virtual bool getSum(const char* columnName, td::Variant& sumOut) = 0;

		//for console output
		virtual void showRow(std::ostream& o, size_t row) = 0;
		virtual void show(std::ostream& o) = 0;

		//for internal use
		virtual bool setFeature(td::LUINT8 feature) = 0;
        //for internal use - use it carefully!
        virtual bool getValue(size_t row, size_t column, td::Variant& val) = 0;

		//virtual bool populateView(cnt::SafeFullVector<td::WORD>& view, int viewPos) = 0;

		//virtual void setExternLoader(dp::IExternLoader* pExtLoader) = 0;

		//browsing interface
		virtual cnt::SafeFullVector<dp::Param>& allocParams() = 0;
		virtual cnt::SafeFullVector<dp::DSColumn>& allocBindColumns(size_t nColumns) = 0;
		virtual bool execute() = 0;
		virtual bool moveNext() = 0;
		virtual bool moveToPos(size_t iPos) = 0;
        
        virtual bool isConnectionless() const = 0;
        
        virtual void setDelegate(IDataSetDelegate* pDelegate, bool deleteWithDataSet = false) = 0;
        
        virtual void getFPValues(const cnt::PushBackVector<td::String>& columns, cnt::PushBackVector<cnt::PushBackVector<double>>& data) = 0;

        //event handlers
        virtual void onRemoveAll(std::function<void(dp::IDataSet*)>) = 0;
        virtual void onUpdate(std::function<void(dp::IDataSet*, size_t)>) = 0;
        virtual void onInsert(std::function<void(dp::IDataSet*, size_t)>) = 0;

	};

	typedef mem::PointerReleaser<dp::IDataSet> IDataSetPtr;
    
}
