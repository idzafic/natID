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

namespace gui
{

class Window;
class FileDialog;

struct WndID
{
    Window* pWnd;
    td::UINT4 wndID;
    
    bool operator == (const struct WndID& w2) const
    {
        if (w2.wndID == 0)
        {
            return (w2.pWnd == pWnd);
        }
        else if (w2.pWnd == nullptr)
        {
            return (w2.wndID == wndID);
        }
        
        return ((w2.wndID == wndID) && (w2.pWnd == pWnd));
    }
};

struct FileDlgID
{
    FileDialog* pFD;
    td::UINT4 wndID;
    
    bool operator == (const struct FileDlgID& w2) const
    {
        if (w2.wndID == 0)
        {
            return (w2.pFD == pFD);
        }
        else if (w2.pFD == nullptr)
        {
            return (w2.wndID == wndID);
        }
        
        return ((w2.wndID == wndID) && (w2.pFD == pFD));
    }
};

}
