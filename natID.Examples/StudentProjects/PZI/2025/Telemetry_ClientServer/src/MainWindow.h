#pragma once
#include <gui/Window.h>
#include <gui/ActionItem.h>
#include "MenuBar.h"
#include "MainView.h"
#include "ViewSplit.h"
#include "ToolBar.h"
#include "Data.h"
#include "ViewTableEdit.h"
#include "NetView.h"
#include "ViewBar.h"
#include <gui/FileDialog.h>


class MainWindow : public gui::Window
{
private:
protected:
    MenuBar menuBar;
    MainView mainView;
    gui::Image imgAddLineGraph;
    gui::Image imgAddTable;
    gui::Image imgAddCollum;
    gui::Image imgNoC;
    gui::Image imgC;
    ToolBar toolBar;
    int viewPos = 0;
    Data data;
public:
    MainWindow()
        : gui::Window(gui::Geometry(50, 50, 800, 800))
        , imgAddLineGraph(":line")
        , imgAddTable(":table")
        , imgAddCollum(":collum")
        , imgNoC(":c")
        , imgC(":c")
        , toolBar(&imgNoC)
    {
        setTitle("Telemetrija");
        menuBar.setAsMain(this);
        setToolBar(toolBar);
        setCentralView(&mainView);
    }
    
    ~MainWindow()
    {
    }
    
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
        
        if (menuID == 10 && firstSubMenuID== 0 && lastSubMenuID == 0){
            switch (actionID)
            {
            case 10:    //creates a new tab with a line graph
            {
                showYesNoQuestionAsync("Create new Line Graph?", "Are you sure?", "Yes", "No",
                                       [this](gui::Alert::Answer retStatus){
                                           if (retStatus != gui::Alert::Answer::Yes)
                                               return;

                                           td::BYTE ctxMenuID = 100;
                                           //add View
                                           //BackView2* pView =  new BackView2;
                                           ViewSplit* pView = new ViewSplit;

                                           int iPos = mainView.addView(pView,"Line Graph",&imgAddLineGraph);

                                           mainView.setHeaderContextMenu(iPos, ctxMenuID);
                                           mainView.forwardContextMenuEvents(iPos, this);
                                           ++viewPos;
                                       });
                return true;
            }
            case 20:{   //creates a new tab with a table of all the data in the database
                showYesNoQuestionAsync("Create new data Table View?", "Are you sure?", "Yes", "No",
                                       [this](gui::Alert::Answer retStatus){
                                           if (retStatus != gui::Alert::Answer::Yes)
                                               return;

                                           td::BYTE ctxMenuID = 100;
                                           //add View
                                           //BackView2* pView =  new BackView2;
                                           ViewTableEdit* pView = new ViewTableEdit();

                                           int iPos = mainView.addView(pView,"Data Table",&imgAddTable);

                                           mainView.setHeaderContextMenu(iPos, ctxMenuID);
                                           mainView.forwardContextMenuEvents(iPos, this);
                                           ++viewPos;
                                       });
                return true;
            }
            case 30:{   //creates a new tab with a column graph showing the maximum values per run of a specified data type
                showYesNoQuestionAsync("Create new Column Graph?", "Are you sure?", "Yes", "No",
                                       [this](gui::Alert::Answer retStatus){
                                           if (retStatus != gui::Alert::Answer::Yes)
                                               return;

                                           td::BYTE ctxMenuID = 100;
                                           //add View
                                           //BackView2* pView =  new BackView2;
                                           ViewBar* pView = new ViewBar();

                                           int iPos = mainView.addView(pView,"Column Graph",&imgAddCollum);

                                           mainView.setHeaderContextMenu(iPos, ctxMenuID);
                                           mainView.forwardContextMenuEvents(iPos, this);
                                           ++viewPos;
                                       });
                return true;
            }
            case 40:{   //creates a new tab with a line graph that recieves and displays its data live from a network connection
                showYesNoQuestionAsync("Create new Network Line Graph?", "Are you sure?", "Yes", "No",
                                       [this](gui::Alert::Answer retStatus){
                                           if (retStatus != gui::Alert::Answer::Yes)
                                               return;

                                           td::BYTE ctxMenuID = 100;
                                           //add View
                                           //BackView2* pView =  new BackView2;
                                           NetView* pView = new NetView();

                                           int iPos = mainView.addView(pView,"Network Graph",&imgC);

                                           mainView.setHeaderContextMenu(iPos, ctxMenuID);
                                           mainView.forwardContextMenuEvents(iPos, this);
                                           ++viewPos;
                                       });
                return true;
            }
            case 0:
            {

                return true;
            }
            }
        }
        
        return true;
    }
    
};
