// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

// interface for the General Matrix class
#pragma once
#include <td/Types.h>
#include <cassert>

namespace dense
{
template <typename T>
class MatrixReader
{
    const T* _pData;
    td::UINT4 _nRows;
    td::UINT4 _nCols;
    
public:
    MatrixReader(): _pData(nullptr), _nRows(0), _nCols(0) {}
    MatrixReader(const T* pData, td::UINT4 nRows, td::UINT4 nCols)
    : _pData(pData), _nRows(nRows), _nCols(nCols)
    {
        assert(_pData != nullptr);
    }
    
    void operator = (const MatrixReader& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
        _nCols = r._nCols;
    }

    const T& operator () (td::UINT4 row, td::UINT4 col) const
    {
        assert( (_nRows > row) && (_nCols > col) );
        return _pData[col * _nRows + row];
    }
};

template <typename T>
class MatrixIO
{
    T* _pData;
    td::UINT4 _nRows;
    td::UINT4 _nCols;
public:
    MatrixIO(): _pData(nullptr), _nRows(0), _nCols(0) {}
    MatrixIO(T* pData, td::UINT4 nRows, td::UINT4 nCols)
    : _pData(pData), _nRows(nRows), _nCols(nCols)
    {
        assert(_pData != nullptr);
    }
    
    void operator = (const MatrixIO& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
        _nCols = r._nCols;
    }
    
    T& operator () (td::UINT4 row, td::UINT4 col)
    {
        assert( (_nRows > row) && (_nCols > col) );
        return _pData[col * _nRows + row];
    }
};

template <typename T>
class MatrixIO1
{
    T* _pData;
    td::UINT4 _nRows;
    td::UINT4 _nCols;
public:
    MatrixIO1(): _pData(nullptr), _nRows(0), _nCols(0) {}
    MatrixIO1(T* pData, td::UINT4 nRows, td::UINT4 nCols)
    : _pData(pData), _nRows(nRows), _nCols(nCols)
    {
        assert(_pData != nullptr);
    }
    
    void operator = (const MatrixIO1& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
        _nCols = r._nCols;
    }
    
    T& operator () (td::UINT4 row, td::UINT4 col)
    {
        --row;
        --col;
        assert( (_nRows > row) && (_nCols > col) );
        return _pData[col * _nRows + row];
    }
    
    const T* getData() const
    {
        return _pData;
    }
    
    td::UINT4 getNoOfRows() const
    {
        return _nRows;
    }
    
    td::UINT4 getNoOfCols() const
    {
        return _nCols;
    }
};

template <typename T>
class MatrixReader1
{
    const T* _pData;
    td::UINT4 _nRows;
    td::UINT4 _nCols;
public:
    MatrixReader1(): _pData(nullptr), _nRows(0), _nCols(0) {}
    MatrixReader1(const T* pData, td::UINT4 nRows, td::UINT4 nCols)
    : _pData(pData), _nRows(nRows), _nCols(nCols)
    {
        assert(_pData != nullptr);
    }

    void operator = (const MatrixReader1& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
        _nCols = r._nCols;
    }
    
    void operator = (const MatrixIO1<T>& r)
    {
        _pData = r.getData();
        _nRows = r.getNoOfRows();
        _nCols = r.getNoOfCols();
    }
    
    const T& operator () (td::UINT4 row, td::UINT4 col) const
    {
        --row;
        --col;
        assert( (_nRows > row) && (_nCols > col) );
        return _pData[col * _nRows + row];
    }
};



template <typename T>
class ColumnReader
{
    const T* _pData;
    td::UINT4 _nRows;
    td::UINT4 _colPos;
public:
    ColumnReader(): _pData(nullptr), _nRows(0), _colPos(0) {}
    ColumnReader(const T* pData, td::UINT4 nRows, td::UINT4 colPos)
    : _pData(pData), _nRows(nRows), _colPos(colPos)
    {
        assert(_pData != nullptr);
    }
    
    void operator = (const ColumnReader& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
        _colPos = r._colPos;
    }
    
    void setColumn(td::UINT4 colPos)
    {
        _colPos = colPos;
    }

    const T& operator () (td::UINT4 row) const
    {
        assert(_nRows > row);
        return _pData[_colPos * _nRows + row];
    }
};

template <typename T>
class ColumnIO
{
    T* _pData;
    td::UINT4 _nRows;
    td::UINT4 _colPos;
public:
    ColumnIO(): _pData(nullptr), _nRows(0), _colPos(0) {}
    ColumnIO(T* pData, td::UINT4 nRows, td::UINT4 colPos)
    : _pData(pData), _nRows(nRows), _colPos(colPos)
    {
        assert(_pData != nullptr);
    }
    
    void operator = (const ColumnIO& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
        _colPos = r._colPos;
    }
    
    void setColumn(td::UINT4 colPos)
    {
        _colPos = colPos;
    }

    T& operator () (td::UINT4 row)
    {
        assert(_nRows > row);
        return _pData[_colPos * _nRows + row];
    }
};

template <typename T>
class FirstColumnReader
{
    const T* _pData;
    td::UINT4 _nRows;
public:
    FirstColumnReader(): _pData(nullptr), _nRows(0) {}
    FirstColumnReader(const T* pData, td::UINT4 nRows)
    : _pData(pData), _nRows(nRows)
    {
        assert(_pData != nullptr);
    }
    
    void operator = (const FirstColumnReader& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
    }

    const T& operator () (td::UINT4 row) const
    {
        assert(_nRows > row);
        return _pData[row];
    }
};

template <typename T>
class FirstColumnIO
{
    T* _pData;
    td::UINT4 _nRows;
public:
    FirstColumnIO(): _pData(nullptr), _nRows(0) {}
    FirstColumnIO(T* pData, td::UINT4 nRows)
    : _pData(pData), _nRows(nRows)
    {
        assert(_pData != nullptr);
    }
    
    void operator = (const FirstColumnIO& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
    }
    
    T& operator () (td::UINT4 row)
    {
        assert(_nRows > row);
        return _pData[row];
    }
};

template <typename T>
class ColumnReader1
{
    const T* _pData;
    td::UINT4 _nRows;
    td::UINT4 _colPos;
public:
    ColumnReader1(): _pData(nullptr), _nRows(0), _colPos(0) {}
    ColumnReader1(const T* pData, td::UINT4 nRows, td::UINT4 colPos)
    : _pData(pData), _nRows(nRows), _colPos(colPos-1)
    {
        assert(colPos > 0);
        assert(_pData != nullptr);
    }
    
    void operator = (const ColumnReader1& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
        _colPos = r._colPos;
    }

    void setColumn(td::UINT4 colPos)
    {
        assert(colPos > 0);
        _colPos = colPos-1;
    }
    
    const T& operator () (int row) const
    {
        assert( (row > 0) && (_nRows >= row));
        return _pData[_colPos * _nRows + row - 1];
    }
};

template <typename T>
class ColumnIO1
{
    T* _pData;
    td::UINT4 _nRows;
    td::UINT4 _colPos;
public:
    ColumnIO1(): _pData(nullptr), _nRows(0), _colPos(0) {}
    ColumnIO1(T* pData, td::UINT4 nRows, td::UINT4 colPos)
    : _pData(pData), _nRows(nRows), _colPos(colPos-1)
    {
        assert(colPos > 0);
        assert(_pData != nullptr);
    }
    
    void operator = (const ColumnIO1& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
        _colPos = r._colPos;
    }

    void setColumn(td::UINT4 colPos)
    {
        assert(colPos > 0);
        _colPos = colPos-1;
    }
    
    T& operator () (int row)
    {
        assert( (row > 0) && (_nRows >= row));
        return _pData[_colPos * _nRows + row - 1];
    }
};

template <typename T>
class FirstColumnReader1
{
    const T* _pData;
    td::UINT4 _nRows;
public:
    FirstColumnReader1(): _pData(nullptr), _nRows(0) {}
    FirstColumnReader1(const T* pData, td::UINT4 nRows)
    : _pData(pData), _nRows(nRows)
    {
        assert(nRows > 0);
        assert(_pData != nullptr);
    }
    
    void operator = (const FirstColumnReader1& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
    }

    const T& operator () (td::UINT4 row) const
    {
        assert( (row > 0) && (_nRows >= row));
        return _pData[row - 1];
    }
};

template <typename T>
class FirstColumnIO1
{
    T* _pData;
    td::UINT4 _nRows;
  
public:
    FirstColumnIO1(): _pData(nullptr), _nRows(0) {}
    FirstColumnIO1(T* pData, td::UINT4 nRows)
    : _pData(pData), _nRows(nRows)
    {
        assert(nRows > 0);
        assert(_pData != nullptr);
    }
    
    void operator = (const FirstColumnIO1& r)
    {
        _pData = r._pData;
        _nRows = r._nRows;
    }

    T& operator () (td::UINT4 row)
    {
        assert( (row > 0) && (_nRows >= row));
        return _pData[row - 1];
    }
};

template <typename T>
class DiagReader
{
    const T* _pData;
    td::UINT4 _size;
public:
    DiagReader(): _pData(nullptr), _size(0) {}
    DiagReader(const T* pData, td::UINT4 size)
    : _pData(pData), _size(size)
    {
        assert(_pData != nullptr);
    }
    
    void operator = (const DiagReader& r)
    {
        _pData = r._pData;
        _size = r._size;
    }

    const T& operator () (td::UINT4 posDiag) const
    {
        assert(_size > posDiag);
        return _pData[posDiag];
    }
};

template <typename T>
class DiagIO
{
    T* _pData;
    td::UINT4 _size;
public:
    DiagIO(): _pData(nullptr), _size(0) {}
    DiagIO(T* pData, td::UINT4 size)
    : _pData(pData), _size(size)
    {
        assert(_pData != nullptr);
    }

    void operator = (const DiagIO& r)
    {
        _pData = r._pData;
        _size = r._size;
    }
    
    T& operator () (td::UINT4 posDiag)
    {
        assert(_size > posDiag);
        return _pData[posDiag];
    }
};

template <typename T>
class DiagReader1
{
    const T* _pData;
    td::UINT4 _size;
public:
    DiagReader1(): _pData(nullptr), _size(0) {}
    DiagReader1(const T* pData, td::UINT4 size)
    : _pData(pData), _size(size)
    {
        assert(_pData != nullptr);
    }
    
    void operator = (const DiagReader1& r)
    {
        _pData = r._pData;
        _size = r._size;
    }
                        
    const T& operator () (td::UINT4 posDiag) const
    {
        assert( (posDiag > 0) && (_size >= posDiag) );
        return _pData[posDiag - 1];
    }
};

template <typename T>
class DiagIO1
{
    T* _pData;
    td::UINT4 _size;
public:
    DiagIO1(): _pData(nullptr), _size(0) {}
    DiagIO1(T* pData, td::UINT4 size)
    : _pData(pData), _size(size)
    {
        assert(_pData != nullptr);
    }
    
    void operator = (const DiagIO1& r)
    {
        _pData = r._pData;
        _size = r._size;
    }

    T& operator () (td::UINT4 posDiag)
    {
        assert( (posDiag > 0) && (_size >= posDiag) );
        return _pData[posDiag-1];
    }
};

}

