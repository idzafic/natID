//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "ViewGrid.h"
#include "ViewVert.h"
#include "ViewHoriz.h"
#include "ScrolledCanvasView.h"
#include <gui/StatusBar.h>
#include "DialogSettings.h"
#include <gui/LineEdit.h>
#include <gui/Label.h>

#include "DynTabView.h"


class StatusBar : public gui::StatusBar {
    gui::LineEdit tabsCnt;
    gui::Label _l;
public:
    StatusBar() :
        gui::StatusBar(2),
        _l(gui::tr("tabsCnt"))
    {
        tabsCnt.setAsReadOnly();
        tabsCnt.setFlat();
        tabsCnt.setText("0");
        _layout << _l << tabsCnt;
        setLayout(&_layout);
    }
    void setCnt(int cnt) {
        td::String s;
        s.formatInt(cnt);
        tabsCnt.setText(s);
    }
};


class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    StatusBar _statusBar;
    DynTabView _dynTabView;
    gui::Image _imgBHFlag;
    gui::Image _imgConnOK;
    gui::Image _imgSettings;
    gui::Symbol _symbTransf;
    gui::Image* _images[3];
    int _tabCnt = 0;
    int _viewPos = 0;
    int _cSettingsDlgID = 10;
    
public:
    MainWindow()
    : gui::Window(gui::Geometry(100, 100, 800, 600))
    , _imgBHFlag(":bhFlag")
    , _imgConnOK(":connOK")
    , _imgSettings(":settings")
    , _symbTransf(":Xfmr2")
    , _images{&_imgBHFlag, &_imgConnOK, &_imgSettings}
    {
        _mainMenuBar.setAsMain(this);
        setToolBar(_toolBar);
        setCentralView(&_dynTabView, gui::Frame::FixSizes::No);
        setStatusBar(_statusBar);
        _dynTabView.onClosedView([this](int tab) {_statusBar.setCnt(--_tabCnt); });
    }
    
    ~MainWindow()
    {
    }
    
protected:
    bool shouldClose() override
    {
        return true;
    }
    
    void onClose() override
    {
        gui::Window::onClose();
    }
    
    void addNewView()
    {
        _statusBar.setCnt(++_tabCnt);
        //add View
        switch (_viewPos)
        {
            case 0:
            {
                ViewGrid* pView = new ViewGrid;
                _dynTabView.addView(pView, tr("view0"), _images[_viewPos]);
                ++_viewPos;
                return;
            }
            case 1:
            {
                ViewVert* pView = new ViewVert;
                _dynTabView.addView(pView, tr("view1"), _images[_viewPos]);
                ++_viewPos;
                return;
            }
            case 2:
            {
                ViewHoriz* pView = new ViewHoriz;
                _dynTabView.addView(pView, tr("view2"), _images[_viewPos]);
                ++_viewPos;
//                _viewPos=0;
                return;
            }
            case 3:
            {
                ScrolledCanvasView* pView = new ScrolledCanvasView;
                _dynTabView.addView(pView, tr("view3"), &_symbTransf);                
                _viewPos=0;
                //_dynTabView.addView(pView, tr("view2"), _images[_viewPos]);
                //++_viewPos;
                return;
            }
        }
    }
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        
        if (menuID == 10 && firstSubMenuID== 0 && lastSubMenuID == 0)
        {
            switch (actionID)
            {
                case 10:
                {
                    addNewView();
                    return true;
                }
                case 20:
                {
                    int curViewPos = _dynTabView.getCurrentViewPos();
                    if (curViewPos > 1)
                        _dynTabView.showView(curViewPos - 1);
                    return true;
                }
                case 30:
                {
                    int curViewPos = _dynTabView.getCurrentViewPos();
                    int nViews = _dynTabView.getNumberOfViews();
                    if (curViewPos < nViews - 1)
                        _dynTabView.showView(curViewPos + 1);
                    return true;
                }

                case 50:
                {
                    _dynTabView.removeAll();
                    return true;
                }
                default:
                    return false;
            }
        }
        if (menuID == 20 && firstSubMenuID== 0 && lastSubMenuID == 0)
        {
            if (actionID == 20)
            {
                 return true;
            }
            if (actionID == 10) {
                auto pDlg = getAttachedWindow(_cSettingsDlgID);
                if (pDlg)
                    pDlg->setFocus();
                else
                {
                    gui::Window* pWnd = getParentWindow();
                    auto statusBar = pWnd->getStatusBar();
                    DialogSettings* pSettingsDlg = new DialogSettings(this, _cSettingsDlgID);
                    pSettingsDlg->keepOnTopOfParent();

                    auto mainTB = pWnd->getToolBar();
                    pSettingsDlg->setMainTB(mainTB);

                    pSettingsDlg->open();
                }
                return true;
            }
            
        }
        if (menuID == 255)
        {
            if (actionID == 10)
            {
                //add view
                return true;
            }
            else if (actionID == 20)
            {
                //move view to left
                return true;
            }
        }
        
        td::String msgText("Handling onActionItem MainWindow");
        td::String informativeText;
        informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);

       showAlert(msgText, informativeText);
        return true;
    }
};
