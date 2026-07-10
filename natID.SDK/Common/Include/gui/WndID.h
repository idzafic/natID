// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file WndID.h
    @brief Identity structures for locating windows and file dialogs by pointer and/or numeric ID. */
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

/// @brief Identifies a Window by either its pointer, its numeric ID, or both.
struct WndID
{
    Window* pWnd;      ///< Pointer to the window; may be nullptr when matching by ID only.
    td::UINT4 wndID;   ///< Numeric window identifier; 0 when matching by pointer only.

    /// @brief Equality operator that matches by pointer, by ID, or by both depending on which fields are set.
    /// @param w2 The WndID to compare against.
    /// @return True if this WndID is considered equal to w2 under the applicable matching rule.
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

/// @brief Identifies a FileDialog by either its pointer, its numeric ID, or both.
struct FileDlgID
{
    FileDialog* pFD;   ///< Pointer to the file dialog; may be nullptr when matching by ID only.
    td::UINT4 wndID;   ///< Numeric dialog identifier; 0 when matching by pointer only.

    /// @brief Equality operator that matches by pointer, by ID, or by both depending on which fields are set.
    /// @param w2 The FileDlgID to compare against.
    /// @return True if this FileDlgID is considered equal to w2 under the applicable matching rule.
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
