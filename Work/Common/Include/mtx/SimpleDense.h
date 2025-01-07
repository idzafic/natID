// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once

#include <td/Types.h>
#include <cassert>


namespace mtx
{

template <typename T, typename TINDEX = size_t>
class SimpleDense
{
private:
    TINDEX _nRows;
    TINDEX _nCols;
    T* _data;
public:
    typedef TINDEX IndexType;
    
    SimpleDense()
    : _nRows(0)
    , _nCols(0)
    , _data(nullptr)
    {}
    
    SimpleDense(td::UINT4 nRows, td::UINT4 nCols)
    : _nRows(nRows)
    , _nCols(nCols)
    , _data (new T[nRows *  nCols])
    {
    }
    
    ~SimpleDense()
    {
        if (_data)
            delete [] _data;
    }
    
    void reserve(td::UINT4 nRows, td::UINT4 nCols)
    {
        if (_data)
            delete [] _data;
        _nRows = nRows;
        _nCols = nCols;
        _data = new T[nRows *  nCols];
    }
    
    void clean()
    {
        memset(_data, 0, _nRows *  _nCols * sizeof(T));
    }
    
    TINDEX getNoOfRows() const
    {
        return _nRows;
    }
    
    TINDEX getNoOfCols() const
    {
        return _nCols;
    }
    
    const T& operator () (TINDEX i, TINDEX j) const
    {
        assert(i < _nRows);
        assert(j < _nCols);
        return _data[i*_nCols + j];
    }
    
    T& operator () (TINDEX i, TINDEX j)
    {
        assert(i < _nRows);
        assert(j < _nCols);
        return _data[i*_nCols + j];
    }
};

} //namespace mtx
