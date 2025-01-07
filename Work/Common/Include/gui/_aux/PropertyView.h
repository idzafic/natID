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
#include <gui/View.h>
#include <gui/Properties.h>
#include <gui/ViewScroller.h>
#include <gui/View.h>
#include <gui/PropertyLayout.h>
#include <gui/IProperty.h>
#include <gui/_aux/PropertyView.h>
#include <gui/PropertyValues.h>

namespace gui
{

class PropertyViewScroller;
class ViewScroller;

class NATGUI_API PropertyView : public View
{
    friend class PropertyViewScroller;

protected:
    IProperty* _pIProp;
    Properties* _properties;
    PropertyLayout _layout;
    ViewScroller* _pScroller = nullptr;
    Canvas* _pCanvas = nullptr;
    PropertyValues _values;
    td::BYTE _autoUpdate;
private:
    bool takeValueFromDataCtrl(DataCtrl* pCtrl);
    void putValuesToCtrls();
protected:
    //virtual void measure(CellInfo& cell);
    //virtual void reMeasure(CellInfo& cell);
    bool _processFrameworkMessage(gui::Message& msg) override;
    void setPropertyConsumer(IProperty* pPropConsumer, bool bUpdateData);
    void updateValues(bool bMakeShadowCopy);
    void setProperties(Properties* pProperties);
    PropertyView(bool autoUpdate = false, td::HAlignment labelAlignment = td::HAlignment::Left);
    void setScroller(ViewScroller* pScroller) override;
    ViewScroller* getScroller();
    bool getModelSize(gui::Size& modelSize) const override;
    void finishedChildrenMeasuring(CellInfo& ci) override;
    
    void setCanvas(Canvas* pCanvas);
    void initCanvasRefreshInfo();
    void processCanvasRefresh();
    
    //for selection changes
    bool onFinishEdit(gui::LineEdit* pCtrl) override;
    bool onActivate(gui::LineEdit* pCtrl) override;
    bool onClick(gui::CheckBox* pBtn) override;
    bool onChangedSelection(gui::ComboBox* pCmb) override;
    bool onChangedSelection(gui::DBComboBox* pCmb) override;
    bool onChangedSelection(gui::PopoverButton* pBtn) override;
    bool onChangedValue(gui::Slider* pSlider) override;
    //virtual bool onChangedValue(gui::DateTimeEdit* pDTE);
    bool onChangedValue(gui::DateEdit* pDE) override;
    bool onChangedValue(gui::TimeEdit* pTE) override;
    bool onChangedValue(gui::ColorPicker* pCP) override;
    bool onChangedValue(gui::LinePatternPicker* pLPP) override;
public:
    PropertyView(Properties* properties, bool autoUpdate = false, td::HAlignment labelAlignment = td::HAlignment::Left);
    ~PropertyView();
    void setLabelAlignment(td::HAlignment hAlign);
    void apply();
    void restore();
};

} //namespace gui
