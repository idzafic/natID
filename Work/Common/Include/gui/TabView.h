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
#include <gui/_aux/TabHeaderScroller.h>
#include "VerticalLayout.h"
#include "ViewHeapSwitcher.h"
#include "ViewScroller.h"
#include "View.h"

namespace gui
{

class NATGUI_API TabView : public View
{
    friend class TabHeader;
private:
    VerticalLayout _vl;
    TabHeaderScroller _headerScroller;
    ViewHeapSwitcher _views;
    std::function<void(int)> _onChangedSelection;
    std::function<void(int pos)> _onClosedView;
protected:
    void measure(CellInfo&) override;
    void reMeasure(CellInfo&) override;
    bool _processFrameworkMessage(gui::Message& msg) override;
    void remove(int posToRemove, int posToSelect, const Size& newSize);
public:
    TabView(TabHeader::Type type, td::BYTE minWidthInChars, td::BYTE maxWidthInChars, gui::Font::ID selFontID = gui::Font::ID::SystemBold, gui::Font::ID fontID = gui::Font::ID::SystemNormal);
    ~TabView();
    gui::ObjType getObjType() const override { return ObjType::TabView;}
    int addView(BaseView* pView, const td::String& strTitle, const Image* pImage, td::BYTE cntType = 0, td::ColorID selColor = td::ColorID::Transparent);
    int addView(BaseView* pView, const td::String& strTitle, const Symbol* pSymbol, td::BYTE cntType = 0, td::ColorID selColor = td::ColorID::Transparent);
    void setViewOwnership(int viewPos, td::Ownership ownership); //Intern - TabView will delete the view (default)
    td::Ownership getViewOwnership() const;
    BaseView* detach(int viewPos); //if OwnerShip == Extern, otherwise fails
    void removeView(int viewPos);
    void removeView(const BaseView* pView);
    void removeAllLeftFrom(int viewPos);
    void removeAllRightFrom(int viewPos);
    void removeAllButOne(int viewPos);
    void removeAll();
    //view cannot be removed by user ineraction (there is no x button and no context menus for this view)
    void setNonRemovable(int viewPos, bool nonRemovable = true);
    
    const BaseView* getView(int pos) const;
    BaseView* getView(int pos);
    BaseView* getCurrentView();
    const BaseView* getCurrentView() const;
    int getCurrentViewPos() const;
    int getViewPos(const BaseView* pView);
    bool showView(int viewPos, bool bSetFocus = true);
    void setBackgroundView(gui::BaseView* pView);
    int getNumberOfViews() const;
    TabHeaderScroller& getScroller();
    void setHeaderContextMenu(int pos, td::BYTE contextMenuID, td::UINT2 contextMenuGroup = 0);
    void forwardContextMenuEvents(int pos, Frame* pFrameToHandleCtxEvents);
    
    void setTitle(int pos, const td::String& strTitle);

    const std::function<void(int)>& getChangedSelectionHandler() const;
    void onChangedSelection(const std::function<void(int)>& fnToCall);

    const std::function<void(int)>& getClosedViewHandler() const;
    void onClosedView(const std::function<void(int)>& fnToCall);
    
    void setModified(int pos, bool bModified);
    bool isModified(int pos) const;
    
    void setContentTypeID(int pos, td::BYTE cntType);
    td::BYTE getContentTypeID(int pos) const;
    
    template <typename T>
    void setContentType(int pos, T contentType)
    {
        setContentTypeID(pos, (td::BYTE) contentType);
    }
    
    template <typename T>
    void getContentType(int pos, T& contentType)
    {
        contentType = (T) getContentTypeID(pos);
    }
};

} //namespace gui
