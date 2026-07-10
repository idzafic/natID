// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file SimpleDense.h
    @brief Dense matrix container with row-major storage and bounds-checked element access. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once

#include <td/Types.h>
#include <cassert>


namespace mtx
{

/// @brief Dense two-dimensional matrix stored in row-major order on the heap.
/// @tparam T     Element type stored in the matrix.
/// @tparam TINDEX Index type used for row and column dimensions (defaults to size_t).
template <typename T, typename TINDEX = size_t>
class SimpleDense
{
private:
    TINDEX _nRows; ///< Number of rows in the matrix.
    TINDEX _nCols; ///< Number of columns in the matrix.
    T* _data;      ///< Heap-allocated flat array of matrix elements in row-major order.
public:
    typedef TINDEX IndexType; ///< Publicly accessible alias for the index type.

    /// @brief Default constructor; creates an empty matrix with no allocated storage.
    SimpleDense()
    : _nRows(0)
    , _nCols(0)
    , _data(nullptr)
    {}

    /// @brief Constructs a matrix with the given dimensions and allocates storage.
    /// @param nRows Number of rows.
    /// @param nCols Number of columns.
    SimpleDense(td::UINT4 nRows, td::UINT4 nCols)
    : _nRows(nRows)
    , _nCols(nCols)
    , _data (new T[nRows *  nCols])
    {
    }

    /// @brief Destructor; releases the heap-allocated element storage.
    ~SimpleDense()
    {
        if (_data)
            delete [] _data;
    }

    /// @brief Reallocates the matrix storage for the specified dimensions, discarding existing data.
    /// @param nRows New number of rows.
    /// @param nCols New number of columns.
    void reserve(td::UINT4 nRows, td::UINT4 nCols)
    {
        if (_data)
            delete [] _data;
        _nRows = nRows;
        _nCols = nCols;
        _data = new T[nRows *  nCols];
    }

    /// @brief Zeroes all elements in the matrix.
    void clean()
    {
        memset(_data, 0, _nRows *  _nCols * sizeof(T));
    }

    /// @brief Returns the number of rows in the matrix.
    /// @return Number of rows.
    TINDEX getNoOfRows() const
    {
        return _nRows;
    }

    /// @brief Returns the number of columns in the matrix.
    /// @return Number of columns.
    TINDEX getNoOfCols() const
    {
        return _nCols;
    }

    /// @brief Const element access at row i and column j with bounds checking.
    /// @param i Row index (0-based).
    /// @param j Column index (0-based).
    /// @return Const reference to the element at (i, j).
    const T& operator () (TINDEX i, TINDEX j) const
    {
        assert(i < _nRows);
        assert(j < _nCols);
        return _data[i*_nCols + j];
    }

    /// @brief Mutable element access at row i and column j with bounds checking.
    /// @param i Row index (0-based).
    /// @param j Column index (0-based).
    /// @return Mutable reference to the element at (i, j).
    T& operator () (TINDEX i, TINDEX j)
    {
        assert(i < _nRows);
        assert(j < _nCols);
        return _data[i*_nCols + j];
    }
};

} //namespace mtx
