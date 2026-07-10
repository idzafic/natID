// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IOManip.h @brief I/O manipulator classes for column-major dense matrix element access. */
// interface for the General Matrix class
#pragma once
#include <td/Types.h>
#include <cassert>

namespace dense
{
/// @brief Read-only accessor for a full column-major matrix using 0-based (row, col) indexing.
template <typename T>
class MatrixReader
{
    const T* _pData; ///< Pointer to the matrix data in column-major order
    td::UINT4 _nRows; ///< Number of rows
    td::UINT4 _nCols; ///< Number of columns

public:
    /// @brief Default constructor — creates an invalid (null) reader.
    MatrixReader(): _pData(nullptr), _nRows(0), _nCols(0) {}
    /// @brief Constructs a reader bound to an existing data block.
    /// @param pData Pointer to the column-major data array.
    /// @param nRows Number of rows.
    /// @param nCols Number of columns.
    MatrixReader(const T* pData, td::UINT4 nRows, td::UINT4 nCols)
    : _pData(pData), _nRows(nRows), _nCols(nCols)
    {
        assert(_pData != nullptr);
    }

    /// @brief Copy assignment operator.
    /// @param r Source reader to copy from.
    void operator = (const MatrixReader& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
        _nCols = r._nCols;
    }

    /// @brief Returns a const reference to the element at (row, col).
    /// @param row 0-based row index.
    /// @param col 0-based column index.
    /// @return Const reference to the requested element.
    const T& operator () (td::UINT4 row, td::UINT4 col) const
    {
        assert( (_nRows > row) && (_nCols > col) );
        return _pData[col * _nRows + row];
    }
};

/// @brief Read-write accessor for a full column-major matrix using 0-based (row, col) indexing.
template <typename T>
class MatrixIO
{
    T* _pData;        ///< Pointer to the mutable matrix data in column-major order
    td::UINT4 _nRows; ///< Number of rows
    td::UINT4 _nCols; ///< Number of columns
public:
    /// @brief Default constructor — creates an invalid (null) I/O accessor.
    MatrixIO(): _pData(nullptr), _nRows(0), _nCols(0) {}
    /// @brief Constructs an I/O accessor bound to an existing data block.
    /// @param pData Pointer to the mutable column-major data array.
    /// @param nRows Number of rows.
    /// @param nCols Number of columns.
    MatrixIO(T* pData, td::UINT4 nRows, td::UINT4 nCols)
    : _pData(pData), _nRows(nRows), _nCols(nCols)
    {
        assert(_pData != nullptr);
    }

    /// @brief Copy assignment operator.
    /// @param r Source I/O accessor to copy from.
    void operator = (const MatrixIO& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
        _nCols = r._nCols;
    }

    /// @brief Returns a mutable reference to the element at (row, col).
    /// @param row 0-based row index.
    /// @param col 0-based column index.
    /// @return Mutable reference to the requested element.
    T& operator () (td::UINT4 row, td::UINT4 col)
    {
        assert( (_nRows > row) && (_nCols > col) );
        return _pData[col * _nRows + row];
    }
};

/// @brief Read-write accessor for a full column-major matrix using 1-based (row, col) indexing.
template <typename T>
class MatrixIO1
{
    T* _pData;        ///< Pointer to the mutable matrix data in column-major order
    td::UINT4 _nRows; ///< Number of rows
    td::UINT4 _nCols; ///< Number of columns
public:
    /// @brief Default constructor — creates an invalid (null) I/O accessor.
    MatrixIO1(): _pData(nullptr), _nRows(0), _nCols(0) {}
    /// @brief Constructs a 1-based I/O accessor bound to an existing data block.
    /// @param pData Pointer to the mutable column-major data array.
    /// @param nRows Number of rows.
    /// @param nCols Number of columns.
    MatrixIO1(T* pData, td::UINT4 nRows, td::UINT4 nCols)
    : _pData(pData), _nRows(nRows), _nCols(nCols)
    {
        assert(_pData != nullptr);
    }

    /// @brief Copy assignment operator.
    /// @param r Source accessor to copy from.
    void operator = (const MatrixIO1& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
        _nCols = r._nCols;
    }

    /// @brief Returns a mutable reference to the element at 1-based (row, col).
    /// @param row 1-based row index.
    /// @param col 1-based column index.
    /// @return Mutable reference to the requested element.
    T& operator () (td::UINT4 row, td::UINT4 col)
    {
        --row;
        --col;
        assert( (_nRows > row) && (_nCols > col) );
        return _pData[col * _nRows + row];
    }

    /// @brief Returns a const pointer to the underlying data array.
    /// @return Const pointer to the first element.
    const T* getData() const
    {
        return _pData;
    }

    /// @brief Returns the number of rows.
    /// @return Number of rows.
    td::UINT4 getNoOfRows() const
    {
        return _nRows;
    }

    /// @brief Returns the number of columns.
    /// @return Number of columns.
    td::UINT4 getNoOfCols() const
    {
        return _nCols;
    }
};

/// @brief Read-only accessor for a full column-major matrix using 1-based (row, col) indexing.
template <typename T>
class MatrixReader1
{
    const T* _pData; ///< Pointer to the matrix data in column-major order
    td::UINT4 _nRows; ///< Number of rows
    td::UINT4 _nCols; ///< Number of columns
public:
    /// @brief Default constructor — creates an invalid (null) reader.
    MatrixReader1(): _pData(nullptr), _nRows(0), _nCols(0) {}
    /// @brief Constructs a 1-based reader bound to an existing data block.
    /// @param pData Pointer to the column-major data array.
    /// @param nRows Number of rows.
    /// @param nCols Number of columns.
    MatrixReader1(const T* pData, td::UINT4 nRows, td::UINT4 nCols)
    : _pData(pData), _nRows(nRows), _nCols(nCols)
    {
        assert(_pData != nullptr);
    }

    /// @brief Copy assignment operator.
    /// @param r Source reader to copy from.
    void operator = (const MatrixReader1& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
        _nCols = r._nCols;
    }

    /// @brief Assigns from a MatrixIO1 accessor, sharing its data pointer.
    /// @param r Source MatrixIO1 accessor.
    void operator = (const MatrixIO1<T>& r)
    {
        _pData = r.getData();
        _nRows = r.getNoOfRows();
        _nCols = r.getNoOfCols();
    }

    /// @brief Returns a const reference to the element at 1-based (row, col).
    /// @param row 1-based row index.
    /// @param col 1-based column index.
    /// @return Const reference to the requested element.
    const T& operator () (td::UINT4 row, td::UINT4 col) const
    {
        --row;
        --col;
        assert( (_nRows > row) && (_nCols > col) );
        return _pData[col * _nRows + row];
    }
};



/// @brief Read-only accessor for a single column of a column-major matrix (0-based row indexing).
template <typename T>
class ColumnReader
{
    const T* _pData;  ///< Pointer to the full matrix data in column-major order
    td::UINT4 _nRows; ///< Number of rows in the matrix
    td::UINT4 _colPos;///< Current column index (0-based)
public:
    /// @brief Default constructor — creates an invalid reader.
    ColumnReader(): _pData(nullptr), _nRows(0), _colPos(0) {}
    /// @brief Constructs a column reader for a specific column.
    /// @param pData Pointer to the full matrix data array.
    /// @param nRows Number of rows.
    /// @param colPos 0-based column index.
    ColumnReader(const T* pData, td::UINT4 nRows, td::UINT4 colPos)
    : _pData(pData), _nRows(nRows), _colPos(colPos)
    {
        assert(_pData != nullptr);
    }

    /// @brief Copy assignment operator.
    /// @param r Source column reader.
    void operator = (const ColumnReader& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
        _colPos = r._colPos;
    }

    /// @brief Changes the active column.
    /// @param colPos New 0-based column index.
    void setColumn(td::UINT4 colPos)
    {
        _colPos = colPos;
    }

    /// @brief Returns a const reference to the element at the given row of the active column.
    /// @param row 0-based row index.
    /// @return Const reference to the element.
    const T& operator () (td::UINT4 row) const
    {
        assert(_nRows > row);
        return _pData[_colPos * _nRows + row];
    }
};

/// @brief Read-write accessor for a single column of a column-major matrix (0-based row indexing).
template <typename T>
class ColumnIO
{
    T* _pData;        ///< Pointer to the mutable matrix data in column-major order
    td::UINT4 _nRows; ///< Number of rows in the matrix
    td::UINT4 _colPos;///< Current column index (0-based)
public:
    /// @brief Default constructor — creates an invalid accessor.
    ColumnIO(): _pData(nullptr), _nRows(0), _colPos(0) {}
    /// @brief Constructs a mutable column accessor for a specific column.
    /// @param pData Pointer to the mutable matrix data array.
    /// @param nRows Number of rows.
    /// @param colPos 0-based column index.
    ColumnIO(T* pData, td::UINT4 nRows, td::UINT4 colPos)
    : _pData(pData), _nRows(nRows), _colPos(colPos)
    {
        assert(_pData != nullptr);
    }

    /// @brief Copy assignment operator.
    /// @param r Source column accessor.
    void operator = (const ColumnIO& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
        _colPos = r._colPos;
    }

    /// @brief Changes the active column.
    /// @param colPos New 0-based column index.
    void setColumn(td::UINT4 colPos)
    {
        _colPos = colPos;
    }

    /// @brief Returns a mutable reference to the element at the given row of the active column.
    /// @param row 0-based row index.
    /// @return Mutable reference to the element.
    T& operator () (td::UINT4 row)
    {
        assert(_nRows > row);
        return _pData[_colPos * _nRows + row];
    }
};

/// @brief Read-only accessor for the first column of a column-major matrix (0-based indexing).
template <typename T>
class FirstColumnReader
{
    const T* _pData;  ///< Pointer to the matrix data (first column starts at index 0)
    td::UINT4 _nRows; ///< Number of rows
public:
    /// @brief Default constructor — creates an invalid reader.
    FirstColumnReader(): _pData(nullptr), _nRows(0) {}
    /// @brief Constructs a first-column reader bound to an existing data block.
    /// @param pData Pointer to the matrix data array.
    /// @param nRows Number of rows.
    FirstColumnReader(const T* pData, td::UINT4 nRows)
    : _pData(pData), _nRows(nRows)
    {
        assert(_pData != nullptr);
    }

    /// @brief Copy assignment operator.
    /// @param r Source reader.
    void operator = (const FirstColumnReader& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
    }

    /// @brief Returns a const reference to the element at the given row of the first column.
    /// @param row 0-based row index.
    /// @return Const reference to the element.
    const T& operator () (td::UINT4 row) const
    {
        assert(_nRows > row);
        return _pData[row];
    }
};

/// @brief Read-write accessor for the first column of a column-major matrix (0-based indexing).
template <typename T>
class FirstColumnIO
{
    T* _pData;        ///< Pointer to the mutable matrix data (first column starts at index 0)
    td::UINT4 _nRows; ///< Number of rows
public:
    /// @brief Default constructor — creates an invalid accessor.
    FirstColumnIO(): _pData(nullptr), _nRows(0) {}
    /// @brief Constructs a mutable first-column accessor bound to an existing data block.
    /// @param pData Pointer to the mutable matrix data array.
    /// @param nRows Number of rows.
    FirstColumnIO(T* pData, td::UINT4 nRows)
    : _pData(pData), _nRows(nRows)
    {
        assert(_pData != nullptr);
    }

    /// @brief Copy assignment operator.
    /// @param r Source accessor.
    void operator = (const FirstColumnIO& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
    }

    /// @brief Returns a mutable reference to the element at the given row of the first column.
    /// @param row 0-based row index.
    /// @return Mutable reference to the element.
    T& operator () (td::UINT4 row)
    {
        assert(_nRows > row);
        return _pData[row];
    }
};

/// @brief Read-only accessor for a single column of a column-major matrix using 1-based indexing.
template <typename T>
class ColumnReader1
{
    const T* _pData;  ///< Pointer to the matrix data in column-major order
    td::UINT4 _nRows; ///< Number of rows
    td::UINT4 _colPos;///< Current 0-based column index (stored as colPos-1)
public:
    /// @brief Default constructor — creates an invalid reader.
    ColumnReader1(): _pData(nullptr), _nRows(0), _colPos(0) {}
    /// @brief Constructs a 1-based column reader for a specific column.
    /// @param pData Pointer to the matrix data array.
    /// @param nRows Number of rows.
    /// @param colPos 1-based column index.
    ColumnReader1(const T* pData, td::UINT4 nRows, td::UINT4 colPos)
    : _pData(pData), _nRows(nRows), _colPos(colPos-1)
    {
        assert(colPos > 0);
        assert(_pData != nullptr);
    }

    /// @brief Copy assignment operator.
    /// @param r Source column reader.
    void operator = (const ColumnReader1& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
        _colPos = r._colPos;
    }

    /// @brief Changes the active column using a 1-based index.
    /// @param colPos New 1-based column index.
    void setColumn(td::UINT4 colPos)
    {
        assert(colPos > 0);
        _colPos = colPos-1;
    }

    /// @brief Returns a const reference to the element at the given 1-based row.
    /// @param row 1-based row index.
    /// @return Const reference to the element.
    const T& operator () (int row) const
    {
        assert( (row > 0) && (_nRows >= row));
        return _pData[_colPos * _nRows + row - 1];
    }
};

/// @brief Read-write accessor for a single column of a column-major matrix using 1-based indexing.
template <typename T>
class ColumnIO1
{
    T* _pData;        ///< Pointer to the mutable matrix data in column-major order
    td::UINT4 _nRows; ///< Number of rows
    td::UINT4 _colPos;///< Current 0-based column index (stored as colPos-1)
public:
    /// @brief Default constructor — creates an invalid accessor.
    ColumnIO1(): _pData(nullptr), _nRows(0), _colPos(0) {}
    /// @brief Constructs a mutable 1-based column accessor for a specific column.
    /// @param pData Pointer to the mutable matrix data array.
    /// @param nRows Number of rows.
    /// @param colPos 1-based column index.
    ColumnIO1(T* pData, td::UINT4 nRows, td::UINT4 colPos)
    : _pData(pData), _nRows(nRows), _colPos(colPos-1)
    {
        assert(colPos > 0);
        assert(_pData != nullptr);
    }

    /// @brief Copy assignment operator.
    /// @param r Source column accessor.
    void operator = (const ColumnIO1& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
        _colPos = r._colPos;
    }

    /// @brief Changes the active column using a 1-based index.
    /// @param colPos New 1-based column index.
    void setColumn(td::UINT4 colPos)
    {
        assert(colPos > 0);
        _colPos = colPos-1;
    }

    /// @brief Returns a mutable reference to the element at the given 1-based row.
    /// @param row 1-based row index.
    /// @return Mutable reference to the element.
    T& operator () (int row)
    {
        assert( (row > 0) && (_nRows >= row));
        return _pData[_colPos * _nRows + row - 1];
    }
};

/// @brief Read-only accessor for the first column of a column-major matrix using 1-based indexing.
template <typename T>
class FirstColumnReader1
{
    const T* _pData;  ///< Pointer to the matrix data
    td::UINT4 _nRows; ///< Number of rows
public:
    /// @brief Default constructor — creates an invalid reader.
    FirstColumnReader1(): _pData(nullptr), _nRows(0) {}
    /// @brief Constructs a 1-based first-column reader.
    /// @param pData Pointer to the matrix data array.
    /// @param nRows Number of rows (must be > 0).
    FirstColumnReader1(const T* pData, td::UINT4 nRows)
    : _pData(pData), _nRows(nRows)
    {
        assert(nRows > 0);
        assert(_pData != nullptr);
    }

    /// @brief Copy assignment operator.
    /// @param r Source reader.
    void operator = (const FirstColumnReader1& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
    }

    /// @brief Returns a const reference to the element at the given 1-based row.
    /// @param row 1-based row index.
    /// @return Const reference to the element.
    const T& operator () (td::UINT4 row) const
    {
        assert( (row > 0) && (_nRows >= row));
        return _pData[row - 1];
    }
};

/// @brief Read-write accessor for the first column of a column-major matrix using 1-based indexing.
template <typename T>
class FirstColumnIO1
{
    T* _pData;        ///< Pointer to the mutable matrix data
    td::UINT4 _nRows; ///< Number of rows

public:
    /// @brief Default constructor — creates an invalid accessor.
    FirstColumnIO1(): _pData(nullptr), _nRows(0) {}
    /// @brief Constructs a mutable 1-based first-column accessor.
    /// @param pData Pointer to the mutable matrix data array.
    /// @param nRows Number of rows (must be > 0).
    FirstColumnIO1(T* pData, td::UINT4 nRows)
    : _pData(pData), _nRows(nRows)
    {
        assert(nRows > 0);
        assert(_pData != nullptr);
    }

    /// @brief Copy assignment operator.
    /// @param r Source accessor.
    void operator = (const FirstColumnIO1& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
    }

    /// @brief Returns a mutable reference to the element at the given 1-based row.
    /// @param row 1-based row index.
    /// @return Mutable reference to the element.
    T& operator () (td::UINT4 row)
    {
        assert( (row > 0) && (_nRows >= row));
        return _pData[row - 1];
    }
};

/// @brief Read-only accessor for a single row of a column-major matrix (0-based column indexing).
template <typename T>
class RowReader
{
    const T* _pData;  ///< Pointer to the matrix data in column-major order
    td::UINT4 _nRows; ///< Number of rows
    td::UINT4 _nCols; ///< Number of columns
    td::UINT4 _rowPos;///< Current row index (0-based)
public:
    /// @brief Default constructor — creates an invalid reader.
    RowReader(): _pData(nullptr), _nRows(0), _nCols(0), _rowPos(0) {}
    /// @brief Constructs a row reader for a specific row.
    /// @param pData Pointer to the matrix data array.
    /// @param nRows Number of rows.
    /// @param nCols Number of columns.
    /// @param rowPos 0-based row index.
    RowReader(const T* pData, td::UINT4 nRows, td::UINT4 nCols, td::UINT4 rowPos)
    : _pData(pData), _nRows(nRows), _nCols(nCols), _rowPos(rowPos)
    {
        assert(_pData != nullptr);
    }

    /// @brief Copy assignment operator.
    /// @param r Source row reader.
    void operator = (const RowReader& r)
    {
        _pData = r._pData;
        _nCols = r._nCols;
        _rowPos = r._rowPos;
    }

    /// @brief Changes the active row.
    /// @param rowPos New 0-based row index.
    void setRow(td::UINT4 rowPos)
    {
        _rowPos = rowPos;
    }

    /// @brief Returns a const reference to the element at the given column of the active row.
    /// @param col 0-based column index.
    /// @return Const reference to the element.
    const T& operator () (td::UINT4 col) const
    {
        assert(_nCols > col);
        return _pData[col * _nRows + _rowPos];
    }
};

/// @brief Read-only accessor for diagonal elements using 0-based indexing.
template <typename T>
class DiagReader
{
    const T* _pData; ///< Pointer to the diagonal data array
    td::UINT4 _size; ///< Number of diagonal elements
public:
    /// @brief Default constructor — creates an invalid reader.
    DiagReader(): _pData(nullptr), _size(0) {}
    /// @brief Constructs a diagonal reader bound to an existing data block.
    /// @param pData Pointer to the diagonal data array.
    /// @param size Number of diagonal elements.
    DiagReader(const T* pData, td::UINT4 size)
    : _pData(pData), _size(size)
    {
        assert(_pData != nullptr);
    }

    /// @brief Copy assignment operator.
    /// @param r Source diagonal reader.
    void operator = (const DiagReader& r)
    {
        _pData = r._pData;
        _size = r._size;
    }

    /// @brief Returns a const reference to the diagonal element at position posDiag.
    /// @param posDiag 0-based diagonal element index.
    /// @return Const reference to the element.
    const T& operator () (td::UINT4 posDiag) const
    {
        assert(_size > posDiag);
        return _pData[posDiag];
    }
};

/// @brief Read-write accessor for diagonal elements using 0-based indexing.
template <typename T>
class DiagIO
{
    T* _pData;       ///< Pointer to the mutable diagonal data array
    td::UINT4 _size; ///< Number of diagonal elements
public:
    /// @brief Default constructor — creates an invalid accessor.
    DiagIO(): _pData(nullptr), _size(0) {}
    /// @brief Constructs a mutable diagonal accessor bound to an existing data block.
    /// @param pData Pointer to the mutable diagonal data array.
    /// @param size Number of diagonal elements.
    DiagIO(T* pData, td::UINT4 size)
    : _pData(pData), _size(size)
    {
        assert(_pData != nullptr);
    }

    /// @brief Copy assignment operator.
    /// @param r Source diagonal accessor.
    void operator = (const DiagIO& r)
    {
        _pData = r._pData;
        _size = r._size;
    }

    /// @brief Returns a mutable reference to the diagonal element at position posDiag.
    /// @param posDiag 0-based diagonal element index.
    /// @return Mutable reference to the element.
    T& operator () (td::UINT4 posDiag)
    {
        assert(_size > posDiag);
        return _pData[posDiag];
    }
};

/// @brief Read-only accessor for diagonal elements using 1-based indexing.
template <typename T>
class DiagReader1
{
    const T* _pData; ///< Pointer to the diagonal data array
    td::UINT4 _size; ///< Number of diagonal elements
public:
    /// @brief Default constructor — creates an invalid reader.
    DiagReader1(): _pData(nullptr), _size(0) {}
    /// @brief Constructs a 1-based diagonal reader bound to an existing data block.
    /// @param pData Pointer to the diagonal data array.
    /// @param size Number of diagonal elements.
    DiagReader1(const T* pData, td::UINT4 size)
    : _pData(pData), _size(size)
    {
        assert(_pData != nullptr);
    }

    /// @brief Copy assignment operator.
    /// @param r Source diagonal reader.
    void operator = (const DiagReader1& r)
    {
        _pData = r._pData;
        _size = r._size;
    }

    /// @brief Returns a const reference to the diagonal element at 1-based position posDiag.
    /// @param posDiag 1-based diagonal element index.
    /// @return Const reference to the element.
    const T& operator () (td::UINT4 posDiag) const
    {
        assert( (posDiag > 0) && (_size >= posDiag) );
        return _pData[posDiag - 1];
    }
};

/// @brief Read-write accessor for diagonal elements using 1-based indexing.
template <typename T>
class DiagIO1
{
    T* _pData;       ///< Pointer to the mutable diagonal data array
    td::UINT4 _size; ///< Number of diagonal elements
public:
    /// @brief Default constructor — creates an invalid accessor.
    DiagIO1(): _pData(nullptr), _size(0) {}
    /// @brief Constructs a mutable 1-based diagonal accessor bound to an existing data block.
    /// @param pData Pointer to the mutable diagonal data array.
    /// @param size Number of diagonal elements.
    DiagIO1(T* pData, td::UINT4 size)
    : _pData(pData), _size(size)
    {
        assert(_pData != nullptr);
    }

    /// @brief Copy assignment operator.
    /// @param r Source diagonal accessor.
    void operator = (const DiagIO1& r)
    {
        _pData = r._pData;
        _size = r._size;
    }

    /// @brief Returns a mutable reference to the diagonal element at 1-based position posDiag.
    /// @param posDiag 1-based diagonal element index.
    /// @return Mutable reference to the element.
    T& operator () (td::UINT4 posDiag)
    {
        assert( (posDiag > 0) && (_size >= posDiag) );
        return _pData[posDiag-1];
    }
};

}
