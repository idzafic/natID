// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file PrintInfo.h @brief Provides runtime printing information such as the current page and page layout details. */
//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include "Types.h"
#include "PageInfo.h"

namespace gui
{

/// @brief Contains information needed during a print operation, including the page info and current page index.
class PrintInfo
{
public:
    const PageInfo* pageInfo = nullptr; ///< Pointer to the page layout information (size, paper, orientation).
//    double scaleLpxToPaperX = 1;
//    double scaleLpxToPaperY = 1;
    td::UINT4 currentPage = 0; ///< Zero-based index of the page currently being rendered.
};

} //namespace gui
