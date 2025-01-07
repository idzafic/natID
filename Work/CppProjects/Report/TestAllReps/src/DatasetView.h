#pragma once
#include <vector>
#include <tuple>
#include <dp/IDataSet.h>

namespace dp
{

	class KeyView
	{
		dp::IDataSet* _pDS;
		std::vector<size_t> _indices;
		std::vector<int> _columns;

		template<class Var>
		bool checkRowColumn(const IDataSet::Row dsRow, int col, const Var& var) const
		{
			return (dsRow[col] == var);
		}

		template <class Tuple, std::size_t... IS>
		bool checkRow(const IDataSet::Row dsRow, Tuple&& key, std::index_sequence<IS...>)
		{
			return (checkRowColumn(dsRow, _columns[IS], std::get<IS>(key)) && ...);
		}

	public:
		KeyView(dp::IDataSet* pDS)
			: _pDS(pDS)
		{
			_indices.reserve(_pDS->getNumberOfRows());
		}
		KeyView(dp::IDataSetPtr pDS)
			: _pDS(pDS.getPtr())
		{
			_indices.reserve(_pDS->getNumberOfRows());
		}

		KeyView& operator<<(const td::String& colName)
		{
			assert(_pDS && "Dataset is null");
			int colPos = _pDS->getColIndex(colName);
			assert(colPos >= 0 && "Column doesent exist");

			_columns.push_back(colPos);
			return *this;
		}

		KeyView& operator<<(int colPos)
		{
			assert(_pDS && "Dataset is null");
			assert(colPos >= 0 && "Column doesent exist");
			assert(colPos < (int)_pDS->getNumberOfCols() && "Column doesent exist");

			_columns.push_back(colPos);
			return *this;
		}

		template<class... Args>
		void prepare(Args&&... args)
		{
			auto key = std::make_tuple(args...);
			auto rowCnt = _pDS->getNumberOfRows();
			for (size_t row = 0; row < rowCnt; ++row)
			{
				auto dsRow = _pDS->getRow(row);
				if (checkRow(dsRow, key, std::make_index_sequence<sizeof...(Args)>()))
					_indices.push_back(row);
				
			}
		}

		size_t getNumberOfRows() const
		{
			return _indices.size();
		}

		IDataSet::Row getRow(size_t row)
		{
			assert(row >= 0 && row < getNumberOfRows() && "Invalid row");
			return _pDS->getRow(_indices[row]);
		}

	};

	class SortedView
	{

	};
}
