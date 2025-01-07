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
#include "BaseView.h"
#include "Types.h"

namespace gui
{

class ViewScrollerHelper;
class NATGUI_API ViewScroller : public BaseView
{
    friend class ViewScrollerHelper;
    
public:
    enum class Type : unsigned char {NoScroll = 0, ScrollerAlwaysVisible, ScrollAndAutoHide, ScrollExternal};
protected:
    BaseView* _pContentView = nullptr;
    ViewScroller::Type _horizontal;
    ViewScroller::Type _vertical;
protected:
    //virtual Frame::FixSizes getFixSizesInfo();
    //bool reMeasure();
    ViewScroller(int, Type, Type);
    void measure(CellInfo& cell) override;
    void reMeasure(CellInfo& cell) override;
//    bool isGeometryChangeNotificationRequired() const override;
    void onGeometryChange(const Geometry& newGeometry) override;
    
    virtual void onContentScrolled();
    
    //virtual void onContentSize(gui::Size& contentSize) const;
public:
    ViewScroller(Type horizontal, Type vertical);
    ~ViewScroller();
    
    gui::ObjType getObjType() const override { return ObjType::ViewScroller;}
    void setContentSize(const gui::Size& size);
    void getContentSize(gui::Size& size);
    
    void setContentView(BaseView* pContentView);
    const BaseView* getContentView() const;
    BaseView* getContentView();
    ViewScroller::Type getHorizontalScrollerType() const;
    ViewScroller::Type getVerticalScrollerType() const;
    void makeVisible(const Rect& rToShow);
    void getVisibleRect(Rect& r) const;
    
    void scaleContent(double newScale);
    void scaleContentAtPoint(double newScale, const gui::Point& atPoint);
    
    void getVisibleOrigin(gui::Point& origin) const;
    void scrollVisibleOriginTo(const gui::Point& newOrigin);
    
    void getOrigin(gui::Point& modelOrigin) const;
    void getPageSize(gui::Size& modelPageSize) const;
    
    double getScale() const override;
    
    void handleModelSizeChanged();
    
    
};

} //namespace gui
