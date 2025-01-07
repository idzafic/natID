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
#include "Consumer.h"
#include "Display.h"
#include "Alert.h"
#include <cnt/PushBackVector.h>
#include <gui/_aux/ContextMenuNat.h>
#include <gui/InputDevice.h>

namespace gui
{
class FrameHelper;
class Window;
class Button;

class NATGUI_API Frame : public Consumer
{
    friend class FrameHelper;
public:
    enum class FixSizes : td::BYTE {No=0, FixMin=1, FixHorMax=2, FixMin_andHorMax=3, FixVerMax=4, FixMin_andVerMax=5, FixHorMinAndMax=6, FixAll=7, FixAuto=8};
    enum class Animation : td::BYTE {Yes, No, SystemDefault};
protected:
    td::Variant _tagID;
    Frame* _parentFrame = nullptr;
    cnt::PushBackVector<ContextMenuNat> _contextMenus;
private:
    gui::Handle getContextMenuNatHandle(td::BYTE menuID) const;
    Frame();
    Frame(const Frame&) = delete;
    Frame(Frame&&) = delete;
    void operator = (const Frame&) = delete;
protected:
    virtual void onSetFocus();
    virtual void onLostFocus();
    void reDraw();
    void reDraw(const gui::Rect& rToRedraw);
    void showYesNoQuestionAsyncWithConsumer(td::UINT4 questionID, IMessageConsumer* pConsumer, const td::String& msgText, const td::String& informativeText, const td::String& strYes, const td::String& strNo) const;
    
    //key events
    virtual bool onKeyPressed(const gui::Key& key);
    virtual bool onKeyReleased(const gui::Key& key);
public:
//    Frame();
    Frame(Frame* parentFrame);
    ~Frame();
    
    void setGeometry(const Geometry& g);
    void getGeometry(Geometry& g) const;
    
    void getDisplayInfo(gui::Display::Info& di) const; 
    
    void setSize(const Size& desiredSize);
    void getSize(Size& sz) const;
    
    void getTLInScreenCoordinates(gui::Point& tl) const;
    void getCenterInScreenCoordinates(gui::Point& center) const;
    
    void setPreferedContentSize(const gui::Size& sz);
    
    void hide(bool bHide);
    void show(bool bShow);
    virtual bool isHidden() const;
    bool isVisible() const;
    
    virtual void setTitle(const td::String& title);
    virtual void setTitle(const char* title);
    td::String getTitle() const;
    
    void setFocus(bool selectAll = true);
    
    void setBackgroundColor(td::ColorID colorID);
    
    virtual void setParentFrame(Frame* pFrame);
    
    //Returns Frame in which this frame resides
    //If this is Window it returns nullptr
    Frame* getParentFrame();
    const Frame* getParentFrame() const;
    
    //Returns the parent Window in which this frame is located.
    //If this is Window, it returns the parent window (on which is kept on top)
    Window* getParentWindow();
    const Window* getParentWindow() const;
    
    void getPointInParentWindow(const gui::Point& ptIn, gui::Point& ptOut) const;
    
    //Returns Window in which this frame resides
    //If this is Window it returns itself
    const Window* getContainingWindow() const;
    Window* getContainingWindow();
    
    void setTagID(const td::Variant& tagID);
    const td::Variant& getTagID() const;
    
    void showAlert(const td::String& msgText, const td::String& informativeText) const;
    
//    Alert::Answer showYesNoQuestion(const td::String& msgText, const td::String& informativeText, const td::String& strYes, const td::String& strNo) const;
    void showYesNoQuestionAsync(Alert::CallBack* pCallBackFn, const td::String& msgText, const td::String& informativeText, const td::String& strYes, const td::String& strNo) const;
    
    template <typename TINT>
    void showYesNoQuestionAsync(TINT questionID, IMessageConsumer* pConsumer, const td::String& msgText, const td::String& informativeText, const td::String& strYes, const td::String& strNo) const
    {
        return showYesNoQuestionAsyncWithConsumer((td::UINT4)questionID, pConsumer, msgText, informativeText, strYes, strNo);
    }

    void showYesNoQuestionAsync(const td::String& msgText, const td::String& informativeText, const td::String& strYes, const td::String& strNo, Alert::CallBack callBackLambda) const;
    
    
    void openContextMenu(td::BYTE menuID, const gui::InputDevice& inpDev, gui::IMessageConsumer* pConsumer = nullptr, const gui::ContextData* pContextData = nullptr, td::UINT2 groupID = 0);
    
    //messages
    void showInfo(const td::String& txtHeader, const td::String& txtMsg) const;
    void showWarning(const td::String& txtHeader, const td::String& txtMsg) const;
    void showError(const td::String& txtHeader, const td::String& txtMsg) const;
};

} //namespace gui
