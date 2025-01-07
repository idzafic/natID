//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/TextEdit.h>
#include <gui/VerticalLayout.h>
#include <gui/HorizontalLayout.h>
#include <gui/GridLayout.h>
#include <gui/Button.h>
#include <gui/NumericEdit.h>
#include <gui/ComboBox.h>
#include <gui/DBComboBox.h>
#include <gui/CheckBox.h>
#include <gui/Slider.h>
#include <gui/ProgressIndicator.h>
#include <gui/GridComposer.h>
#include <gui/DateEdit.h>
#include <gui/TimeEdit.h>
#include <gui/ColorPicker.h>
#include <gui/TableEdit.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <dp/IDataSetDelegate.h>
#include <fo/FileOperations.h>
#include <gui/ImageView.h>
#include <rnd/MinMax.h>

#include "ReportData.h"
#include <rpt/ShapeType.h>
#include <rpt/IViewer.h>


class DSDelegate : public dp::IDataSetDelegate
{
protected:
    int _colQuant = -1;
    int _colPrice = -1;
    int _colValue = -1;
public:
    
    void init(dp::IDataSet* pDS) override
    {
        _colQuant = pDS->getColIndex("Quant");
        assert(_colQuant >= 0);
        _colPrice = pDS->getColIndex("Price");
        assert(_colPrice >= 0);
        _colValue = pDS->getColIndex("Value");
        assert(_colValue >= 0);
    }
    
    bool onConsume(dp::IDataSet* pDS, dp::RowWrapper& row, size_t) override
    {
        td::Variant quant(row[_colQuant]);
        td::Variant price(row[_colPrice]);
        td::Variant value = quant * price;
        td::Decimal2 decVal(value.r8Val());
        row[_colValue].setValue(decVal);
        return true;
    }
};

class DSDelegateRunSum : public DSDelegate
{
    td::Decimal2 runSum = 0;
    int _colRunSum = -1;
public:
    void init(dp::IDataSet* pDS) override
    {
        DSDelegate::init(pDS);
        _colRunSum = pDS->getColIndex("RunSum");
        assert(_colRunSum >= 0);
    }
    
    bool onLoad(dp::IDataSet* pDS, dp::RowWrapper& row, size_t) override
    {
        td::Variant quant(row[_colQuant]);
        td::Variant price(row[_colPrice]);
        td::Decimal2 value = quant.dec3Val() * price.dec4Val();
//        td::Variant value (row[_colValue]);
//        td::Decimal2 decVal(value.dec2Val());
        runSum += value;
        row[_colRunSum].setValue(runSum);
        return true;
    }
};

class ViewTableEdit : public gui::View
{
private:
protected:
    gui::Label _lblID;
    gui::NumericEdit _id;
    gui::Label _lblName;
    gui::LineEdit _name;
    
    gui::Label _lblColorTMP;
    gui::ColorPicker _colorTMP;
    
    gui::Label _lblQuant;
    gui::NumericEdit _quant;
    gui::Label _lblPrice;
    gui::NumericEdit _price;
    gui::Label _lblDate;
    gui::DateEdit _date;
    gui::Label _lblTime;
    gui::TimeEdit _time;
    gui::Label _lblColor;
    gui::ColorPicker _color;
    gui::TableEdit _table;
    //labels for the bottom row
    gui::Label _lblFromID;
    gui::NumericEdit _fromID;
    gui::Label _lblToID;
    gui::NumericEdit _toID;
    gui::Image _imgNormalRep;
    gui::Image _imgWFallRep;
    gui::Symbol _symbolToShowInView;
    gui::ImageView _imgView;
    gui::HorizontalLayout _hlBtns;
    gui::Button _btnSave;
    gui::Button _btnReload;
    gui::Button _btnRemoveAll;
    gui::Button _btnDelete;
    gui::Button _btnUpdate;
    gui::Button _btnInsert;
    gui::Button _btnLogo;
    gui::Button _btnPushBack;
    gui::GridLayout _gl;
    dp::IDatabasePtr _db;
    dp::IDataSetPtr _pDS;
    DSDelegate _delegate;
    DSDelegate _delegateWF;
    DSDelegateRunSum _delegateRunSum;
    td::INT4 _paramFrom;
    td::INT4 _paramTo;
public:
    ViewTableEdit()
    : _lblID("ID:")
    , _id(td::int4)
    , _lblName(tr("Name:"))
    , _lblColorTMP("TMPColor:")
    , _lblQuant("Kol:")
    , _quant(td::decimal3, gui::LineEdit::Messages::Send, true, "Enter quantity tooltip")
    , _lblPrice("Cij:")
    , _price(td::decimal4, gui::LineEdit::Messages::Send, true, "Enter price tooltip")
    , _lblDate("Date:")
    , _lblTime("Time:")
    , _lblColor(tr("Color"))
    , _lblFromID("From ID:")
    , _fromID(td::int4)
    , _lblToID("To ID:")
    , _toID(td::int4)
    , _imgNormalRep(":normalRep")
    , _imgWFallRep(":fallRep")
    , _symbolToShowInView(":TestPie")
    , _imgView(true) //keep ratio
    , _hlBtns(14)
    , _btnSave(tr("Save"), tr("SaveTT"))
    , _btnReload(tr("Reload"), tr("ReloadTT"))
    , _btnRemoveAll(tr("DelAll"), tr("DelAllTT"))
    , _btnDelete(tr("Delete"), tr("DeleteTT"))
    , _btnUpdate(tr("Update"), tr("UpdateTT"))
    , _btnInsert(tr("Insert"), tr("InsertTT"))
    , _btnLogo(tr("Logo"), tr("Promjeni logo"))
    , _btnPushBack(tr("PushBack"), tr("PushBackTT"))
    , _gl(4,11)
    , _db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
    , _paramFrom(0)
    , _paramTo(1000)
    {
        _lblPrice.setTextColor(td::Accent::Critical);
        _price.setTextColor(td::Accent::Warning);
        _date.setTextColor(td::Accent::Success);
        
        _imgView.setImage(&_imgNormalRep);
        //fix space for imgView
        gui::Size szColorPicker;
        _color.getMinSize(szColorPicker);
        auto colSpace = _gl.getSpaceBetweenColumns();
        td::UINT2 imgvFH = (td::UINT2) (2*szColorPicker.height + colSpace);
        _imgView.setFixedHeight(imgvFH);
        
        _fromID.setValue(_paramFrom);
        _toID.setValue(_paramTo);
        
        //populate last row (horizontal layout)
        _hlBtns.append(_btnSave);
        _hlBtns.appendSpacer();
        _hlBtns.append(_lblFromID);
        _hlBtns.append(_fromID);
        _hlBtns.append(_lblToID);
        _hlBtns.append(_toID);
        _hlBtns.append(_btnReload);
        _hlBtns.appendSpace(20);
        _hlBtns.append(_btnRemoveAll);
        _hlBtns.append(_btnDelete);
        _hlBtns.append(_btnUpdate);
        _hlBtns.append(_btnInsert);
        _hlBtns.append(_btnLogo);
        _hlBtns.append(_btnPushBack);
        _btnPushBack.setAsDefault();

        gui::GridComposer gc(_gl);
        gc.appendRow(_lblID); gc.appendCol(_id); gc.appendCol(_lblName); gc.appendCol(_name, 5); gc.appendCol(_lblColorTMP); gc.appendCol(_colorTMP); gc.appendCol(_imgView, 1, td::HAlignment::Left, 2);
        gc.appendRow(_lblQuant); gc.appendCol(_quant); gc.appendCol(_lblPrice); gc.appendCol(_price); gc.appendCol(_lblDate); gc.appendCol(_date); gc.appendCol(_lblTime); gc.appendCol(_time); gc.appendCol(_lblColor); gc.appendCol(_color);
        
        gc.appendRow(_table, 0);
        gc.appendRow(_hlBtns, 0);
        
        setLayout(&_gl);
        
        populateData();
        initTable(2); //change from 0 to 2 to demonstrate different init types
        
        int iSel = _table.getFirstSelectedRow();
//        if (iSel < 0)
        _table.selectRow(10, true);
        iSel = _table.getFirstSelectedRow();
    }
    
protected:
    
    void initTable(int visPopulateType)
    {
        //this method demonstrates different TableEdit init capabilities
        if (visPopulateType == 0)
        {
            _table.init(_pDS, 0);
        }
        else if (visPopulateType == 1)
        {
            _table.init(_pDS, {0, 1, 2, 3, 9, 4, 8});
        }
        else
        {
            //full control
            gui::Columns visCols(_table.allocBindColumns(8));
            visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, tr("ItemID"), tr("ItemIDTT"))
            << gui::Header(1, tr("ProductN"), tr("ProductNTT"), 170)
            << gui::Header(2, tr("Quant"), tr("QuantTT"), 80, td::HAlignment::Right)
            << gui::Header(3, tr("Price"), tr("PriceTT"), 80, td::HAlignment::Right)
            << gui::Header(9, tr("Value"), tr("ValueTT"), 120, td::HAlignment::Right)
            //<< gui::NumFormat(3) << gui::Header(6, "Real4", "Tooltip for column 7", 80, td::HAlignment::Right)
            //<< gui::NumFormat(6) << gui::Header(7, "Real8", "Tooltip for column 8", 90, td::HAlignment::Right)
            << gui::Header(4, tr("Date"), tr("DateTT"), 80)
            << td::Time::Format::ShortHMMSS << gui::Header(5, tr("Time"), tr("TimeTT"), 100)
            << gui::Header(8, tr("Color"), tr("ColorTT"), 70);
            _table.init(_pDS);
        }
       
    }
    void populateData()
    {
//        fo::fs::path homePath;
//        mu::getHomePath(homePath);
//        fo::fs::path testDBPath = (homePath / "other_bin/TestData/natGUITest/TestRep.db");
        gui::Application* pApp = getApplication();
        fo::fs::path dbPath;
        if (!pApp->getUserDataFilePath("TestRep.db", "other_bin/TestData/natGUITest", dbPath))
        {
            mu::dbgLog("ERROR! Database file %s does not exist!", dbPath.c_str());
            return;
        }
        
        if (!_db->connect(dbPath.string().c_str()))
            return;
        
        if (false)
        {
//            CREATE TABLE "Test" (
//                "ID"    INTEGER NOT NULL,
//                "Name"    TEXT NOT NULL,
//                "Quant"    INTEGER NOT NULL,
//                "Price"    INTEGER NOT NULL,
//                "Date"    INTEGER NOT NULL,
//                "Time"    INTEGER NOT NULL,
//                "R4"    REAL NOT NULL,
//                "R8"    REAL NOT NULL,
//                "Color"    INTEGER NOT NULL,
//                PRIMARY KEY("ID")
//            );
            
            //ZA Slajdove
//            td::Variant varValue1(td::int4);
//            td::INT4 val1 = 5;
//            varValue1.setValue(val1);
//
//            varValue1 = val1;
//
//            td::INT4 val;
//            varValue1.getValue(val);
//
//
//            td::INT4 val2 = 5;
//            td::Variant varValue2(val2);
//
//            td::DataType dt = varValue2.getType();
//
//            td::String str("Neki string");
//            varValue1 = str;
            dp::Transaction tr(_db.ptr());
            dp::IStatementPtr pDelColor(_db->createStatement("DELETE FROM Color"));
            if (!pDelColor->execute())
                return;
            
            //delete some data
            dp::IStatementPtr pDelTest(_db->createStatement("DELETE FROM Test"));
            if (!pDelTest->execute())
                return;
            
            
            //enter some data
            dp::IStatementPtr pStatColor(_db->createStatement("INSERT INTO Color (ID, Name) VALUES(?,?)"));
            dp::Params parColors(pStatColor->allocParams());
            td::UINT2 colorKey;
            td::Variant colorName(td::string8, td::nch, 30);
            parColors << colorKey << colorName;
            td::UINT2 endColor = (td::UINT2) td::ColorID::SysSelectedItemBack;
            for(td::UINT2 iColor = 0; iColor < endColor; ++iColor)
            {
                colorKey = (td::UINT2) iColor;
                td::ColorID clrID = (td::ColorID) iColor;
                td::String strColorName = td::toString(clrID);
                colorName.setValue(strColorName);
                if (!pStatColor->execute())
                {
                    td::String strErr;
                    pStatColor->getErrorStr(strErr);
                    const char* pErr = strErr.c_str();
                    return;
                }
            }
            
            //enter some data
            dp::IStatementPtr pStat(_db->createStatement("INSERT INTO Test (ID, Name, Quant, Price, Date, Time, R4, R8, Color) VALUES(?,?,?,?,?,?,?,?,?)"));
            dp::Params par(pStat->allocParams());
            td::INT4 elemID = 3;
            td::String strName = "Name 3";
            td::Variant strVar(td::string8, td::nch, 40);
            strVar.setValue(strName);
            td::Decimal3 quant = 100.658;
            td::Decimal4 price = 2.5;
            td::Date date(true);
            td::Time time(true);
            float r4 = 15.72726;
            double r8 = 1024.7863;
            td::UINT2 colorID;
            par << elemID << strVar << quant << price << date << time << r4 << r8 << colorID;
            
            rnd::MinMax<td::UINT2> rndColor(0, endColor-1);
            rnd::MinMax<int> rndKolicina(1, 3000000);
            rnd::MinMax<int> rndCijena(1, 20000000);
            rnd::MinMax<int> rndYear(1995, 2022);
            rnd::MinMax<int> rndMonth(1,12);
            rnd::MinMax<int> rndDay(1,31);
            rnd::MinMax<int> rndHour(0,23);
            rnd::MinMax<int> rndMinSec(0,59);
            rnd::MinMax<int> rndFraction(0,9999);
            td::Date dateNow;
            dateNow.now();
            for (int i=0;i<500000; ++i)
            {
                td::Date rndDate(rndYear.next(), rndMonth.next(), rndDay.next());
                if (rndDate > dateNow)
                    rndDate = dateNow;
                date = rndDate;
                td::Time rndTime;
                rndTime.setTime2(rndHour.next(), rndMinSec.next(), rndMinSec.next(), rndFraction.next());
                time = rndTime;
                elemID = (i+1);
                if (i < 6)
                    colorID = (td::UINT2) ((int) td::ColorID::SysText - i);
                else
                    colorID = rndColor.next();
                
                int cij = rndCijena.next();
                double dCij = cij / 10000.;
                td::Decimal4 dec4(dCij);
                price = dec4;
                int kol = rndKolicina.next();
                double dKol = kol / 1000.;
                td::Decimal3 dec3(dKol);
                quant = dec3;
                strName.format("Naziv za ID=%d", elemID);
                strVar.setValue(strName);
                
                r4 = (float) rndCijena.next();
                r8 = rndCijena.next();
                if (!pStat->execute())
                {
                    td::String strErr;
                    pStat->getErrorStr(strErr);
                    const char* pErr = strErr.c_str();
                    return;
                }
            }
            tr.commit();
        }
        
        _pDS = _db->createDataSet("SELECT * from Test WHERE ID>=? and ID<=?", dp::IDataSet::Execution::EX_MULT);
        dp::Params params(_pDS->allocParams());
        params << _paramFrom << _paramTo;
    
        _pDS->setDelegate(&_delegate);
        //specify columns to obtain from the data provider
        dp::DSColumns cols(_pDS->allocBindColumns(10));
        cols << "ID" << td::int4 << "Name" << td::string8 << "Quant" << td::decimal3 << "Price" << td::decimal4
        << "Date" << td::date << "Time" << td::time << "R4" << td::real4 << "R8" << td::real8;
        cols << "Color" << td::word << "Value" << dp::DSColumn::Type::CalcOnConsume << td::decimal2;
        
        if (!_pDS->execute())
        {
            //show log
            _pDS = nullptr;
            return;
        }
        
        //ispod samo za debug
        size_t nRows = _pDS->getNumberOfRows();
        if (nRows == 0)
        {
            return;
        }
    }
    
    bool onChangedSelection(gui::TableEdit* pTE) override
    {
        if (pTE == &_table)
        {
            int iRow= _table.getFirstSelectedRow();
            if (iRow < 0)
            {
                _id.toZero();
                _name.toZero();
                _quant.toZero();
                _price.toZero();
                _date.toZero();
                _time.toZero();
                //_color.toZero();
                _color.setValue(td::ColorID::Black, false);
                return true;
            }
            td::Variant val;
            dp::IDataSet* pDS = _table.getDataSet();
            auto& row = pDS->getRow(iRow);
            val = row[0];
            _id.setValue(val);
            val = row[1];
            _name.setValue(val);
            val = row[2];
            _quant.setValue(val);
            val = row[3];
            _price.setValue(val);
            val = row[4];
            _date.setValue(val);
            val = row[5];
            _time.setValue(val);
            val = row[8];
            td::UINT2 u2Val = val.wordVal();
            td::ColorID clrID = (td::ColorID) u2Val;
            val = clrID;
            _color.setValue(val);
            return true;
        }
        return false;
    }
    
    void populateDSRow(dp::IDataSet::Row& row)
    {
        td::Variant val;
        _id.getValue(val);
        row[0].setValue(val);
        _name.getValue(val);
        row[1].setValue(val);
        _quant.getValue(val);
        row[2].setValue(val);
        _price.getValue(val);
        row[3].setValue(val);
        _date.getValue(val);
        row[4].setValue(val);
        _time.getValue(val);
        row[5].setValue(val);
        td::ColorID colorID = _color.getValue();
        
        td::WORD color = (td::WORD) colorID;
        row[8].setValue(color);
    }
    
    bool saveData()
    {
        dp::IStatementPtr pInsStat(_db->createStatement("INSERT INTO Test (ID, Name, Quant, Price, Date, Time, R4, R8, Color) VALUES(?,?,?,?,?,?,?,?,?)"));
        dp::Params parDS(pInsStat->allocParams());
        cnt::Array<td::Variant, 10> params;
        dp::RowWrapper row(params);
        if (!_pDS->initRowWrapper(row))
            return false;
        
        //set string DB parameters
        row[1].setDBTypeAndLen(td::nch, 64);
        
        for (size_t i=0; i<parDS.size(); ++i)
            parDS << row[i];
        
        dp::Transaction tr(_db.ptr());
        //delete some data
        dp::IStatementPtr pDel(_db->createStatement("DELETE FROM Test"));
        if (!pDel->execute())
            return false;
        size_t nRows = _pDS->getNumberOfRows();
        
        for (size_t i=0; i<nRows; ++i)
        {
            _pDS->getRow(i, row);
            if (!pInsStat->execute())
                return false;
        }
        tr.commit();
        return true;
    }
    
    bool onClick(gui::Button* pBtn) override
    {
        if (pBtn == &_btnReload)
        {
//            _table.selectRow(100, true);
//            return true;
            _paramFrom = _fromID.getValue().i4Val();
            _paramTo = _toID.getValue().i4Val();
            _table.reload();
            _table.selectRow(0, true);
            return true;
        }
        
        if (pBtn == &_btnLogo)
        {
            if (_imgView.getType() == gui::ImageView::Type::Image)
                _imgView.setImage(&_symbolToShowInView);
            else
                _imgView.setImage(&_imgNormalRep);
        }
        
        if (pBtn == &_btnRemoveAll)
        {
            _table.clean();
            return true;
        }
        
        if (pBtn == &_btnDelete)
        {
            int iRow= _table.getFirstSelectedRow();
            if (iRow < 0)
                return true;
            _table.beginUpdate();
            _table.removeRow(iRow);
            _table.endUpdate();
            return true;
        }
        if (pBtn == &_btnUpdate)
        {
            int iRow= _table.getFirstSelectedRow();
            if (iRow < 0)
                return true;
            _table.beginUpdate();
            auto& row = _table.getCurrentRow();
            populateDSRow(row);
            _table.updateRow(iRow);
            _table.endUpdate();
            return true;
        }
        
        if (pBtn == &_btnInsert)
        {
            int iRow= _table.getFirstSelectedRow();
            if (iRow < 0)
                iRow = 0;
            _table.beginUpdate();
            auto& row = _table.getEmptyRow();
            populateDSRow(row);
            _table.insertRow(iRow);
            _table.endUpdate();
            return true;
        }
        
        if (pBtn == &_btnPushBack)
        {
            _table.beginUpdate();
            auto& row = _table.getEmptyRow();
            populateDSRow(row);
            _table.push_back();
            _table.endUpdate();
            return true;
        }
        
        if (pBtn == &_btnSave)
        {
            saveData();
            return true;
        }
        
        return false;
    }

public:

void showNormalReport(bool showRunningSum)
{
//        dp::IDatabase* pDB = dp::getMainDatabase();
    dp::IDatabase* pDB = _db.getPtr();
    DocumentData docData;
    CompanyData companyData;
    ObjectData objectData;

    td::Date now;
    now.now();
    if (showRunningSum)
        docData[RPT_TXT_DOCUMENT_TYPE] = "Lager lista sa tr. sumom";
    else
        docData[RPT_TXT_DOCUMENT_TYPE] = "Lager lista";
    
    docData[RPT_TXT_DOCUMENT_ID] = "01-5/23";
    docData[RPT_DT_DOCUMENT_TO_DATETIME] = td::DateTime(true, true);
    rpt::ReportData repData(1, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

    rpt::ReportData::iterator it(repData.begin());
    
    auto pDSOrig = _table.getDataSet();
#ifdef USE_DUPLICATE
    auto pDS = pDSOrig->duplicate(nullptr);
#else
    auto pDS = _db->createDataSet("SELECT * from Test WHERE ID>=? and ID<=?", dp::IDataSet::Execution::EX_SINGLE);
    dp::Params params(pDS->allocParams());
    params << _paramFrom << _paramTo;

    
    //specify columns to obtain from the data provider
    size_t nColumns = 10;
    if (showRunningSum)
        nColumns = 11;
    
    dp::DSColumns cols(pDS->allocBindColumns(nColumns));
    cols << "ID" << td::int4 << "Name" << td::string8 << "Quant" << td::decimal3 << "Price" << td::decimal4
    << "Date" << td::date << "Time" << td::time << "R4" << td::real4 << "R8" << td::real8;
    cols << "Color" << td::word << "Value" << dp::DSColumn::Type::CalcOnConsume << td::decimal2;
    if (showRunningSum)
    {
        cols << "RunSum" << dp::DSColumn::Type::CalcOnLoad << td::decimal2;
        pDS->setDelegate(&_delegateRunSum);
    }
    else
        pDS->setDelegate(&_delegate);
    
    if (!pDS->execute())
    {
        //show log
        pDS->release();
        pDS = nullptr;
        return;
    }
    
    //ispod samo za debug
    size_t nRows = pDS->getNumberOfRows();
    if (nRows == 0)
    {
        assert(false);
        return;
    }

#endif

    it << rpt::ContainerType::CNT_Body << pDS; //sada bi se ovo trebalo ovako moci ubaciti?
    
    td::String configName;
    
    if (showRunningSum)
        configName = "LagerList2";
    else
        configName = "LagerList";

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        if (showRunningSum)
            pRepView->show(tr("Lager lista sa tr. sumama"), &_imgNormalRep);
        else
            pRepView->show(tr("Lager lista"), &_imgNormalRep);
    }

}
    
void showWaterfallReport(bool showGroupSums)
{
#ifndef TEST_KEY
   dp::IDatabase* pDB = dp::getMainDatabase();
   DocumentData docData;
   CompanyData companyData;
   ObjectData objectData;
    SignatureData signatureData;

   td::Date now;
   now.now();
    if (showGroupSums)
        docData[RPT_TXT_DOCUMENT_TYPE] = "Lager List Waterfall with group sum";
    else
        docData[RPT_TXT_DOCUMENT_TYPE] = "Lager List Waterfall";
    
   docData[RPT_TXT_DOCUMENT_ID] = "";
   docData[RPT_DT_DOCUMENT_TO_DATETIME] = td::DateTime(true, true);
   rpt::ReportData repData(3, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());
    repData.setSignatureData(&signatureData);

   rpt::ReportData::iterator it(repData.begin());

   auto datasetBody = _db->createDataSet("SELECT * from Test WHERE ID>=? and ID<=? and Color>=1 and Color <= 10 order by Color, ID asc", dp::IDataSet::Execution::EX_MULT);
   dp::Params params(datasetBody->allocParams());
   params << _paramFrom << _paramTo;


   datasetBody->setDelegate(&_delegateWF);
   dp::DSColumns cols(datasetBody->allocBindColumns(10));
   cols << "Color" << td::word << "ID" << td::int4 << "Name" << td::string8 << "Quant" << td::decimal3 << "Price" << td::decimal4
       << "Date" << td::date << "Time" << td::time << "R4" << td::real4 << "R8" << td::real8;
   cols << "Value" << dp::DSColumn::Type::CalcOnConsume << td::decimal2;


    if (!datasetBody->execute())
    {
        return;
    }
    
    auto nData = datasetBody->getNumberOfRows();
    assert(nData > 0);
    
   it << rpt::ContainerType::CNT_Body << datasetBody;

   //dodaj recordset za grouper
   auto datasetGrouper = _db->createDataSet("SELECT * from Color WHERE ID>=1 and ID <= 10 order by ID asc", dp::IDataSet::Execution::EX_MULT);
   dp::DSColumns cols2(datasetGrouper->allocBindColumns(2));
   cols2 << "ID" << td::word << "Name" << td::string8;
   if (!datasetGrouper->execute())
   {
       return;
   }

    nData = datasetGrouper->getNumberOfRows();
    assert(nData > 0);
    
    ++it;
   it << rpt::ContainerType::CNT_Grouper << datasetGrouper;
    
    ++it;
    td::String note = getNote();
    it << rpt::ContainerType::CNT_DocFooter << rpt::ShapeType::SHP_Note << "Note" << &note;


    td::String configName;
    if (showGroupSums)
        configName = "LagerListWfall2";
    else
        configName = "LagerListWfall";

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        if (showGroupSums)
            pRepView->show(tr("Lager lista WF with run sums"), &_imgWFallRep);
        else
            pRepView->show(tr("Lager lista WF"), &_imgWFallRep);
            
    }
    

//       rpt::IRepViewer* pRepView = rpt::createViewer(parentWidget(), td::String("LagerListWfall"), repData);
//       if (pRepView)
//           pRepView->showMDI(&_mdiArea);
#else

       auto datasetBody = _db->createDataSet("SELECT * from Test WHERE ID>=? and ID<=? order by Color asc", dp::IDataSet::Execution::EX_MULT);
       dp::Params params(datasetBody->allocParams());
       params << _paramFrom << _paramTo;

       datasetBody->setDelegate(&_delegate);
       dp::DSColumns cols(datasetBody->allocBindColumns(10));
       cols << "Color" << td::word << "ID" << td::int4 << "Name" << td::string8 << "Quant" << td::decimal3 << "Price" << td::decimal4
           << "Date" << td::date << "Time" << td::time << "R4" << td::real4 << "R8" << td::real8;
       cols << "Value" << dp::DSColumn::Type::CalcOnConsume << td::decimal2;
       if (!datasetBody->execute())
       {
           return;
       }


       //dodaj recordset za grouper
       auto datasetGrouper = _db->createDataSet("SELECT * from Color order by ID asc", dp::IDataSet::Execution::EX_MULT);
       dp::DSColumns cols2(datasetGrouper->allocBindColumns(2));
       cols2 << "ID" << td::word << "Name" << td::string8;
       if (!datasetGrouper->execute())
       {
           return;
       }
       auto rowCntGr = datasetGrouper->getNumberOfRows();

       dp::KeyView colorView(datasetBody);
       colorView << "Color";
       auto row = datasetGrouper->getRow(5);
       auto colorID = row[0].wordVal();
       colorView.prepare(row[0]);

       auto rowCnt = colorView.getNumberOfRows();
       for (size_t row = 0; row < rowCnt; ++row)
       {
           auto dsRow = colorView.getRow(row);

           //radi sa redom koji zadovoljava key
           auto id = dsRow[1].i4Val();
       }
#endif

   }

   void showOtherRep(gui::Frame* pContextManger)
   {
       dp::IDatabase* pDB = dp::getMainDatabase();
       DocumentData docData;
       CompanyData companyData;
       ObjectData objectData;

       docData[RPT_TXT_DOCUMENT_TYPE] = "Price Card";
       docData[RPT_TXT_DOCUMENT_ID] = "";
       docData[RPT_DT_DOCUMENT_TO_DATETIME] = td::DateTime(true, true);

       rpt::ReportData repData(1, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

       rpt::ReportData::iterator it(repData.begin());

       auto datasetBody = _db->createDataSet("SELECT * from Test WHERE ID>=? and ID<=?", dp::IDataSet::Execution::EX_SINGLE);
       dp::Params params(datasetBody->allocParams());
       params << _paramFrom << _paramTo;

       dp::DSColumns cols(datasetBody->allocBindColumns(4));
       cols << "ID" << td::int4 << "Name" << td::string8 << "Quant" << td::decimal3 << "Price" << td::decimal4;

       if (!datasetBody->execute())
       {
           return;
       }

       size_t nRows = datasetBody->getNumberOfRows();
       if (nRows == 0)
       {
           assert(false);
           return;
       }

       it << rpt::ContainerType::CNT_Replicable << datasetBody;

       //this report will contain context menu manager
       repData.setContextManager(pContextManger);

       rpt::IViewer* pRepView = rpt::IViewer::create("WindowItems", repData);
       if (pRepView)
       {
           pRepView->show(tr("Price Card"), &_imgNormalRep);
       }

   }
    
};
