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

class DSDelegate : public dp::IDataSetDelegate
{
public:
    virtual bool onConsume(dp::IDataSet* pDS, dp::RowWrapper& row, size_t)
    {
        td::Variant quant(row[2]);
        td::Variant price(row[3]);
        td::Variant value = quant * price;
        td::Decimal2 decVal(value.r8Val());
        row[9].setValue(decVal);
        return true;
    }
};

class ViewWithTable : public gui::View
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
    gui::Image _imageToShowInView;
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
    td::INT4 _paramFrom;
    td::INT4 _paramTo;
public:
    ViewWithTable()
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
    , _imageToShowInView(":UNSA")
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
    , _paramTo(500000)
    {
        _imgView.setImage(&_imageToShowInView);
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
        initTable(1); //change from 0 to 2 to demonstrate different init types
        
        int iSel = _table.getFirstSelectedRow();
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
        fo::fs::path homePath;
        mu::getHomePath(homePath);
        fo::fs::path testDBPath = (homePath / "other_bin/TestData/natGUITest/Test.db");
        
        if (!_db->connect(testDBPath.string().c_str()))
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
            par << elemID << strVar << quant << price << date << time << r4 << r8;
            par << colorID;
            dp::Transaction tr(_db.ptr());
            //delete some data
            dp::IStatementPtr pDel(_db->createStatement("DELETE FROM Test"));
            if (!pDel->execute())
                return;
            rnd::MinMax<td::UINT2> rndColor(0, (td::UINT2) td::ColorID::SysText);
            rnd::MinMax<int> rndKolicina(1, 3000000);
            rnd::MinMax<int> rndCijena(1, 20000000);
            
            for (int i=0;i<500000; ++i)
            {
                elemID = (i+1);
                colorID = rndColor.next();
                int cij = rndCijena.next();
                double dCij = cij / 10000.;
                td::Decimal4 dec4(dCij);
                price = dec4;
                int kol = rndKolicina.next();
                double dKol = kol / 1000.;
                td::Decimal3 dec3(dKol);
                quant = dec3;
                strName.format("Gen name for id=%d", elemID);
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
    
    virtual bool onChangedSelection(gui::TableEdit* pTE)
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
    
    virtual bool onClick(gui::Button* pBtn)
    {
        if (pBtn == &_btnReload)
        {
            _paramFrom = _fromID.getValue().i4Val();
            _paramTo = _toID.getValue().i4Val();
            _table.reload();
            return true;
        }
        
        if (pBtn == &_btnLogo)
        {
            if (_imgView.getType() == gui::ImageView::Type::Image)
                _imgView.setImage(&_symbolToShowInView);
            else
                _imgView.setImage(&_imageToShowInView);
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
    
};
