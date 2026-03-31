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
#include <gui/Transformation.h>

namespace gui
{

class Context
{
    Context(const Context&) = delete;
    Context& operator =(const Context&) = delete;
public:
    Context()
    {
        gui::Transformation::saveContext();
    }

    ~Context()
    {
        gui::Transformation::restoreContext();
    }
};

} //namespace gui
