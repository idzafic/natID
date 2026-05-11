//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/Button.h>
#include <gui/LineEdit.h>
#include <gui/TextEdit.h>
#include <gui/HorizontalLayout.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <fo/FileOperations.h>
#include <gui/FileDialog.h>
#include "ViewOptions.h"

class ViewConv : public gui::View
{
private:
protected:
    sc::IPlugin* _pIPlugin;
    sc::IPlugin::CallBack _onComplete;
    ViewOptions* _pViewOptions = nullptr;
    
    gui::Label _lblFnIn;
    gui::LineEdit _editFnIn;
    gui::Label _lblFnOut;
    gui::LineEdit _editFnOut;
    gui::Label _lblStatus;
    gui::LineEdit _editStatus;
    gui::Button _btnSelectInFn;
    gui::Button _btnSelectOutFn;
    gui::TextEdit _te;
    gui::Button _btnInfo;;
    gui::Button _btnConvert;
    
    gui::HorizontalLayout _hlButtons;
    gui::GridLayout _gl;
    
    td::UINT4 _wndID;
protected:
    void handleUserActions()
    {
        //Info btn click
        _btnInfo.onClick([this]{
            td::String fileName = _editFnIn.getText();
            if (!fo::fileExists(fileName))
            {
                _editStatus = "ERROR! File doesn't exist!";
                return;
            }
            td::String content;
            if (!fo::loadBinaryFile(fileName, content))
            {
                _editStatus = "ERROR! Cannot load the file content!";
                return;
            }
            _editStatus = "INFO! Content ok!";
            _te.setText(content);
        });
        
        
        //Select input filen name button click
        _btnSelectInFn.onClick([this]{
            gui::OpenFileDialog::show(this, tr("openEQModel"), "*.txt", _wndID + 1000, [this](gui::FileDialog* pDlg)
            {
                  auto status = pDlg->getStatus();
                  if (status == gui::FileDialog::Status::OK)
                  {
                      td::String fileName = pDlg->getFileName();
                      if (fileName.isEmpty())
                          return;
                      _editFnIn = fileName;
                      _editFnIn.setFocus();
                  }
            });
        });
        
        //Select input filen name button click
        _btnSelectOutFn.onClick([this]{
            gui::SaveFileDialog::show(this, tr("createDmodl"), "*.dmodl", _wndID + 2000, [this](gui::FileDialog* pDlg)
            {
                  auto status = pDlg->getStatus();
                  if (status == gui::FileDialog::Status::OK)
                  {
                      td::String fileName = pDlg->getFileName();
                      if (fileName.isEmpty())
                          return;
                      _editFnOut = fileName;
                      _editFnOut.setFocus();
                  }
            });
        });
        
        //convert button click
        _btnConvert.onClick([this]{
            td::String inputFileName = _editFnIn.getText();
            if (inputFileName.isEmpty())
            {
                _editStatus = "ERROR! Empty input file name";
                return;
            }
            if (!fo::fileExists(inputFileName))
            {
                _editStatus = "ERROR! Input file name doesn't exist";
                return;
            }
            td::String outFileName = _editFnOut.getText();
            if (outFileName.isEmpty())
            {
                _editStatus = "ERROR! Empty output file name";
                return;
            }
            
            const auto& options = _pViewOptions->getOptions();
            if (!createModel(inputFileName, outFileName, _pIPlugin, options, _editStatus))
            {
                //_editStatus = "ERROR! Couln't create the model!";
                return;
            }
            _onComplete(_pIPlugin);

            gui::Window* pWnd = getParentWindow();
            pWnd->close();
            onClosedPluginWindow();
        });
    }
    ViewConv() = delete;
public:
    ViewConv(sc::IPlugin* pIPlugin, const sc::IPlugin::CallBack& onComplete)
    : _pIPlugin(pIPlugin)
    , _onComplete(onComplete)
    , _lblFnIn(tr("In File Name:"))
    , _lblFnOut(tr("Out File Name:"))
    , _lblStatus(tr("Status:"))
    , _btnSelectInFn("…")
    , _btnSelectOutFn("…")
    , _btnInfo(tr("Info"))
    , _btnConvert(tr("Convert"))
    , _hlButtons(3)
    , _gl(5,3)
    {
        assert(_pIPlugin);
        _editStatus.setAsReadOnly();
        _te.setAsReadOnly();
        _editFnIn.setToolTip(tr("PLEQ_EnterFN"));
        gui::GridComposer gc(_gl);
        gc.appendRow(_lblFnIn) << _editFnIn << _btnSelectInFn;
        gc.appendRow(_lblFnOut) << _editFnOut << _btnSelectOutFn;
        gc.appendRow(_lblStatus); gc.appendCol(_editStatus, 0);
        gc.appendRow(_te, 0); //0:span to end
        _hlButtons.appendSpacer() << _btnInfo << _btnConvert;
        gc.appendRow(_hlButtons, 0); //0:span to end
        
        setLayout(&_gl);
        
        handleUserActions();
    }
    
    void setOptions(ViewOptions* pViewOptions)
    {
        _pViewOptions = pViewOptions;
    }
    
    td::String getOutFileName() const
    {
        td::String strOutFn = _editFnOut.getText();
        return strOutFn;
    }
    
};
