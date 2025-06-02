#include <iostream>
#include "Prodaja.h"
#include <cnt/PushBackVector.h>

using DataType1 = int;
using DataType2 = float;
using DataType3 = int;

bool createConnection()
{
    //should be executed once
    dp::IDatabasePtr pDB(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3));
    if (!pDB->connect("PathDoDatabase/dbName.db"))
        return false;
    return true;
}

class DataStructure
{
public:
    DataType1 valCol1;
    DataType2 valCol2;
    DataType3 valCol3;
};

//pseudo algorithm
bool pseudoInsert(dp::IDatabasePtr& pDB, const cnt::SafeFullVector<DataStructure>& inputDataCnt) {
    //create insert statement
    dp::IStatementPtr pStat(pDB->createStatement("INSERT INTO TableName (Col1,Col2,Col3) VALUES(?,?,?)"));
    //declare local variables from which data will be transferred as parameters.
    DataType1 localVar1;
    DataType2 localVar2;
    DataType3 localVar3;
    //bind local variables to parameters.
    dp::Params par(pStat->allocParams());
    par << localVar1 << localVar2 << localVar3;
    dp::Transaction tr(pDB); //transaction start
    size_t nRecordsToTransfer = inputDataCnt.size();
    for (size_t i=0; i<nRecordsToTransfer; ++i)
    {
        //set the values of local variables.
        localVar1 = inputDataCnt[i].valCol1; //get the value of the first variable (parameter)
        localVar2 = inputDataCnt[i].valCol2; //get the value of the second variable (parameter)
        localVar3 = inputDataCnt[i].valCol3; //get the value of the third variable (parameter)
        if (!pStat->execute()) //execute takes the values of local variables as parameters.
        {
            tr.rollBack(); //the first thing to do if something is not okay.
            td::String strErr;
            pStat->getErrorStr(strErr); //get error description
            std::cout << "Greska! " << strErr << td::endl; //show the error description
            return false;
        }
    }
    tr.commit(); //finalize (save) changes 
    return true;
}

bool pseudoSelect(dp::IDatabasePtr& pDB, cnt::PushBackVector<DataStructure>& dataCont, DataType3 par1){
    //create SELECT statement
    dp::IStatementPtr pStat(pDB->createStatement("SELECT Col1, Col2 FROM TableName WHERE Col3=?"));
    // This can also be done this way if Col1 and Col2 are included in the returned set
    // dp::IStatementPtr pStat(pDB->createStatement("SELECT * FROM TableName WHERE Col=?"));
    // (the variant with the explicitly listed columns is better)

    // Bind local variables to parameters
    DataType3 localVarForParam = par1; // The parameter is set only once for a select
    dp::Params par(pStat->allocParams());  //alloc input parameters
    par << localVarForParam;  //insert input parameters
    //bind local variables to parameters.
    DataType1 localVarForCol1;
    DataType2 localVarForCol2;
    dp::Columns cols(pStat->allocBindColumns(2)); //allocate output columns 
    cols << "Col1" << localVarForCol1 << "Col2" << localVarForCol2; //insert columns
    if (!pStat->execute()) //just once for a select
        return false;   //add here error processing
    while (pStat->moveNext()) // Iterate through the returned dataset
    {
        // Assign the values of local variables to the return data vector
        DataStructure dataStrucure;
        dataStrucure.valCol1 = localVarForCol1;
        dataStrucure.valCol2 = localVarForCol2;
        dataCont.push_back(strukturaPodataka); //store data in output container
    }
    return true;
}
