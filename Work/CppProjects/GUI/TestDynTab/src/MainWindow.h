//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "ViewComplex.h"
#include "EditorViewScroller.h"
#include "ViewWithTable.h"
#include "ViewHoriz.h"
#include "ViewVert.h"
#include "ViewWithTabs.h"
#include "NavigatorView.h"
#include "DynTabView.h"
#include "BackView2.h"


class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    
    DynTabView _dynTabView;
    gui::Image _imgComplex;
    gui::Image _imgScroll;
    gui::Image _imgTable;
    gui::Image _imgHoriz;
    gui::Image _imgVert;
    gui::Image _imgStandardTabs;
    gui::Image _imgNavigator;
    gui::Symbol _symbTransf;
    //gui::Alert::CallBack _callBackFn;
    gui::Image* _images[7];
    int _viewPos = 0;
    
public:
    MainWindow()
    : gui::Window(gui::Geometry(100, 100, 800, 600))
    , _imgComplex(":complex")
    , _imgScroll(":scroll")
    , _imgTable(":table")
    , _imgHoriz(":horiz")
    , _imgVert(":vert")
    , _imgStandardTabs(":standard")
    , _imgNavigator(":navigator")
    , _symbTransf(":Xfmr2")
    //, _callBackFn(std::bind(&MainWindow::handleAddNewView, this, std::placeholders::_1))
    , _images{&_imgComplex, &_imgScroll, &_imgTable, &_imgHoriz, &_imgVert, &_imgStandardTabs, &_imgNavigator}
    {
        _mainMenuBar.setAsMain(this);
    
        setToolBar(_toolBar);
        setCentralView(&_dynTabView, gui::Frame::FixSizes::No);
        
        _dynTabView.onChangedSelection([] (int newTabPos)
            {
            mu::dbgLog("Selected Tab Pos = %d", newTabPos);
            });
        
        _dynTabView.onClosedView([] (int newTabPos)
            {
            mu::dbgLog("Closed Tab Pos = %d", newTabPos);
            });
        
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
   
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
//        auto pAI = aiDesc.getActionItem();
        
        if (menuID == 10 && firstSubMenuID== 0 && lastSubMenuID == 0)
        {
            switch (actionID)
            {
                case 10:
                {
                    //with lambda (handle it asynchronosly here)
                    showYesNoQuestionAsync("Create new view", "Are you sure?", "Yes", "No", 
                        [this](gui::Alert::Answer retStatus) //lanbda
                        {
                            //this code will be called asynchronously when user clicks any button on the dialog
                            if (retStatus != gui::Alert::Answer::Yes)
                                return;

                            td::BYTE ctxMenuID = 100;
                            //add View
                            switch (_viewPos)
                            {
                            case 0:
                            {
                                //BackView2* pView =  new BackView2;
                                ViewComplex* pView = new ViewComplex;

                                int iPos = _dynTabView.addView(pView, tr("view1"), _images[_viewPos]);
                                if (iPos == 0)
                                    _dynTabView.setNonRemovable(0);
                                
                                _dynTabView.setHeaderContextMenu(iPos, ctxMenuID);
                                _dynTabView.forwardContextMenuEvents(iPos, this);
                                ++_viewPos;
                                return;
                            }
                            case 1:
                            {
                                EditorViewScroller* pView = new EditorViewScroller;

                                int iPos = _dynTabView.addView(pView, tr("view2"), &_symbTransf);
                                _dynTabView.setHeaderContextMenu(iPos, ctxMenuID);
                                _dynTabView.forwardContextMenuEvents(iPos, this);
                                ++_viewPos;
                                return;
                            }
                            case 2:
                            {
                                ViewWithTable* pView = new ViewWithTable;
                                int iPos = _dynTabView.addView(pView, tr("view3"), _images[_viewPos]);
                                _dynTabView.setHeaderContextMenu(iPos, ctxMenuID);
                                _dynTabView.forwardContextMenuEvents(iPos, this);
                                ++_viewPos;
                                return;
                            }

                            case 3:
                            {
                                ViewHoriz* pView = new ViewHoriz;
                                int iPos = _dynTabView.addView(pView, tr("view4"), _images[_viewPos]);
                                _dynTabView.setHeaderContextMenu(iPos, ctxMenuID+1);
                                //context menu handling will be delegated to pView
//                                _dynTabView.forwardContextMenuEvents(iPos, this);
                                ++_viewPos;
                                return;
                            }
                            case 4:
                            {
                                ViewVert* pView = new ViewVert;
                                int iPos = _dynTabView.addView(pView, tr("view5"), _images[_viewPos]);
                                _dynTabView.setHeaderContextMenu(iPos, ctxMenuID);
                                _dynTabView.forwardContextMenuEvents(iPos, this);
                                ++_viewPos;
                                return;
                            }
                            case 5:
                            {
                                ViewWithTabs* pView = new ViewWithTabs;
                                int iPos = _dynTabView.addView(pView, tr("view6"), _images[_viewPos]);
                                _dynTabView.setHeaderContextMenu(iPos, ctxMenuID);
                                _dynTabView.forwardContextMenuEvents(iPos, this);
                                ++_viewPos;
                                return;
                            }
                            case 6:
                            {
                                NavigatorView* pView = new NavigatorView;
                                int iPos = _dynTabView.addView(pView, tr("view7"), _images[_viewPos]);
                                _dynTabView.setHeaderContextMenu(iPos, ctxMenuID);
                                _dynTabView.forwardContextMenuEvents(iPos, this);
                                _viewPos = 0;
                                return;
                            }
                            }
                            
                        });

                    return true;
                }
                case 20:
                {
                    int iPos = _dynTabView.getCurrentViewPos();
                    if (iPos > 0)
                    {
                        _dynTabView.showView(iPos - 1);
                    }
                    return true;
                }
                case 30:
                {
                    int iPos = _dynTabView.getCurrentViewPos();

                    if (iPos >= 0)
                    {
                        auto nViews = _dynTabView.getNumberOfViews();
                        if (iPos < nViews-1)
                            _dynTabView.showView(iPos + 1);
                    }
                    return true;
                }
                case 40:
                {
                    int iPos = _dynTabView.getCurrentViewPos();

                    if (iPos >= 0)
                    {
                        td::String strTitle("New title on this tab");
                        _dynTabView.setTitle(iPos, strTitle);
                    }
                    return true;
                }
                case 50:
                {
                    int iPos = _dynTabView.getCurrentViewPos();

                    if (iPos >= 0)
                    {
                        
                    }
                    return true;
                }
                default:
                    return false;
            }
        }
        if (menuID == 100) //context menu
        {
            const gui::ContextData* pCtxData = aiDesc.getContextData();
            assert(pCtxData);
            int viewPos = pCtxData->viewPos;
            switch (actionID)
            {
                case 10: //close all on left
                    _dynTabView.removeAllLeftFrom(viewPos);
                    break;
                case 20: //close all on right
                    _dynTabView.removeAllRightFrom(viewPos);
                    break;
                case 30: //close all but this
                    _dynTabView.removeAllButOne(viewPos);
                    break;
                case 40: //close all
                    _dynTabView.removeAll();
                    break;
            }
            return true;
        }
        
        td::String msgText("Handling onActionItem MainWindow");
        td::String informativeText;
        informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);

       showAlert(msgText, informativeText);
        return true;
    }
};
