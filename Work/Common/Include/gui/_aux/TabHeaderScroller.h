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
#include <gui/Font.h>
#include "TabHeader.h"
#include <gui/ViewScroller.h>

namespace gui
{
class TabView;

class NATGUI_API TabHeaderScroller : public ViewScroller
{
    friend class TabView;
    
private:
    TabHeader _header;
protected:
    void measure(CellInfo& cell) override;
    void reMeasure(CellInfo& cell) override;
    void setGeometry(const Geometry& cellFrame, const Cell& cell) override;
    const Size& getHeaderSize() const;
    void append(const Image* pImg, const td::String& lbl, td::ColorID selLineColor, td::BYTE cntType);
    void append(const Symbol* pSymb, const td::String& lbl, td::ColorID selLineColor, td::BYTE cntType);
    void sendSelChangeMessage();
    void onGeometryChange(const Geometry& newGeometry) override;
    void onContentSizeChange(const gui::Size& newSize) override;
    void setContentSize(const gui::Size& size);
    void removeTab(int pos);
    void removeTabExt(int pos, int newSelection, bool sendMessage, bool bRedraw);
    void showTab(int pos, bool bSetFocus =  true);
    void setTitle(int pos, const td::String& strTitle);
    void setModified(int pos, bool bModified);
    bool isModified(int Pos) const;
    void setContentTypeID(int pos, td::BYTE cntType);
    td::BYTE getContentTypeID(int pos) const;
    void setHeaderContextMenu(int pos, td::BYTE contextMenuID, td::UINT2 contextMenuGroup = 0);
    void forwardContextMenuEventsTo(int pos, Frame* pConsumer);
    void setNonRemovable(int viewPos, bool nonRemovable);
public:
    TabHeaderScroller(TabView* pTabView, TabHeader::Type type, td::BYTE minWidthInChars, td::BYTE maxWidthInChars, Font::ID selFontID, Font::ID fontID);
    ~TabHeaderScroller();
};

} //namespace gui
