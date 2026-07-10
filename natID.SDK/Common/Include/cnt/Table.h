// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file Table.h
 @brief Two-dimensional table container supporting row-major and column-major layouts. */
#pragma once
#include <cstddef>   // size_t
#include <cassert>   // assert

namespace cnt
{

/// @brief Two-dimensional array with configurable memory layout.
/// @tparam T        Element type.
/// @tparam RowMajor When true data is stored in row-major order; when false in column-major order.
template <typename T, bool RowMajor = true>
class Table
{
    private:
    T* _data = nullptr; ///< Pointer to the flat element buffer.
    std::size_t _rows;  ///< Number of rows.
    std::size_t _cols;  ///< Number of columns.


    /// @brief Computes the flat index for the element at (row, col).
    /// @param row Zero-based row index.
    /// @param col Zero-based column index.
    /// @return Flat offset into the element buffer.
    inline std::size_t index(std::size_t row, std::size_t col) const
    {
        if constexpr (RowMajor)
        {
            // Row-major layout
            return row * _cols + col;
        }
        else
        {
            // Column-major layout
            return col * _rows + row;
        }
    }

    public:
    // Constructor
    /// @brief Constructs a Table with the given dimensions.
    /// @param rows Number of rows to allocate.
    /// @param cols Number of columns to allocate.
    Table(std::size_t rows, std::size_t cols)
    : _rows(rows)
    , _cols(cols)
    {
        _data = new T[rows * cols];
    }

    /// @brief Constructs an empty Table with zero rows and columns.
    Table()
    : _rows(0)
    , _cols(0)
    {
    }

    // Copy constructor
    /// @brief Copy-constructs a Table from another instance.
    /// @param other Source Table to copy.
    Table(const Table& other)
    : _rows(other._rows)
    , _cols(other._cols)
    {
        std::size_t n = _rows * _cols;
        _data = new T[n];
        for (std::size_t i = 0; i < n; ++i)
            _data[i] = other._data[i];
    }

    // Destructor
    /// @brief Destroys the Table and frees all allocated memory.
    ~Table()
    {
        if (_data)
            delete[] _data;
    }


    /// @brief Reallocates the Table with new dimensions, discarding previous content.
    /// @param nRows    New number of rows.
    /// @param nColumns New number of columns.
    void reserve(size_t nRows, size_t nColumns)
    {
        if (_data)
            delete[] _data;
        _rows = nRows;
        _cols = nColumns;
        _data = new T[nRows * nColumns];
    }

    /// @brief Sets all elements to T(0).
    void zeros()
    {
        std::size_t n = _rows * _cols;
        for (std::size_t i = 0; i < n; ++i)
            _data[i] = T(0);
    }

    // Copy assignment
    /// @brief Copy-assigns from another Table.
    /// @param other Source Table to copy from.
    /// @return Reference to this Table.
    Table& operator=(const Table& other)
    {
        if (this != &other)
        {
            delete[] _data;

            _rows = other._rows;
            _cols = other._cols;

            std::size_t n = _rows * _cols;
            _data = new T[n];
            for (std::size_t i = 0; i < n; ++i)
                _data[i] = other._data[i];
        }
        return *this;
    }

    // Read/write access
    /// @brief Provides mutable access to the element at (r, c).
    /// @param r Zero-based row index.
    /// @param c Zero-based column index.
    /// @return Reference to the element.
    T& operator()(std::size_t r, std::size_t c)
    {
        assert(r < _rows && c < _cols);
        return _data[index(r, c)];
    }

    // Read-only access
    /// @brief Provides const access to the element at (r, c).
    /// @param r Zero-based row index.
    /// @param c Zero-based column index.
    /// @return Const reference to the element.
    const T& operator()(std::size_t r, std::size_t c) const
    {
        assert(r < _rows && c < _cols);
        return _data[index(r, c)];
    }

    /// @brief Returns the column (or row) index of the maximum element in the given row (or column).
    /// @param pos Row index in row-major mode; column index in column-major mode.
    /// @return Index of the maximum value within the specified row or column.
    size_t argMax(size_t pos) const
    {
        if constexpr (RowMajor)
        {
            // pos = row index
            size_t iPos = index(pos, 0);

            assert(pos < _rows);

            T* pRow = _data + iPos;
            size_t maxValue = pRow[0];
            size_t maxIndex = 0;

            for (size_t c = 1; c < _cols; ++c)
            {
                T v = pRow[c];
                if (v > maxValue)
                {
                    maxValue = v;
                    maxIndex = c;
                }
            }
            return maxIndex;
        }
        else
        {
            // pos = column index
            assert(pos < _cols);

            size_t iPos = index(0, pos);

            T* pCol = _data + iPos;
            T maxValue = pCol[0];
            size_t maxIndex = 0;

            for (size_t r = 1; r < _rows; ++r)
            {
                T v = pCol[r];
                if (v > maxValue)
                {
                    maxValue = v;
                    maxIndex = r;
                }
            }
            return maxIndex;
        }
    }

    /// @brief Returns the maximum element value in the given row (or column).
    /// @param pos Row index in row-major mode; column index in column-major mode.
    /// @return Maximum value within the specified row or column.
    T max(size_t pos) const
    {
        if constexpr (RowMajor)
        {
            // pos = row index
            size_t iPos = index(pos, 0);

            assert(pos < _rows);

            T* pRow = _data + iPos;
            T maxValue = pRow[0];

            for (size_t c = 1; c < _cols; ++c)
            {
                T v = pRow[c];
                if (v > maxValue)
                    maxValue = v;
            }
            return maxValue;
        }
        else
        {
            // pos = column index
            assert(pos < _cols);

            size_t iPos = index(0, pos);

            T* pCol = _data + iPos;
            T maxValue = pCol[0];

            for (size_t r = 1; r < _rows; ++r)
            {
                T v = pCol[r];
                if (v > maxValue)
                    maxValue = v;
            }
            return maxValue;
        }
    }


    // Raw pointer access (optional, useful for interop)
    /// @brief Returns a raw pointer to the underlying element buffer.
    /// @return Pointer to the buffer.
    T* data() { return _data; }

    /// @brief Returns a const raw pointer to the underlying element buffer.
    /// @return Const pointer to the buffer.
    const T* data() const { return _data; }

    /// @brief Returns the number of rows.
    /// @return Row count.
    std::size_t rows() const { return _rows; }

    /// @brief Returns the number of columns.
    /// @return Column count.
    std::size_t cols() const { return _cols; }
};

} //namespace cnt
