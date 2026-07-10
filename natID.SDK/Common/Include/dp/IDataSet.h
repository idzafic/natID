// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IDataSet.h
    @brief Abstract interface for a generic data set with row/column access, filtering, sorting, and persistence. */
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
    using RowWrapper = cnt::Wrapper<td::Variant>; ///< Convenience alias for a row wrapped in a cnt::Wrapper.

	/// @brief Abstract interface representing a data set that supports row manipulation, querying, and I/O operations.
	class IDataSet
	{
	public:
        /// @brief Indicates the expected size category of the data set.
        enum class Size : td::BYTE { SingleRow, ///< Data set holds a single row.
                                     Small,      ///< Small number of rows.
                                     Medium,     ///< Medium number of rows.
                                     Large       ///< Large number of rows.
                                   } ;

		/// @brief Controls the execution mode of a data set query.
		enum class Execution : td::UINT4 { EX_SINGLE = 0x00000010, ///< Execute to fetch a single result set.
		                                   EX_MULT = 0x00000020,   ///< Execute to fetch multiple result sets.
		                                   EX_NO = 0x00000040      ///< No execution (non-query statement).
		                                 };

		using Row = cnt::SafeFullVector<td::Variant, false>&; ///< Alias for a reference to a row of variant values.

		/// @brief Returns the column index for the given column name.
		/// @param colName The name of the column to look up.
		/// @return Zero-based column index, or -1 if the column is not found.
		virtual int getColIndex(const td::String& colName) const =0; //returns -1 if falls

		/// @brief Returns the total number of rows in the data set.
		/// @return Number of rows currently held.
		virtual size_t getNumberOfRows() const =0;

		/// @brief Returns the total number of columns in the data set.
		/// @return Number of columns.
		virtual size_t getNumberOfCols() const =0;

		/// @brief Returns the total number of query parameters.
		/// @return Number of bound parameters.
		virtual size_t getNumberOfParams() const = 0;

		/// @brief Returns the zero-based index of the current row.
		/// @return Index of the current row.
		virtual size_t getCurrentRowNo() const = 0;

        /// @brief Returns the number of rows from the previous execution.
        /// @return Row count before the most recent data refresh.
        virtual size_t getPreviousNumberOfRows() const = 0;

		/// @brief Returns the column position by name using a C-string.
		/// @param columnName Null-terminated column name.
		/// @return Zero-based column position, or -1 if not found.
		virtual int getColumnPos(const char* columnName) const = 0;

		/// @brief Returns the column position by name using a String object.
		/// @param columnName The column name to search for.
		/// @return Zero-based column position, or -1 if not found.
		virtual int getColumnPos(const td::String& columnName) const = 0;

		/// @brief Returns the name of the column at the given index.
		/// @param cIndex Zero-based column index.
		/// @return Reference to the column name string.
		virtual const td::String& getColName(size_t cIndex) const = 0;

		/// @brief Removes all rows from the data set.
		virtual void removeAll() = 0;

		/// @brief Removes the row at the specified position.
		/// @param row Zero-based index of the row to remove.
		/// @return True if the row was successfully removed.
		virtual bool removeRow(size_t row) = 0;

		/// @brief Inserts a new empty row at the specified position.
		/// @param row Zero-based index at which to insert the new row.
		/// @return True if the row was successfully inserted.
		virtual bool insertRow(size_t row) = 0;

		/// @brief Persists any changes made to the row at the specified position.
		/// @param row Zero-based index of the row to update.
		/// @return True if the update was successful.
		virtual bool updateRow(size_t row) = 0;

        /// @brief Updates a single column in the specified row.
        /// @param row Zero-based row index.
        /// @param iCol Zero-based column index.
        /// @param callDelegate Whether to invoke the delegate callback after the update.
        /// @return True if the column was successfully updated.
        virtual bool updateColumnInRow(size_t row, size_t iCol, bool callDelegate = false) = 0;

        /// @brief Updates a single column in the currently active row.
        /// @param iCol Zero-based column index.
        /// @param callDelegate Whether to invoke the delegate callback after the update.
        /// @return True if the column was successfully updated.
        virtual bool updateColumnInCurrentRow(size_t iCol, bool callDelegate = false) = 0;

		/// @brief Appends the contents of the empty row buffer as a new row.
		/// @return True if the row was successfully appended.
		virtual bool push_back() = 0;

		/// @brief Returns a reference to the shared empty row buffer for staging new data.
		/// @return Reference to the empty row.
        virtual IDataSet::Row getEmptyRow() = 0;

        /// @brief Fills the provided string with a description of the last error.
        /// @param strErr Output string that receives the error description.
        virtual void getErrorStr(td::String& strErr) const = 0;

        /// @brief Returns a string describing the last error.
        /// @return String containing the error description.
        virtual td::String getErrorStr() const = 0;

        /// @brief Returns a reference to the row at the specified position.
        /// @param row Zero-based row index.
        /// @return Reference to the requested row's data.
        [[nodiscard]] virtual IDataSet::Row getRow(size_t row) = 0;

		/// @brief Sets the current row pointer to the given position.
		/// @param row Zero-based row index to make current.
		/// @return True if the position is valid and the cursor was moved.
		virtual bool setCurrentRow(size_t row) = 0;

        /// @brief Returns a reference to the currently active row.
        /// @return Reference to the current row's data.
        [[nodiscard]] virtual IDataSet::Row getCurrentRow() = 0;

        /// @brief Copies the data of the specified row into a RowWrapper.
        /// @param row Zero-based row index.
        /// @param rowWrapper Output wrapper that receives the row data.
        /// @return True if the row data was successfully retrieved.
        virtual bool getRow(size_t row, dp::RowWrapper& rowWrapper) = 0;

        /// @brief Initialises a RowWrapper with the correct number of columns and default values.
        /// @param rowWrapper The RowWrapper to initialise.
        /// @return True if initialisation succeeded.
        virtual bool initRowWrapper(dp::RowWrapper& rowWrapper) = 0;

		//virtual void search(const td::String& startsText, const td::String& containsText) = 0;

		//virtual void clearSearch() = 0;

		///returns true if val is found in column columnPos, startPos will contain row position of first val appereance
		///startPos is input/output parameter
		/// @brief Searches for the first occurrence of a value in the specified column.
		/// @param val The value to search for.
		/// @param columnPos Zero-based column index to search within.
		/// @param startPos Input/output: starting position for the search; receives the index of the first match.
		/// @return True if the value was found.
		virtual bool findFirst(const td::Variant& val, size_t columnPos, size_t& startPos) = 0;

		///this method uses first keySize columns (keys) to search for
		/// @brief Finds a row whose leading key columns match the supplied key values.
		/// @param keys Pointer to an array of key values.
		/// @param keySize Number of key columns to match.
		/// @param startPos Output: zero-based index of the first matching row.
		/// @return True if a matching row was found.
		virtual bool find(const td::Variant* keys, size_t keySize, size_t& startPos) = 0;

		//get total number of records with specified keys
		/// @brief Returns the count of rows that match the given key values.
		/// @param keys Pointer to an array of key values.
		/// @param keySize Number of key columns to match.
		/// @param startPos Output: position of the first matching row.
		/// @return Number of rows matching the specified keys.
		virtual size_t getForeignKeyCount(const td::Variant* keys, size_t keySize, size_t& startPos) = 0;

		///this method uses first column as key and finds it (equivalent to find(&key, 1, startPos) from above)
		/// @brief Finds a row whose first column matches the supplied key value.
		/// @param key The key value to search for in the first column.
		/// @param startPos Output: zero-based index of the first matching row.
		/// @return True if a matching row was found.
		virtual bool find(const td::Variant& key, size_t& startPos) = 0;

		/// @brief Marks the row identified by the given ID as changed.
		/// @param id The identifier of the row to mark.
		virtual void markChanged(const td::Variant&  id) = 0;

		/// @brief Clears the changed mark for the row identified by the given ID.
		/// @param id The identifier of the row to unmark.
		virtual void unmarkChanged(const td::Variant&  id) = 0;

		/// @brief Checks whether the row identified by the given ID has been marked as changed.
		/// @param id The identifier of the row to check.
		/// @return True if the row is marked as changed.
		virtual bool isChanged(const td::Variant&  id) const = 0;

		/// @brief Returns the set of IDs that have been marked as changed.
		/// @return Constant reference to the set of changed row identifiers.
		virtual const std::set<td::Variant>& getChangedSet() const = 0;

		/// @brief Returns a vector of column descriptor objects for all columns.
		/// @return Constant reference to the column description vector.
		virtual const cnt::SafeFullVector<dp::DSColumn>& getColumnDescriptions() const = 0;

		/// @brief Returns the column descriptor for the column at the given position.
		/// @param columnPos Zero-based column index.
		/// @return Constant reference to the column descriptor.
		virtual const dp::DSColumn& getColumnDescription(size_t columnPos) const = 0;

		/// @brief Sets the value of the query parameter at the given position.
		/// @param posParam Zero-based parameter index.
		/// @param val The value to assign to the parameter.
		/// @return True if the parameter was successfully set.
		virtual bool setParam(size_t posParam, const td::Variant& val) = 0;

		/// @brief Returns the value of the query parameter at the given position.
		/// @param posParam Zero-based parameter index.
		/// @return Constant reference to the parameter value.
		virtual const td::Variant& getParam(size_t posParam) const = 0;

		/// @brief Returns a pointer to the current set of query parameter values.
		/// @return Pointer to the vector of current parameter values, or nullptr if none.
		virtual const cnt::SafeFullVector<td::Variant>* getCurrentParams() const = 0;

		/// @brief Returns the logger associated with this data set.
		/// @return Pointer to the ILogger instance used for logging.
		virtual const mu::ILogger* getLogger() const = 0;

		/// @brief Logs a message with the given type, header, and body.
		/// @param msgType The severity or category of the log message.
		/// @param header Short description or title of the log entry.
		/// @param msg Detailed log message body.
		/// @return True if the message was successfully logged.
		virtual bool showLog(mu::ILogger::MsgType, const td::String& header, const td::String& msg) const = 0;

		/// @brief Exports the data set contents to an XML file.
		/// @param fileName Path to the output XML file.
		/// @param strID String identifier embedded in the XML output.
		/// @param checkStrings If true, performs string validation during export.
		/// @return True if the export succeeded.
		virtual bool exportToXML(const td::String& fileName, const td::String& strID, bool checkStrings = false) = 0;

        /// @brief Exports the data set contents to a plain-text file.
        /// @param fileName Path to the output text file.
        /// @return True if the export succeeded.
        virtual bool exportToTXT(const td::String& fileName) = 0;

		/// @brief Imports rows from an XML file into the data set.
		/// @param fileName Path to the XML file to import.
		/// @return Number of rows inserted, or a negative value on failure.
		virtual td::INT4 importFromXML(const td::String& fileName) = 0;  //returns number of inserted rows, neg number for not ok

		/// @brief Copies rows from this data set that pass the filter into the destination data set.
		/// @param pDS Target data set to receive the duplicated rows.
		/// @param pFilter Optional filter; only matching rows are copied.
		virtual void duplicate(dp::IDataSet* pDS, dp::IFilter* pFilter) = 0;

        /// @brief Creates and returns a new data set containing rows that pass the given filter.
        /// @param pFilter Optional filter to apply during duplication.
        /// @return Pointer to the newly created data set containing matching rows.
        virtual dp::IDataSet* duplicate(dp::IFilter* pFilter) = 0;

		/// @brief Removes all rows that match the given filter criteria.
		/// @param pFilter The filter used to select rows for removal.
		virtual void remove(dp::IFilter* pFilter) = 0;

		/// @brief Appends all rows from the given data set to this data set.
		/// @param pDS Pointer to the source data set whose rows will be appended.
		virtual void push_back(const IDataSet* pDS) = 0;

		/// @brief Sorts the rows of the data set using the supplied sort comparator.
		/// @param pSort Pointer to the sort strategy object.
		virtual void sort(dp::ISort* pSort) = 0;

        /// @brief Stores an arbitrary user-defined value associated with the data set.
        /// @param val The value to store as user information.
        virtual void setUserInfo(const td::Variant& val) = 0;
        /// @brief Retrieves the user-defined value previously stored via setUserInfo.
        /// @return Constant reference to the stored user information value.
        virtual const td::Variant& getUserInfo() const = 0;

		/// @brief Returns the database connection associated with this data set.
		/// @return Pointer to the IDatabase instance, or nullptr if not connected.
		virtual IDatabase* getDatabase() const = 0;

		/// @brief Returns whether the data set is in a valid, operational state.
		/// @return True if the data set is OK and ready to use.
		virtual bool isOK() const = 0;

		/// @brief Increments the reference count of this data set.
		virtual void addRef() = 0;

		/// @brief Decrements the reference count and destroys the data set when it reaches zero.
		virtual void release() = 0;

		/// @brief Releases any underlying database statements held by the data set.
		virtual void releaseStatements() = 0;

		/// @brief Computes the sum of values in the specified column by position.
		/// @param colPos Zero-based column index.
		/// @param sumOut Output variant that receives the computed sum.
		virtual void getSum(int colPos, td::Variant& sumOut) = 0;

		/// @brief Computes the sum of values in the column identified by name.
		/// @param columnName Null-terminated column name.
		/// @param sumOut Output variant that receives the computed sum.
		/// @return True if the column was found and the sum computed successfully.
		virtual bool getSum(const char* columnName, td::Variant& sumOut) = 0;

		//for console output
		/// @brief Writes the contents of a single row to the output stream.
		/// @param o Output stream to write to.
		/// @param row Zero-based index of the row to display.
		virtual void showRow(std::ostream& o, size_t row) = 0;
		/// @brief Writes all rows of the data set to the output stream.
		/// @param o Output stream to write to.
		virtual void show(std::ostream& o) = 0;

		//for internal use
		/// @brief Sets an internal feature flag on the data set.
		/// @param feature Bit-flag value representing the feature to enable.
		/// @return True if the feature was successfully applied.
		virtual bool setFeature(td::LUINT8 feature) = 0;
        //for internal use - use it carefully!
        /// @brief Retrieves the value of a specific cell by row and column index.
        /// @param row Zero-based row index.
        /// @param column Zero-based column index.
        /// @param val Output variant that receives the cell value.
        /// @return True if the value was successfully retrieved.
        virtual bool getValue(size_t row, size_t column, td::Variant& val) = 0;

		//virtual bool populateView(cnt::SafeFullVector<td::WORD>& view, int viewPos) = 0;

		//virtual void setExternLoader(dp::IExternLoader* pExtLoader) = 0;

		//browsing interface
		/// @brief Allocates and returns the parameter list for binding query parameters.
		/// @return Reference to the allocated parameter vector.
		virtual cnt::SafeFullVector<dp::Param>& allocParams() = 0;
		/// @brief Allocates and returns the column binding list for the specified number of columns.
		/// @param nColumns Number of columns to allocate binding descriptors for.
		/// @return Reference to the allocated column descriptor vector.
		virtual cnt::SafeFullVector<dp::DSColumn>& allocBindColumns(size_t nColumns) = 0;
		/// @brief Executes the data set query using the current parameters.
		/// @return True if execution succeeded.
		virtual bool execute() = 0;
		/// @brief Advances the cursor to the next row in the result set.
		/// @return True if there is a next row; false when all rows have been consumed.
		virtual bool moveNext() = 0;
		/// @brief Moves the cursor to the specified row position.
		/// @param iPos Zero-based row index to move to.
		/// @return True if the position is valid and the cursor was moved.
		virtual bool moveToPos(size_t iPos) = 0;

        /// @brief Returns whether this data set operates without a live database connection.
        /// @return True if the data set is connectionless (in-memory only).
        virtual bool isConnectionless() const = 0;

        /// @brief Assigns a delegate object that receives event callbacks from the data set.
        /// @param pDelegate Pointer to the delegate to register.
        /// @param deleteWithDataSet If true, the delegate will be deleted when the data set is destroyed.
        virtual void setDelegate(IDataSetDelegate* pDelegate, bool deleteWithDataSet = false) = 0;

        /// @brief Extracts double-precision values from the specified columns into parallel vectors.
        /// @param columns Names of the columns to extract.
        /// @param data Output container of per-column double vectors.
        virtual void getFPValues(const cnt::PushBackVector<td::String>& columns, cnt::PushBackVector<cnt::PushBackVector<double>>& data) = 0;

        //event handlers
        /// @brief Registers a callback invoked when all rows are removed from the data set.
        /// @param callback Function called with a pointer to this data set after removeAll.
        virtual void onRemoveAll(std::function<void(dp::IDataSet*)>) = 0;
        /// @brief Registers a callback invoked when a row is updated.
        /// @param callback Function called with a pointer to this data set and the updated row index.
        virtual void onUpdate(std::function<void(dp::IDataSet*, size_t)>) = 0;
        /// @brief Registers a callback invoked when a new row is inserted.
        /// @param callback Function called with a pointer to this data set and the new row index.
        virtual void onInsert(std::function<void(dp::IDataSet*, size_t)>) = 0;

	};

	typedef mem::PointerReleaser<dp::IDataSet> IDataSetPtr; ///< Smart pointer type that calls release() on destruction.

}
