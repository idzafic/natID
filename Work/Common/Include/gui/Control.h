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
#include "Frame.h"
#include <gui/_aux/Cell.h>
#include <td/Variant.h>
#include <gui/Font.h>
#include <functional>
#include <tuple>
#include <math/Limits.h>

namespace gui
{

class Layout;
class FileDialog;
class Dialog;
class View;
class StackedLayout;
class GridLayout;
class SplitterLayout;

class NATGUI_API Control : public Frame
{
public:
    enum class Limit : td::BYTE {None=0, UseAsMin, Fixed};
private:
    friend class Layout;
    friend class StackedLayout;
    friend class GridLayout;
    friend class SplitterLayout;
    friend class ControlHelper;
    friend class View;

    td::UINT4 _visualID = 0;
//    td::UINT4 _maxWidth = math::maxVal<td::UINT4>(); //not used yet
    td::UINT2 _preferableWidth = 0;
    td::UINT2 _preferableHeight = 0;
protected:
    td::UINT2 _contextMenuGroup = 0;
    td::BYTE _contextMenuID = 0;
    td::BYTE _preferableWidthUsage : 2;
    td::BYTE _preferableHeightUsage : 2;
    td::BYTE _onToolbar : 1;
    td::BYTE _reMeasure : 1;
private:
    void openContextMenu(const gui::InputDevice& inputDevice);
    
protected:
    Control();
    void sendReMeasureMsg();
    td::BYTE getContextMenuID() const;
    void adjustToPreferableSize(CellInfo& ci);
    virtual void reMeasure(CellInfo&) = 0;
    virtual void measure(CellInfo&) = 0;
    //void getMinSize(Size& sz);
    virtual bool freeze();
    virtual void unFreeze();
public:
    
    virtual void getMinSize(Size& minSize) const;
    virtual td::WORD getTotalHMargin() const;
    virtual td::WORD getTotalVMargin() const;
    
    virtual void setGeometry(const Geometry& cellFrame, const Cell& cell);
    void measure(const char* pString, Size& sz) const; //return size of the text in ptring using control's font
    void measure(td::BYTE nChars, Size& sz, char ch='H') const; //return size of nChars using control's font
    void measureToFitContent(Size& sizeToFit) const;
    
    //can set limits on width and height of control
    void setSizeLimits(td::UINT2 width, Limit widthHandling, td::UINT2 height = 0, Limit heightHandling = Limit::None);
    //set limits on width of control
    void setSizeLimit(const char* strWidthPattern, Limit widthHandling);
    void setSizeLimitForNChars(td::BYTE nChars, Limit widthHandling, char ch='H');
    std::tuple<td::UINT2, Control::Limit> getWidthLimit() const;
    std::tuple<td::UINT2, Control::Limit> getHeightLimit() const;
    
    virtual td::UINT2 getPreferableWidth(td::UINT2 forHeight, td::UINT2 minWidth) const;
    
    void disableRemeasuring();
    //virtual td::UINT4 getTagID() const;
    void enable(bool bEnable = true);
    void disable(bool bDisable = true);
    bool isDisabled() const;
    virtual bool isEditable() const;
    void setFlat() const;
    bool isText() const;
    bool isLayout() const;
    void hide(bool bShow, bool recalcLayout);
    virtual void sizeToFit();
    void setToolTip(const td::String& toolTip);
    void setBold();
    void setFont(Font::ID fnt);
    void setTextColor(td::Accent accClrID);
    void getFontDescription(gui::Font& font) const;
    gui::FileDialog* getAttachedFileDialog(td::UINT4 fileDlgID);
    gui::Window* getAttachedWindow(td::UINT4 wndID);
    
    template <typename TID>
    gui::Dialog* getAttachedDialog(TID wndID)
    {
        gui::Window* pWnd = getAttachedWindow((td::UINT4) wndID);
        if (!pWnd)
            return nullptr;
        gui::Dialog* pDlg = reinterpret_cast<gui::Dialog*>(pWnd);
        return pDlg;
    }
    
    void adjustToToolBarSize(gui::Size& sz) const;
    void setVisualID(td::UINT4 visualID);
    td::UINT4 getVisualID() const override;
    bool isOnToolBar() const;
    
    //contextMenuID has to be prepared; the context menu will be shown upon a right mouse click (on buttons)
    void setContextMenuID(td::BYTE contextMenuID, td::UINT2 contextGroup = 0);
};
}
