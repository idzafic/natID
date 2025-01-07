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
#include "InputDevice.h"
#include "PrintInfo.h"
#include "Cursor.h"

namespace gui
{
class DrawableViewHelper;
class NATGUI_API DrawableView : public BaseView
{
    friend class DrawableViewHelper;
    
protected:
    virtual void onDraw(const gui::Rect& rect);
    virtual void getRectForPage(td::UINT4 pageNo, gui::Rect& pageRectInModelCoordinates);
    virtual void onPrint(const PrintInfo& pi);
    virtual void onPDFPrint(const PrintInfo& pi);
    
    virtual void onPrimaryButtonPressed(const gui::InputDevice& inputDevice);
    virtual void onPrimaryButtonReleased(const gui::InputDevice& inputDevice);
    virtual void onSecondaryButtonPressed(const gui::InputDevice& inputDevice);
    virtual void onSecondaryButtonReleased(const gui::InputDevice& inputDevice);
    virtual void onPrimaryButtonDblClick(const gui::InputDevice& inputDevice);
    
    virtual bool onZoom(const gui::InputDevice& inputDevice);
    virtual bool onScroll(const gui::InputDevice& inputDevice);
    
    virtual void onCursorEntered(const gui::InputDevice& inputDevice);
    virtual void onCursorMoved(const gui::InputDevice& inputDevice);
    virtual void onCursorDragged(const gui::InputDevice& inputDevice);
    virtual void onCursorExited(const gui::InputDevice& inputDevice);
    
//    virtual bool onKeyPressed(const gui::Key& key);
//    virtual bool onKeyReleased(const gui::Key& key);
    
//    void enableResizeEvent(bool bEnable); //false if not called
    virtual void onResize(const gui::Size& newSize);
    
    virtual void onFocusIn();
    virtual void onFocusOut();

    DrawableView();
public:
    void setAsCentralViewInWindow(Window* pWnd, Frame::FixSizes fixSizes) override;
//    void setCursor(Cursor::Type ct, bool showImmediately = false) const;
    
    void showCursor(bool bShow); //Show/hide cursor
    void moveCursorToCenter() const; //moves cursor to center of this view
    bool getCursorPosition(gui::Point& pt) const; //returns true if view is in focus, pt contains point relative to view    
};

} //namespace gui
