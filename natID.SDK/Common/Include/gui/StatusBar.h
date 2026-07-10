// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file StatusBar.h
    @brief Horizontal status bar widget displayed at the bottom of a window. */
#pragma once
#include "natGUI.h"
#include "View.h"
#include "HorizontalLayout.h"

namespace gui
{

/// @brief A view that presents a row of status indicators at the bottom of a window.
class NATGUI_API StatusBar : public View
{
private:
    /// @brief Private default constructor; use the public parameterised constructor.
    StatusBar();
//    gui::CoordType _height;
protected:
    HorizontalLayout _layout; ///< Horizontal layout organising the status bar elements.
//    bool reMeasure() override;

    /// @brief Measures the preferred size of the status bar.
    /// @param cell CellInfo to populate with the measured size.
    void measure(CellInfo& cell) override;

    /// @brief Re-measures the status bar after a size change.
    /// @param cell CellInfo to update.
    void reMeasure(CellInfo& cell) override;

    StatusBar(const StatusBar&) = delete;
    StatusBar& operator =(const StatusBar&) = delete;
public:
    /// @brief Constructs a status bar with the specified number of element slots.
    /// @param nElems Number of child elements (controls/labels) to allocate space for.
    StatusBar(size_t nElems);

    /// @brief Returns the GUI object type identifier.
    /// @return ObjType::StatusBar.
    gui::ObjType getObjType() const override { return ObjType::StatusBar;}

    /// @brief Shows or hides the status bar.
    /// @param bShow True to make the bar visible; false to hide it.
    void show(bool bShow);
    //const gui::CoordType getHeight() const;
};

}
