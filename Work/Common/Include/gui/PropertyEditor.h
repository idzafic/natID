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
#include <gui/PropertyViewScroller.h>
#include <gui/_aux/PropertyEditorTitle.h>
#include <gui/VerticalLayout.h>

namespace gui
{

class NATGUI_API PropertyEditor : public View
{
    PropertyEditorTitle _title;
    PropertyViewScroller _propViewScroller;
    gui::VerticalLayout _layout;
    
protected:
    void setProperties(Properties* properties);
    bool onClick(gui::DrawableButtonIntern* pSymbBtn) override;
    PropertyView* getPropertyView();
public:
    PropertyEditor(const td::String& title, Properties* properties, bool autoUpdate = false, td::HAlignment labelAlignment = td::HAlignment::Right);
    PropertyEditor(const td::String& title, bool autoUpdate = false, td::HAlignment labelAlignment = td::HAlignment::Right);
    
    ~PropertyEditor();
    void setParentFrame(Frame* pFrame) override;
    void setLabelAlignment(td::HAlignment hAlign);
    void setTitleAlignment(td::HAlignment hAlign);
    void setPropertyConsumer(gui::IProperty* propConsumer, bool updateData);
    void setCanvas(Canvas* pCanvas);
    void updateValues();
    
    //void setGeometry(const Geometry& cellFrame, const Cell& cell);
};

} //namespace gui
