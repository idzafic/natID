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
#include <gui/CodeEdit.h>
#include <gui/View.h>
#include <gui/TableEdit.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <gui/VerticalLayout.h>
#include <cnt/SafeFullVector.h>

namespace gui
{

class NATCODE_API CodeEditLogger : public gui::View, public sc::ILog
{
public:
    enum class InfoType : td::BYTE {Text=0, Emojis};
protected:
    dp::IDataSet* _pDS; //Intern management
    cnt::SafeFullVector<CodeEdit*> _pCodeEdits;
    cnt::SafeFullVector<td::String> _editorEnumStrings;
    static cnt::SafeFullVector<td::String> typeEnumStrings;
    gui::VerticalLayout _layout;
    gui::TableEdit _table;
    td::UINT4 _noOfErrors = 0;
    td::UINT2 _currentEditorPos = 0;
    td::BYTE _collectLoggs = 0;
    InfoType _infoType = InfoType::Text;
private:
    CodeEditLogger() = delete;
public:
    CodeEditLogger(dp::IDataSet* pDS, td::UINT2 nEditors, InfoType infoType=InfoType::Text);
    
    //CodeEdit::ILog interface implementation
    void clean() override;
    void enableCollecting(bool bCollectLogs) override;
    void setModulePosition(td::UINT2 editorPos) override;
    void add(const sc::ILog::Entry& logEntry, sc::ILog::Type type = sc::ILog::Type::Error) override;
    void setTranslationFn(sc::TranslateFunction) override;
    void addWoTranslation(const td::String& str, Type type = Type::Info) override;
    void translateAndAdd(const char* pMsg, Type type = Type::Error) override;
    void translateAndAdd(const char* pMsg, td::UINT4 lineNo, td::UINT4 colNo, Type type = Type::Error) override;
    void translateAndAdd(const char* pMsg, td::UINT4 lineNo, td::UINT4 colNo, Type type, const char* param1, ...) override;
    void refresh() override;
    void setInfoType(InfoType infoType);
    void setEditor(CodeEdit* pEdit, const td::String& editorName, td::UINT2 editorPos);
    void setCurrentEditor(td::UINT2 editorPos);
    td::String toString() const;
    td::UINT4 getNoOfErrors() const override;
    ~CodeEditLogger();
};

} //namespace gui

