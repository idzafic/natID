//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBarSwitcher.h"
#include "SwitcherView.h"
#include <functional>

//#include <gui/priv/CanvasColorPopoverView.h>

class SwitcherWindow : public gui::Window
{
private:
protected:
    ToolBarSwitcher _toolBar;
    SwitcherView _myView;
    std::function<void (void)> _fnCloseSwitcher;
public:
    SwitcherWindow(std::function<void (void)>& fnCloseSwitcher, Window* pParent = nullptr, td::UINT4 wndID = 0)
    : gui::Window(gui::Geometry(10, 300, 1000, 600), pParent, wndID)
    , _fnCloseSwitcher(fnCloseSwitcher)
    {
        _toolBar.forwardMessagesTo(this);
        _toolBar.setIconSize(gui::ToolBar::IconSize::SystemDefault);
        
        setToolBar(_toolBar);
        gui::Size sz;
        _myView.getSize(sz);
        setCentralView(&_myView, gui::Frame::FixSizes::No);
    }
    
    bool shouldClose() override
    {
        return true;
    }
    
    void onClose() override
    {
        gui::Window::onClose();
        _fnCloseSwitcher();
    }
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        auto pAI = aiDesc.getActionItem();
        
        if (menuID == 20 && firstSubMenuID == 0 && lastSubMenuID == 0)
        {
            //tolbar
            if (actionID == 10)
            {
                //go to prev
                auto prevPos = _myView.getCurrentViewPos();
                auto nViews = _myView.getNoOfViews();
                
                _myView.showPrev(pAI);
                //auto [iPos, nElems] = _myView.showPrev(pAI);
                if (prevPos+1 == nViews)
                {
                    //get next TBI and enable it
                    auto pNextAI = _toolBar.getItem(20, 0, 0, 11);
                    if (pNextAI)
                        pNextAI->enable();
                }
            }
            else if (actionID == 11)
            {
                //go to next
                auto prevPos = _myView.getCurrentViewPos();
                _myView.showNext(pAI);
                //auto [iPos, nElems] = _myView.showNext(pAI);
                if (prevPos == 0)
                {
                    //get prev TBI and enable it
                    auto pPrevAI = _toolBar.getItem(20, 0, 0, 10);
                    if (pPrevAI)
                        pPrevAI->enable();
                }
            }
            else if (actionID == 20)
            {
                //remove view
//                auto[iPos, nElems] = _myView.removeCurrent();
//                if (nElems == 0)
//                {
//                    auto pPrevAI = _toolBar.getItem(20, 0, 0, 10);
//                    if (pPrevAI)
//                        pPrevAI->disable();
//                    
//                    auto pNextAI = _toolBar.getItem(20, 0, 0, 11);
//                    if (pNextAI)
//                        pNextAI->disable();
//                    
//                    pAI->disable(); //disable Remove TBI
//                }
//                else if (iPos == 0)
//                {
//                    auto pPrevAI = _toolBar.getItem(20, 0, 0, 10);
//                    if (pPrevAI)
//                        pPrevAI->disable();
//                }
//                else if (iPos + 1 == nElems)
//                {
//                    auto pNextAI = _toolBar.getItem(20, 0, 0, 11);
//                    if (pNextAI)
//                        pNextAI->disable();
//                }
            }
            return true;
        }
        
        
        td::String msgText("Handling onActionItem SwitcherWindow");
        td::String informativeText;
        informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);
        
       showAlert(msgText, informativeText);
        return true;
    }
    
    ~SwitcherWindow()
    {
    }
};
