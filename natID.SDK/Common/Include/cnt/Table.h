// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <cstddef>   // size_t
#include <cassert>   // assert

namespace cnt
{

template <typename T, bool RowMajor = true>
class Table
{
    private:
    T* _data = nullptr;
    std::size_t _rows;
    std::size_t _cols;
    
    
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
    Table(std::size_t rows, std::size_t cols)
    : _rows(rows)
    , _cols(cols)
    {
        _data = new T[rows * cols];
    }
    
    Table()
    : _rows(0)
    , _cols(0)
    {
    }
    
    // Copy constructor
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
    ~Table()
    {
        if (_data)
            delete[] _data;
    }
    
    
    void reserve(size_t nRows, size_t nColumns)
    {
        if (_data)
            delete[] _data;
        _rows = nRows;
        _cols = nColumns;
        _data = new T[nRows * nColumns];
    }
    
    void zeros()
    {
        std::size_t n = _rows * _cols;
        for (std::size_t i = 0; i < n; ++i)
            _data[i] = T(0);
    }
    
    // Copy assignment
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
    T& operator()(std::size_t r, std::size_t c)
    {
        assert(r < _rows && c < _cols);
        return _data[index(r, c)];
    }
    
    // Read-only access
    const T& operator()(std::size_t r, std::size_t c) const
    {
        assert(r < _rows && c < _cols);
        return _data[index(r, c)];
    }
    
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
    T* data() { return _data; }
    const T* data() const { return _data; }
    
    std::size_t rows() const { return _rows; }
    std::size_t cols() const { return _cols; }
};

} //namespace cnt
