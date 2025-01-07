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
#include "Control.h"
#include <gui/Alert.h>
#include <tuple>

namespace gui
{
class Window;
class Image;
class Symbol;
class ToolBarItem;
class IPopoverButton;

class BaseViewHelper;
class ViewScroller;

class NATGUI_API BaseView : public Control
{
    friend class ToolBarItem;
    friend class BaseViewHelper;
    friend class ViewScroller;
    
public:
    //enum class PopoverType : unsigned char {None=0, Form, Canvas};
protected:
    //Window* _parentWnd;
    
protected:
    BaseView();
    BaseView(const BaseView&) = delete;
    BaseView(Window&&) = delete;
    void operator = (const BaseView&) = delete;

    void adjustInitialWindowSize(gui::Frame::FixSizes fixSizes, const gui::Size& sz);
    void readjustWindowSize(gui::Frame::FixSizes fixSizes, const gui::Size& sz, const gui::Size& minSize);
    virtual void setScroller(ViewScroller* pScroller);
    virtual bool getModelSize(gui::Size& modelSize) const;
    
    void registerForScrollEvents();
    
//    virtual bool isGeometryChangeNotificationRequired() const;
    virtual bool estimateDesiredInitialSize(const gui::Size& initialSize, gui::Size& desiredSize) const;
    virtual void onGeometryChange(const Geometry& newGeometry);
    virtual void onContentSizeChange(const gui::Size& newSize);
    virtual void setScale(double newScale);
    virtual void finishedChildrenMeasuring(CellInfo& ci);
    virtual bool shouldClose(); //tabview will not be closed if this method returns false
    virtual void onClose(); //will be called only once 
    virtual std::tuple<bool, bool> isFixedSize() const;

//    virtual void setMargins(td::BYTE left, td::BYTE top, td::BYTE right, td::BYTE bottom);
public:
    virtual Frame::FixSizes getFixSizesInfo();
    
    virtual void setAsCentralViewInWindow(Window* pWnd, Frame::FixSizes fixSizes);
        
    virtual void scale(double newScale);
    virtual void scaleToPoint(double newScale, const gui::Point& toPoint);
    virtual double getScale() const;
};

} //namespace gui
