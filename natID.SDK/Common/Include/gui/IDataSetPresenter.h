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
#include <dp/IDataSet.h>

namespace gui
{

class IDataSetPresenter
{
public:
    virtual void show(dp::IDataSet* pDS) = 0;
    virtual void cleanAndDetachDataSet() = 0;
};

} //namespace gui
