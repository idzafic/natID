// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <dp/IDataSet.h>

namespace dp {
class IDataSetDelegate
{
public:
    virtual void init(dp::IDataSet* pDS)
    {
    }
    virtual bool onLoad(dp::IDataSet* pDS, dp::RowWrapper& row, size_t iRow)
    {
        return false;
    }
    virtual bool onConsume(dp::IDataSet* pDS, dp::RowWrapper& row, size_t iRow)
    {
        return false;
    }
};
} //namespace dp
