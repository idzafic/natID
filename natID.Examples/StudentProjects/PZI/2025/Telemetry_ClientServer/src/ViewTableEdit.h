#pragma once
#include <gui/View.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <gui/TableEdit.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <fo/FileOperations.h>
#include <gui/ImageView.h>
#include <gui/ComboBox.h>
#include "Data.h"

class ViewTableEdit : public gui::View{     //shows all the data in the database in one table edit
private:  
protected:
    gui::TableEdit table;
    gui::GridLayout gl;
    dp::IDataSetPtr pDS;
    std::vector<td::String> dataTypes;
public:
    ViewTableEdit():
        table(td::Ownership::Extern, gui::TableEdit::RowNumberVisibility::VisibleOneBased)
        , gl(1,1)
    {
        gui::GridComposer gc(gl);
        gc.appendRow(table);
        setLayout(&gl);
        populateData();
        initTable();
    }

protected:
    
    void initTable()
    {
        gui::Columns visCols(table.allocBindColumns(4));
        visCols << gui::ThSep::DoNotShowThSep << gui::Header(0, "measurement_index_id", "measurement_index_id")
                << gui::Header(1, "run_id", "run_id", 80, td::HAlignment::Right)
                << gui::Header(2, "name", "name", 150, td::HAlignment::Right)
                << gui::Header(3, "value", "value", 80, td::HAlignment::Right);
        table.init(pDS);
    }

    void populateData()
    {
        pDS=nullptr;
        pDS = Data::instance().getDB()->createDataSet("SELECT m.measurement_index_id, m.run_id, dt.name, m.value FROM measurement m, data_type dt WHERE m.data_type_id==dt.id ORDER BY run_id, measurement_index_id;", dp::IDataSet::Execution::EX_MULT);

        dp::DSColumns cols(pDS->allocBindColumns(4));

        cols << "measurement_index_id" << td::int4 << "run_id"<< td::int4 <<"name"<<td::string8<< "value" << td::real4;
        if (!pDS->execute())
        {
            //show log
            mu::dbgLog("d");
            pDS = nullptr;
            return;
        }
    }
    
};
