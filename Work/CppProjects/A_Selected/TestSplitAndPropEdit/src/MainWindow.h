//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include <gui/FileDialog.h>
#include "MainView.h"
#include "DialogSettings.h"
#include "ContextMenus.h"


class MainWindow : public gui::Window
{
private:
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    MainView _view;
    ContextMenus _contextMenus;
//    gui::FileDialog::CallBack _callBackHandleOpenDlg;
//    gui::FileDialog::CallBack _callBackHandleSaveDlg;
public:
    
    MainWindow()
    : gui::Window(gui::Size(1200, 800))
//    , _callBackHandleOpenDlg(std::bind(&MainWindow::openFile, this, std::placeholders::_1))
//    , _callBackHandleSaveDlg(std::bind(&MainWindow::saveFile, this, std::placeholders::_1))
    {
        setTitle(tr("ShapeEditor"));
        _mainMenuBar.setAsMain(this);
        //_mainMenuBar.forwardMessagesTo(this);
        _toolBar.forwardMessagesTo(this);
        _toolBar.setIconSize(gui::ToolBar::IconSize::SystemDefault);
    
        setToolBar(_toolBar);
        setCentralView(&_view);
        setContextMenus(&_contextMenus);
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
    
    void onInitialAppearance() override //will be called only once
    {
        EditorView* pEditor = _view.getEditor();
        pEditor->setFocus();
    }
    
    bool onToolbarsPopoverSelectionChange(gui::PopoverCanvas* pPOView, td::UINT2 ctrlID, td::UINT2 selection) override
    {
        IShape2D::currentTool = (IShape2D::Tool) selection;
        EditorView* pEditor = _view.getEditor();
        pEditor->updateCursor();
        return true;
    }
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        auto pAI = aiDesc.getActionItem();
        
        if (menuID == 10 && actionID == 10)
        {
            td::UINT4 settingsID = 728289; //bilo koji unikatan broj
            auto pDlg = getAttachedWindow(settingsID);
            if (pDlg)
                pDlg->setFocus();
            else
            {
                DialogSettings* pSettingsDlg = new DialogSettings(this, settingsID);
                pSettingsDlg->keepOnTopOfParent();
                pSettingsDlg->setMainTB(&_toolBar);
                pSettingsDlg->open();
            }
            return true;
        }
        
        if (menuID == 100)
        {
                    
            //context menu;
            return true;
        }
        if (menuID == 20 && firstSubMenuID== 0 && lastSubMenuID == 0)
        {
            switch (actionID)
            {
                case 10:
                {
                    //PRIMJER Otvaranja fajla/modela
//                    pAI->disable();
                    td::String dlgTitle(tr("openF"));
                    gui::OpenFileDialog::show(this, dlgTitle,  { {tr("VGF"),"*.getf"}, {tr("BGF"), "*.bin"}}, actionID, [this](gui::FileDialog* pFileDlg)
                      {
                          auto status = pFileDlg->getStatus();
                          if (status == gui::FileDialog::Status::OK)
                          {
                              EditorView* pEditor = _view.getEditor();
                              td::String fileName = pFileDlg->getFileName();
                              pEditor->load(fileName);
                              mu::dbgLog("User pressed OK! Selected file to open =%s", fileName.c_str());
                          }
                          else
                          {
                              mu::dbgLog("User cancelled opening!");
                          }
                      });
                    
//                    gui::OpenFileDialog* pFD = new gui::OpenFileDialog(this, dlgTitle,  { {tr("VGF"),"*.getf"}, {tr("BGF"), "*.bin"}});
////                    pFD->openModal(&_callBackHandleOpenDlg);
//                    pFD->open([this](gui::FileDialog* pFileDlg)
//                    {
//                        auto status = pFileDlg->getStatus();
//                        if (status == gui::FileDialog::Status::OK)
//                        {
//                            EditorView* pEditor = _view.getEditor();
//                            td::String fileName = pFileDlg->getFileName();
//                            pEditor->load(fileName);
//                            mu::dbgLog("User pressed OK! Selected file to open =%s", fileName.c_str());
//                        }
//                        else
//                            mu::dbgLog("User cancelled opening!");
//                    });
                    return true;
                }
                    
                case 20:
                {
                    //PRIMJER snimanja fajla
                    td::String dlgTitle(tr("SaveT"));
                    const char* defaultFileName ="ImeFajla";
                    
                    gui::SaveFileDialog::show(this, dlgTitle, { {tr("VGF"),"*.getf"}, {tr("BGF"), "*.bin"}}, actionID, [this](gui::FileDialog* pFileDlg)
                      {
                          auto status = pFileDlg->getStatus();
                          if (status == gui::FileDialog::Status::OK)
                          {
                              gui::SaveFileDialog* saveFD = (gui::SaveFileDialog*) pFileDlg;
  //                            int docIndex = saveFD->getSelectedDocumentDescriptionIndex();
  //                            mu::dbgLog("Selected document description index = %d", docIndex);
                              
                              EditorView* pEditor = _view.getEditor();
                              td::String fileName = pFileDlg->getFileName();
                              pEditor->save(fileName);
                              
                              mu::dbgLog("User pressed OK! Selected file to save =%s", fileName.c_str());
                          }
                          else
                              mu::dbgLog("User cancelled saving!");
                      }, defaultFileName);
//                    gui::SaveFileDialog* pFD = new gui::SaveFileDialog(this, dlgTitle, { {tr("VGF"),"*.getf"}, {tr("BGF"), "*.bin"}}, defaultFileName);
//
//                    pFD->open([this](gui::FileDialog* pFileDlg)
//                    {
//                        auto status = pFileDlg->getStatus();
//                        if (status == gui::FileDialog::Status::OK)
//                        {
//                            gui::SaveFileDialog* saveFD = (gui::SaveFileDialog*) pFileDlg;
////                            int docIndex = saveFD->getSelectedDocumentDescriptionIndex();
////                            mu::dbgLog("Selected document description index = %d", docIndex);
//                            
//                            EditorView* pEditor = _view.getEditor();
//                            td::String fileName = pFileDlg->getFileName();
//                            pEditor->save(fileName);
//                            
//                            mu::dbgLog("User pressed OK! Selected file to save =%s", fileName.c_str());
//                        }
//                        else
//                            mu::dbgLog("User cancelled saving!");
//                    });
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
                //td::String msgText("Handling To je to");
                //td::String infoText("To je to! Sad nastavi ovdje");
                //showAlert(msgText, infoText);
//                ScrollCanvasWindow* canvasWnd = new ScrollCanvasWindow(this);
//                canvasWnd->show(gui::Window::InitialSize::Maximized);
                return true;
            }
            
            if (actionID == 40)
            {
                //Show switcher (Prikazi switcher)

                return true;
            }
            
            if (actionID == 50)
            {
                //Show navigator
//                NavigatorWindow* pNavWindow = new NavigatorWindow(this);
//                pNavWindow->show(gui::Window::InitialSize::Normal);
                return true;
            }
        }
        if (menuID == 255)
        {
            if (actionID == 10)
            {
                //increase window size
//                gui::Geometry g;
//                getGeometry(g);
//                g.size.width *= 1.2f;
//                g.size.height *= 1.2f;
//                setGeometry(g, true, gui::Frame::Animation::Yes);
                return true;
            }
            else if (actionID == 20)
            {
                //decrease window size
//                gui::Geometry g;
//                getGeometry(g);
//                g.size.width *= 0.8f;
//                g.size.height *= 0.8f;
//                setGeometry(g, true, gui::Frame::Animation::Yes);
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
