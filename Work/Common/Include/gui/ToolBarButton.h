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
#include "DrawableButton.h"

namespace gui
{

class ToolBar;
class ToolBarItem;

class NATGUI_API ToolBarButton : public DrawableButton
{
private:
    ToolBarItem* _pTBI = nullptr;
//    int _width = 0;
protected:
    void measure(CellInfo&) override;
    void reMeasure(CellInfo&) override;   
    void onDraw(const gui::Rect& rect) override;
public:
    ToolBarButton(ToolBarItem* pTBI);
    
    gui::ObjType getObjType() const override { return ObjType::ToolBarButton;}
    ToolBarItem* getToolBarItem();
    ToolBar* getToolBar();
    int getIconSizeInPixels() const;
};

}
