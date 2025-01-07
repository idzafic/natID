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
#include <gui/Types.h>
#include <td/Types.h>
//#include <tuple>

namespace gui
{

class NATGUI_API Color
{
public:
    static void getRGBA(td::ColorID, td::BYTE& R, td::BYTE& G, td::BYTE& B, td::BYTE& A);
    static void getRGBA(td::ColorID, float& R, float& G, float& B, float& A);
    static td::Color toTDColor(td::ColorID);
    //static td::Color getColor(td::ColorID colorID);
    static td::ColorID toColorID(const td::Color&);
    static const char* getName(td::ColorID clrID);
    static void toHex3(td::ColorID clrID, char hex[8]);
    static td::ColorID fromHex3(const char* hex);
//    static void toHex4(td::ColorID clrID, char hex[10]);
};

} //namespace gui
