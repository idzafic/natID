// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include "natGUI.h"
#include "View.h"
#include "HorizontalLayout.h"

namespace gui
{

class NATGUI_API StatusBar : public View
{
private:
    StatusBar();
//    gui::CoordType _height;
protected:
    HorizontalLayout _layout;
//    bool reMeasure() override;
    void measure(CellInfo& cell) override;
    void reMeasure(CellInfo& cell) override;
public:
    StatusBar(size_t nElems);
    gui::ObjType getObjType() const override { return ObjType::StatusBar;}
    void show(bool bShow);
    //const gui::CoordType getHeight() const;
};

}
