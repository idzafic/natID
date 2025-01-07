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
#include <gui/_aux/PropertyView.h>

namespace gui
{
class PropertyEditor;

class NATGUI_API PropertyViewScroller : public ViewScroller
{
    friend PropertyEditor;
    
    PropertyView _propView;
protected:
    void setProperties(Properties* properties);
    PropertyView* getPropertyView();
public:
    PropertyViewScroller(Properties* properties, bool autoUpdate = false, td::HAlignment labelAlignment = td::HAlignment::Left);
    PropertyViewScroller(bool autoUpdate = false, td::HAlignment labelAlignment = td::HAlignment::Left);
    
    ~PropertyViewScroller();
    void setParentFrame(Frame* pFrame) override;
    void setLabelAlignment(td::HAlignment hAlign);
    void setPropertyConsumer(gui::IProperty* propConsumer, bool bUpdate);
    void setCanvas(Canvas* pCanvas);
    void updateValues();
};

} //namespace gui
