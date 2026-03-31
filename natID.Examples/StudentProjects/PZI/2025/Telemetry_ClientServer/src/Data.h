#pragma once
#include "gui/Application.h"
#include <gui/SplitterLayout.h>
#include <gui/View.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <dp/IDataSetDelegate.h>
#include <fo/FileOperations.h>

class Data : public gui::View{

    dp::IDatabasePtr db;
    std::vector<td::String> dataTypes;

    void populateDataTypes(){
        dp::IDataSetPtr pDS = db->createDataSet("SELECT * from data_type", dp::IDataSet::Execution::EX_MULT);
        int id;
        td::String name;
        dp::DSColumns cols(pDS->allocBindColumns(2));
        cols << "id" << id << "name" << name;                   //finds all the data types in the database and stores them in a vector
        if (!pDS->execute())                                    //so they can be used whenever needed without having to query the
        {                                                       //databese again every time
            pDS = nullptr;
            return;
        }

        while(pDS->moveNext()){
            dataTypes.push_back(name);
        }
    }

    void Connect(){
        gui::Application* pApp = getApplication();
        fo::fs::path dbPath;
        std::string path= pApp->getFolderPath().string();
        path=path+"/res";
        if (!pApp->getUserDataFilePath("measure.db", path.c_str(), dbPath))
        {
            mu::dbgLog("ERROR! Database file %s does not exist!", dbPath.c_str());       //connects to the database
            return;
        }

        if (!db->connect(dbPath.string().c_str()))
            return;
    }

public:

    Data():db(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3))
    {
        Connect();
        populateDataTypes();
    }

    Data(const Data&) = delete;
    Data& operator=(const Data&) = delete;

    static Data& instance() {       //makes it so that the entire class can be used as a static variable over the entire program
        static Data instance;       //without having to be instanced every time
        return instance;
    }

    dp::IDatabasePtr& getDB(){return db;} //returns a poiter to the database so queryes can be made by other functions over the same database connection

    std::vector<td::String> getTypes(){return dataTypes;} //returns the vector with all the types of data in the database
};
