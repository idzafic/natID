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
#include <td/Variant.h>

namespace gui
{
class Application;
class Window;
class Image;
class Symbol;
class ToolBarItem;
class IPopoverButton;
class Layout;
class BaseViewHelper;
class ViewScroller;

class NATGUI_API BaseView : public Control
{
    friend class Application;
    friend class ToolBarItem;
    friend class BaseViewHelper;
    friend class ViewScroller;
    friend class Window;
    
public:
    //enum class PopoverType : unsigned char {None=0, Form, Canvas};
protected:
    void* _menuActions = nullptr;
    td::Variant _varSource; //file name or something else
    td::BYTE _contentType = 0;
    
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
    virtual bool estimateDesiredInitialSize(const gui::Size& initialSize, gui::Size& desiredSize) const;
    virtual void onGeometryChange(const Geometry& newGeometry);
    virtual void onContentSizeChange(const gui::Size& newSize);
    virtual void setScale(double newScale);
    virtual void finishedChildrenMeasuring(CellInfo& ci);
    virtual bool shouldClose(); //tabview will not be closed if this method returns false
    virtual void onClose(); //will be called only once 
    virtual std::tuple<bool, bool> isFixedSize() const;
    virtual Layout* getLayout();
    virtual void systemColorModeChanged(bool bDarkMode);
public:
    virtual Frame::FixSizes getFixSizesInfo();
    
    virtual void setAsCentralViewInWindow(Window* pWnd, Frame::FixSizes fixSizes);
        
    virtual void scale(double newScale);
    virtual void scaleToPoint(double newScale, const gui::Point& toPoint);
    virtual double getScale() const;
    
    void setSource(const td::Variant& varFilenameOrOtherIndicator);
    ///returnns fileName of other indicator used for the view
    const td::Variant& getSource() const;
    
    template <typename TSOURCE>
    void getSource(TSOURCE& src)
    {
        const td::Variant& var = getSource();
        var.getValue(src);
    }
    
    void setContentTypeID(td::BYTE cntType);
    td::BYTE getContentTypeID() const;
    
    template <typename T>
    void setContentType(T contentType)
    {
        setContentTypeID((td::BYTE) contentType);
    }
    
    template <typename T>
    void getContentType(T& contentType)
    {
        contentType = (T) getContentTypeID();
    }
    
    //event handlers
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override;
    void onActionItem(td::BYTE menuID, td::BYTE actionID, const std::function<void()>& fnToCall); //first and last submenu = 0
    void onActionItem(td::BYTE menuID, td::BYTE firstSubMenuID, td::BYTE actionID, td::BYTE lastMenuID, const std::function<void()>& fnToCall);
};

} //namespace gui
