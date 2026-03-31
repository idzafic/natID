//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
//
#pragma once
#include <gui/Window.h>
#include "MenuBar.h"
#include "ToolBar.h"
#include "MainView.h"
#include <gui/FileDialog.h>
#include <gui/Alert.h>
#include <fo/FileOperations.h>
#include "DialogSettings.h"

//#define USE_CALLBACKS

class MainWindow : public gui::Window
{
    enum class QuestionID : td::UINT4 {OpenFile=1, SaveFile};
protected:
    MenuBar _mainMenuBar;
    ToolBar _toolBar;
    MainView _mainView;
    
#ifdef USE_CALLBACKS
    gui::FileDialog::CallBack _callBackOpenFileDlg;
    gui::FileDialog::CallBack _callBackSaveFileDlg;
    gui::Alert::CallBack _callBackReloadAnswer;
#endif
    
public:
    MainWindow()
    : gui::Window(gui::Size(400, 300))
#ifdef USE_CALLBACKS
    , _callBackOpenFileDlg(std::bind(&MainWindow::openFile, this, std::placeholders::_1)) //async file open dialog
    , _callBackSaveFileDlg(std::bind(&MainWindow::saveFile, this, std::placeholders::_1)) //async file save dialog
    , _callBackReloadAnswer(std::bind(&MainWindow::checkLoadAnswer, this, std::placeholders::_1)) //async question (showYesNoQuestionAsync)
#endif
    {       
        setTitle(tr("appTitle"));
        //_mainMenuBar.setAsMain(this, gui::MenuBar::Location::OnWndTop);
        _mainMenuBar.setAsMain(this);
        setToolBar(_toolBar);
        setCentralView(&_mainView);

    }
    
    ~MainWindow()
    {
    }
    
protected:


    virtual void onInitialAppearance() override
    {
        //show or hide toolbar
        auto settings = getApplication()->getProperties();
        _toolBar.show(settings->getValue("TBVisible", true));
        _toolBar.setIconSize(static_cast<gui::ToolBar::IconSize>(settings->getValue("TBIconSz", 2)));
        _toolBar.showLabels(settings->getValue("TBLbls", true));
    }
    
    void openFile(gui::FileDialog* pFD)
    {
        auto status = pFD->getStatus();
        if (status == gui::FileDialog::Status::OK)
        {
            td::String strFileName = pFD->getFileName();
            td::String strContent;
            
            if (fo::loadFileContent(strFileName, strContent))
            {
                gui::TextEdit* pTE = _mainView.getTextEdit();
                pTE->setText(strContent);
            }
        }
    }
    
    void saveFile(gui::FileDialog* pFD)
    {
        auto status = pFD->getStatus();
        if (status == gui::FileDialog::Status::OK)
        {
            td::String strFileName = pFD->getFileName();
            gui::TextEdit* pTE = _mainView.getTextEdit();
            td::String strContent = pTE->getText();
            fo::OutFile f;
            if (fo::createTextFile(f, strFileName))
                f << strContent;
            f.close();
        }
    }
#ifdef USE_CALLBACKS
    void checkLoadAnswer(gui::Alert::Answer answer)
    {
        if (answer == gui::Alert::Answer::Yes)
        {
            showOpenFileDialog();
        }
    }
#endif
    
    void showOpenFileDialog()
    {
        //create OpenFile dialog and open it
        gui::OpenFileDialog::show(this, tr("OpenF"), {{tr("TxtDocs"),"*.txt"}}, WndID::FileOpenDlg);
        
//        auto pFD = new gui::OpenFileDialog(this, tr("OpenF"), {{tr("TxtDocs"),"*.txt"}});
//#ifdef USE_CALLBACKS
//        pFD->open(&_callBackOpenFileDlg);
//#else
//        pFD->openWithID(WndID::FileOpenDlg, this);
//#endif
    }
    
   
    
    bool onActionItem(gui::ActionItemDescriptor& aiDesc) override
    {
        auto [menuID, firstSubMenuID, lastSubMenuID, actionID] = aiDesc.getIDs();
        
        if (menuID == 10)
        {
            if (actionID == 10)
            {

                //old way to do it
                if (false)
                {
                    auto pDlg = getAttachedWindow((td::UINT4) WndID::SettingsDlg);
                    if (pDlg)
                        pDlg->setFocus();
                    else
                    {
                        DialogSettings* pSettingsDlg = new DialogSettings(this, (td::UINT4) WndID::SettingsDlg);
                        pSettingsDlg->open();
                        auto pView = pSettingsDlg->getCentralControl();
                        auto pViw2 = dynamic_cast<const ViewSettings*>(pView);

                        auto str = pViw2->getTranslationExt();
                        int bp = 1;
                    }
                }
                else
                {
                    td::UINT4 dlgID = 2600;
                    gui::Panel::show<ViewLogin>(this, tr("Login"), dlgID, gui::Dialog::OkCancel, [](gui::Dialog::Button::ID, ViewLogin*)
                    {
                    });
                }
                
                //return true;

                //or as panel
//                gui::showPanel<ViewSettings>(&_toolBar, this);

                //another way to do it
                //gui::showPanel<ViewSettings>(this, [this](gui::Dialog* pDlg)
                //    {
                //        //update settings, user clicked ok
                //        if (pDlg->getClickedButtonID() == gui::Dialog::Button::ID::OK)
                //        {

                //            gui::PanelDefault<ViewSettings, gui::Dialog>* pDlg2 = dynamic_cast<gui::PanelDefault<ViewSettings, gui::Dialog>*>(pDlg);

                //            auto& view2 =  *(dynamic_cast<const ViewSettings*>(pDlg->getCentralView()));
                //            //auto pDlg2 = dynamic_cast<gui::PanelDefault<ViewSettings, gui::Dialog>*>(pDlg);
                //            auto& view = pDlg2->view();

                //            auto settings = getApplication()->getProperties();
                //            //update translation if needed
                //            td::String strTr = view.getTranslationExt();
                //            if (strTr.length() > 0)
                //            {
                //                //write translation info back to properties
                //                settings->setValue("translation", strTr);
                //            }

                //            //show or hide toolbar
                //            bool isTBVisible = view.isTBVisible();
                //            _toolBar.show(isTBVisible);
                //            settings->setValue("TBVisible", isTBVisible);


                //            //adjust toolbar icon size if needed
                //            gui::ToolBar::IconSize iconSize = view.getTBIconSize();
                //            _toolBar.setIconSize(iconSize);
                //            settings->setValue("TBIconSz", static_cast<std::underlying_type_t<gui::ToolBar::IconSize>>(iconSize));

                //            //show or hide toolbar labels
                //            bool TBLbls = view.getTBShowLabels();
                //            _toolBar.showLabels(TBLbls);
                //            settings->setValue("TBLbls", TBLbls);
                //        }
                //    });

                return true;
            }
        }
        else if (menuID == 20) // && firstSubMenuID== 0 && lastSubMenuID == 0)
        {
            switch (actionID)
            {
                case 10:
                {
                    //EXAMPLE: Select file using OpenFileDialog
                    //check if OpenFileDialog is already open
                    gui::TextEdit* pTE = _mainView.getTextEdit();
                    bool isEmpty = pTE->isEmpty();
                    if (!isEmpty)
                    {
                        //TODO: Add translation for strings
                        //example of async question (_callBackReloadAnswer is called after user picks the answer via buttons)
#ifdef USE_CALLBACKS
                        showYesNoQuestionAsync(&_callBackReloadAnswer, "Replace data", "Text edit is not empty. Are you sure you want to replace it with new content?", tr("Yes"), tr("No"));
#else
                        showYesNoQuestionAsync(QuestionID::OpenFile, this, "Replace data", "Text edit is not empty. Are you sure you want to replace it with new content?", tr("Yes"), tr("No"));
#endif
                    }
                    else
                        showOpenFileDialog();
                    
                    return true;
                }
                    
                case 20:
                {
                    //TODO: Add translation for strings
                    
                    gui::TextEdit* pTE = _mainView.getTextEdit();
                    if (pTE->isEmpty())
                    {
                        showAlert("Save Text", "TextEdit is empty! There is Nothing to save!");
                        return true;
                    }
                    
                    //EXAMPLE: Ceate file using OpenFileDialog
                    gui::SaveFileDialog::show(this, tr("SaveF"), "*.txt", WndID::FileSaveDlg);
//                    auto pFD = new gui::SaveFileDialog(this, tr("SaveF"), "*.txt");
//#ifdef USE_CALLBACKS
//                    pFD->open(&_callBackSaveFileDlg);
//#else
//                    pFD->openWithID(WndID::FileSaveDlg, this);
//#endif
                    return true;
                }
                
                case 30:
                {
                    td::String msgText(tr("mRect"));
                    td::String informativeText;
                    informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);
                    
                   showAlert(msgText, informativeText);
                    return true;
                }
                default:
                    return false;
            }
        }
        
        td::String msgText("Unhandled onActionItem MainWindow");
        td::String informativeText;
        informativeText.format("Handled onActionItem(subMenuID=%d, firstSubSubMenuID=%d, lastSubSubMenuID=%d, actionID=%d)", menuID, firstSubMenuID, lastSubMenuID, actionID);
        
       showAlert(msgText, informativeText);
        return true;
    }
    
#ifndef USE_CALLBACKS
    bool onAnswer(td::UINT4 questionID, gui::Alert::Answer answer) override
    {
        if ( (QuestionID) questionID == QuestionID::OpenFile)
        {
            if (answer == gui::Alert::Answer::Yes)
                showOpenFileDialog();            
            return true;
        }
        return false;
    }
    
    bool onClick(gui::FileDialog* pFD, td::UINT4 dlgID) override
    {
        if ( (WndID)dlgID == WndID::FileOpenDlg)
        {
            openFile(pFD);
            return true;
        }
        else if ((WndID)dlgID == WndID::FileSaveDlg)
        {
            saveFile(pFD);
            return true;
        }
        return false;
    }
#endif
    
};
