// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <td/String.h>
#include <dp/Column.h>

namespace dp
{

class DSColumn
{
public:
    enum class Type {FromDP, CalcOnLoad, CalcOnConsume};
    
    td::String name;
    void* pExtData = nullptr;
    td::DataType type = td::DataType::TD_NONE;
    char fromDP = 1;
    char toStore = 1;
    
    void operator = (const DSColumn& b)
    {
        pExtData = b.pExtData;
        name = b.name;
        type = b.type;
        fromDP = b.fromDP;
        toStore = b.toStore;
    }
    
    bool shouldStore() const
    {
        return toStore != 0;
    }
    
};

}
