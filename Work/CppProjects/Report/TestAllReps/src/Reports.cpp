#include "Reports.h"
#include "ReportData.h"
#include <rpt/IViewer.h>
#include <dp/IDataSetDelegate.h>
#include <gui/ContextMenu.h>

void populateCustomerData(dp::IDatabase* pDB, rpt::ReportData::iterator& it, td::INT4 customerID)
{
//	dp::IDataSet* pDP(pDB->createDataSet(dp::IDataSet::Size::SingleRow, "RepCustomer"));
    //
    dp::IDataSet* pDP(dp::createConnectionlessDataSet(dp::IDataSet::Size::SingleRow));
    
    dp::DSColumns cols(pDP->allocBindColumns(9));
        cols << "CustomerID" << td::int4 << "GID" << td::string8 << "TaxID" << td::string8 << "Name" << td::string8 <<  "Subname" << td::string8 << "Street" << td::string8 << "Post" << td::string8
    << "Town" << td::string8 << "Country" << td::string8;
    
    if (!pDP->execute())
    {
        assert(false);
        return;
    }
            
	it << rpt::ContainerType::CNT_DocHeader << rpt::ContainerType::CNT_Group << "Customer" << pDP;
    auto nRows = pDP->getNumberOfRows();
	dp::IDataSet::Row row(pDP->getRow(0));
	row[0] = customerID; //CustomerID
	row[1] = "4062592827172"; //GID (13 digits)
	row[2] = "206154431123"; //TaxID (12 digits)	
	row[3] = "Siemens AG"; //Name
	row[4] = "Smart Grid Division"; //Subname
	row[5] = "Humboldtstr. 59"; //Street
	row[6] = "90459"; //Post
	row[7] = "Nurnberg"; //Town
	row[8] = "Germany"; //Country
    
//    pDP->push_back();
}

void populateObjectData(dp::IDatabase* pDB, rpt::ReportData::iterator& it)
{
//	dp::IDataSet* pDP(pDB->createDataSet(dp::IDataSet::Size::SingleRow, "RepInvoiceData"));
    dp::IDataSet* pDP(dp::createConnectionlessDataSet(dp::IDataSet::Size::SingleRow));
    
    dp::DSColumns cols(pDP->allocBindColumns(5));
    cols << "PaymentType" << td::string8 << "ProInvoiceID" << td::string8 << "ProInvoiceDate" << td::date << "AgreementID" << td::string8 <<  "AgreementDate" << td::date;
    
    if (!pDP->execute())
    {
        assert(false);
        return;
    }
    
	it << rpt::ContainerType::CNT_DocHeader << rpt::ContainerType::CNT_Group << "InvoiceData" << pDP;
    
	dp::IDataSet::Row row(pDP->getRow(0));
	td::Date proInvoiceDate(2015, 7, 22);
	td::Date agreementDate(2015, 8, 22);
	row[0] = "Bank transfer"; //PaymentType
	row[1] = "M1P102/15"; //ProInvoiceID
	row[2] = proInvoiceDate; //ProInvoiceDate
	row[3] = "M1A409/15"; //AgreementID
	row[4] = agreementDate; //AgreementDate	
    
//    pDP->push_back();
}

void populateFromToCardData(dp::IDatabase* pDB, rpt::ReportData::iterator& it, td::INT4 fromID, td::INT4 toID)
{
//		dp::IDataSet* pDP(pDB->createSingleRowDataSet("/")); //sta ovdje??
    dp::IDataSet* pDP = dp::createConnectionlessDataSet(dp::IDataSet::Size::SingleRow);

    dp::DSColumns cols(pDP->allocBindColumns(2));
    cols << "FromID" << td::int4  << "ToID" << td::int4;
    
    if (!pDP->execute())
    {
        assert(false);
        return;
    }
    
    it << rpt::ContainerType::CNT_DocHeader << pDP;

    dp::IDataSet::Row row(pDP->getRow(0));
    row[0] = fromID; //FromID
    row[1] = toID; //ToID
    
//    pDP->push_back();

}

//#endif

void racunV1(const gui::Image* pImage)
{

	dp::IDatabase* pDB = dp::getMainDatabase();

//#ifdef REPTEST
	DocumentData docData;
	CompanyData companyData;
	ObjectData objectData;

	rpt::ReportData repData(3, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

	rpt::ReportData::iterator it(repData.begin());
	////it.setLayoutNo(0); no need to call this one

	//Customer DataGroup
	populateCustomerData(pDB, it, 100);

	++it;
	//Object DataGroup
	populateObjectData(pDB, it);

	++it;
//#endif
	//Body 
	{
		td::LUINT8 guid = 2000;
		dp::IDataSet* pDP(pDB->createDataSet("SELECT N.Name as ItemName, D.Quantity, D.Price, D.ItemID, CAST((D.Quantity * D.Price) as integer) as Value from DocuItems D, Naming N, Items I, Tax T WHERE D.GUID1 = ? and D.GUID2 = ? and N.NT = D.ItemNT and N.ID = D.ItemID and I.NT = D.ItemNT and I.ID = ItemID and T.ID = I.TaxID"));

//#ifdef REPTEST
		it << rpt::ContainerType::CNT_Body << pDP; //define field and its data
//#endif
		dp::DSColumns cols(pDP->allocBindColumns(5));
		cols << "ItemName" << td::string8
			<< "Quantity" << td::decimal3
			<< "Price" << td::decimal4
			<< "ItemID" << td::int4
			<< "Value" << td::decimal2;
		dp::Params pars = pDP->allocParams();
		pars << guid << guid;
        

		if (!pDP->execute())//sto ovdje nece ne znam...
		{
			return;
		}


		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{		
			return;
		}
	}

	//InvoiceSimple
    td::String configName("InvoiceSimple");

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr("InvoiceSimple"), pImage);
            
    }

}

void racunV2(const gui::Image* pImage)
{
	dp::IDatabase* pDB = dp::getMainDatabase();

	DocumentData docData;
	CompanyData companyData;
	ObjectData objectData;

	rpt::ReportData repData(4, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

	rpt::ReportData::iterator it(repData.begin());
	////it.setLayoutNo(0);

	//Customer DataGroup
	populateCustomerData(pDB, it, 200);

	//Object DataGroup
	++it;
	populateObjectData(pDB, it);
	++it;

	//Body 
	{
		td::LUINT8 guid = 2000;
		dp::IDataSet* pDP(pDB->createDataSet("SELECT D.*, CAST((D.Quantity * D.Price) as integer) as Value, N.Name as ItemName, I.Unit, I.EANCode, T.Value as VATPerc, I.TaxID"
			" from DocuItems D, Naming N, Items I, Tax T"
			" WHERE D.GUID1 = ? and D.GUID2 = ? and N.NT = D.ItemNT and N.ID = D.ItemID and I.NT = D.ItemNT and I.ID = ItemID and T.ID = I.TaxID"));

		it << rpt::ContainerType::CNT_Body << pDP; //define field and its data
		dp::DSColumns cols(pDP->allocBindColumns(9));
		cols << "ItemName" << td::string8 
			<< "Quantity" << td::decimal3
			<< "Price" << td::decimal4
			<< "ItemID" << td::int4
			<< "EANCode" << td::lint8
			<< "Unit" << td::string8
			<< "Value" << td::decimal2
			<< "TaxID" << td::int2
			<< "VATPerc" << td::smallDecimal1;

		dp::Params pars = pDP->allocParams();
		pars << guid << guid;

		if (!pDP->execute())//sto ovdje nece ne znam...
		{
			return;
		}

		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			return;
		}
	}
	//Note example
	td::String note = getNote();
	++it;
	{
		it << rpt::ContainerType::CNT_DocFooter << rpt::ShapeType::SHP_Note << "Note" << &note;
	}

	//InvoiceWoDiscount
    td::String configName("InvoiceWoDiscount");

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr("InvoiceWoDiscount"), pImage);
    }
}

bool prepareInvoice(rpt::ReportData& repData, bool addAssortment)
{
	dp::IDatabase* pDB = repData.getDB();
	rpt::ReportData::iterator it(repData.begin());
	////it.setLayoutNo(0);

	//Customer DataGroup
	populateCustomerData(pDB, it, 300);

	//Object DataGroup
	++it;
	populateObjectData(pDB, it);
	td::LUINT8 guid = 2000;
	//Body 
	++it;
	{
		
		dp::IDataSet* pDP(pDB->createDataSet("SELECT D.*, CAST((D.Quantity * D.Price) as integer) as Value, N.Name as ItemName, I.Unit, I.EANCode, T.Value as VATPerc, I.TaxID"
			" from DocuItems D, Naming N, Items I, Tax T"
			" WHERE D.GUID1 = ? and D.GUID2 = ? and N.NT = D.ItemNT and N.ID = D.ItemID and I.NT = D.ItemNT and I.ID = ItemID and T.ID = I.TaxID"));
		it << rpt::ContainerType::CNT_Body << pDP; //define field and its data

		dp::DSColumns cols(pDP->allocBindColumns(9));
		cols << "ItemName" << td::string8
			<< "Quantity" << td::decimal3
			<< "Price" << td::decimal4
			<< "ItemID" << td::int4
			<< "EANCode" << td::lint8
			<< "Unit" << td::string8
			<< "Value" << td::decimal2
			<< "TaxID" << td::int2
			<< "VATPerc" << td::smallDecimal1;

		dp::Params pars = pDP->allocParams();
		pars << guid << guid;


		if (!pDP->execute())
		{
			return false;
		}

		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{			
			return false;
		}
	}

	//VAT Spec 
	++it;
	{
		dp::IDataSet* pDP(pDB->createDataSet("SELECT I.TaxID, T.Name, T.Value as VATPerc, SUM(CAST((D.Quantity * D.Price) as integer)) as Value," 
			" SUM(CAST(CAST((D.Quantity * D.Price) as integer)*(1.0-1.0/(1+T.Value/100.0)) as integer)) as VATValue"
			" from DocuItems D, Items I, Tax T"
			" WHERE D.GUID1 = ? and D.GUID2 = ? and I.NT = D.ItemNT and I.ID = D.ItemID and T.ID = I.TaxID"
			" GROUP BY I.TaxID, T.Name, T.Value"));
		it << rpt::ContainerType::CNT_DocFooter << rpt::ContainerType::CNT_Table << "PDVSpec" << rpt::ContainerType::CNT_Body << pDP; //define field and its data

		dp::DSColumns cols(pDP->allocBindColumns(5));
		cols << "TaxID" << td::int2
			<< "Name" << td::string8
			<< "VATPerc" << td::smallDecimal1
			<< "VATValue" << td::decimal2
			<< "Value" << td::decimal2;

		dp::Params pars = pDP->allocParams();
		pars << guid << guid;

		if (!pDP->execute())
		{
			return false;
		}

		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			return false;
		}
	}

	if (addAssortment)
	{
		++it;
		{
			dp::IDataSet* pDP(pDB->createDataSet("SELECT A.ID, A.Name, SUM(CAST((D.Quantity * D.Price) as integer)) as Value,"
				" SUM(CAST(CAST((D.Quantity * D.Price) as integer)*(1-1/(1+T.Value/100.0)) as integer)) as VATValue"
				" from DocuItems D, Items I, Assortment A, Tax T"
				" WHERE D.GUID1 = ? and D.GUID2 = ? and I.NT = D.ItemNT and I.ID = D.ItemID and T.ID = I.TaxID and I.IDAssort = A.ID"
				" GROUP BY A.ID, A.Name"));
			it << rpt::ContainerType::CNT_DocFooter << rpt::ContainerType::CNT_Table << "AssortSpec" << rpt::ContainerType::CNT_Body << pDP; //define field and its data



			dp::DSColumns cols(pDP->allocBindColumns(4));
			cols << "ID" << td::int2
				<< "Name" << td::string8
				<< "VATValue" << td::decimal2
				<< "Value" << td::decimal2;

			dp::Params pars = pDP->allocParams();
			pars << guid << guid;

			if (!pDP->execute())
			{
				return false;
			}

			size_t nRows = pDP->getNumberOfRows();
			if (nRows == 0)
			{
				return false;
			}
		}
	}

	return true;
}


void racunV3(const gui::Image* pImage)
{
	dp::IDatabase* pDB = dp::getMainDatabase();

	DocumentData docData;
	CompanyData companyData;
	ObjectData objectData;

	rpt::ReportData repData(5, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

	if (!prepareInvoice(repData, false))
		return;

	//Note 
	rpt::ReportData::iterator it(repData.at(4));
	////it.setLayoutNo(0);

	td::String note = getNote();	//note is local variable, can be sent to report only from this point
	{
		it << rpt::ContainerType::CNT_DocFooter << rpt::ShapeType::SHP_Note << "Note" << &note;
	}

	//InvoiceWoDiscount_withVATSpec
    td::String configName("InvoiceWoDiscount_withVATSpec");

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr("InvoiceWoDiscount_withVATSpec"), pImage);
    }

}

void racunV4(const gui::Image* pImage)
{
	dp::IDatabase* pDB = dp::getMainDatabase();


	DocumentData docData;
	CompanyData companyData;
	ObjectData objectData;

	rpt::ReportData repData(7, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

	if (!prepareInvoice(repData, true))
		return;

	//Note 
	rpt::ReportData::iterator it(repData.at(5));
	//it.setLayoutNo(0);

	td::String note = getNote();	//note is local variable, can be sent to report only from this point
	{
		it << rpt::ContainerType::CNT_DocFooter << rpt::ShapeType::SHP_Note << "Note" << &note;
	}

	//Signature
	++it;

	{
        dp::IDataSet* pDP = dp::createConnectionlessDataSet(dp::IDataSet::Size::SingleRow);

		it << rpt::ContainerType::CNT_DocFooter << rpt::ContainerType::CNT_Row << "Signature" << pDP; //define field and its data
		dp::DSColumns cols(pDP->allocBindColumns(3));
		cols << "PreparedBy" << td::string8
			<< "CtrlBy" << td::string8
			<< "ApprovedBy" << td::string8;
        
        pDP->execute();
        
		dp::IDataSet::Row row(pDP->getRow(0));
		row[0] = "Meho Mehic"; //CustomerID
		row[1] = "Juso Jusic"; //Name
		row[2] = "Kemo Kemic"; //Subname
//        pDP->push_back();
	}
    
	//InvoiceWoDiscount_withVATSpecAndAssort
    td::String configName("InvoiceWoDiscount_withVATSpecAndAssort");

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr("InvoiceWoDiscount_withVATSpecAndAssort"), pImage);
    }
}


void racunParallelTables(const gui::Image* pImage)
{

	dp::IDatabase* pDB = dp::getMainDatabase();

	DocumentData docData;
	CompanyData companyData;
	ObjectData objectData;

	rpt::ReportData repData(9, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

	if (!prepareInvoice(repData, true))
		return;

	//Note 
	rpt::ReportData::iterator it(repData.at(5));
	//it.setLayoutNo(0);

	td::String note = getNote();	//note is local variable, can be sent to report only from this point
	{
		it << rpt::ContainerType::CNT_DocFooter << rpt::ShapeType::SHP_Note << "Note" << &note;
	}

	//Signature
	++it;

	{
		dp::IDataSet* pDP = dp::createConnectionlessDataSet(dp::IDataSet::Size::SingleRow);

		it << rpt::ContainerType::CNT_DocFooter << rpt::ContainerType::CNT_Row << "Signature" << pDP; //define field and its data
		dp::DSColumns cols(pDP->allocBindColumns(3));
		cols << "PreparedBy" << td::string8
			<< "CtrlBy" << td::string8
			<< "ApprovedBy" << td::string8;

        pDP->execute();
		dp::IDataSet::Row row(pDP->getRow(0));
		row[0] = "Meho Mehic"; //CustomerID
		row[1] = "Juso Jusic"; //Name
		row[2] = "Kemo Kemic"; //Subname
//        pDP->push_back();
	}

	//positive entries
	++it;


	{
		dp::IDataSet* pDP(pDB->createDataSet("select N.Name, I.Datum, I.Mult, I.Quantity, I.Price"
			" from Naming N, ItemsCardDemo I"
			" where N.NT = I.ItemNT and N.ID = I.ItemID and I.Datum >= ? and I.Datum <= ? and I.Mult = ?"
			" ORDER BY I.Datum"));
        
        
//        dp::IDataSet* pDP(pDB->createDataSet("SELECT N.Name, I.Datum, I.Mult, I.Quantity, I.Price FROM Naming N, ItemsCardDemo I "
//                                             "WHERE N.NT = I.ItemNT and N.ID = I.ItemID ORDER BY I.Datum"));

		it << rpt::ContainerType::CNT_DocFooter << rpt::ContainerType::CNT_Table << "PosEntries" << rpt::ContainerType::CNT_Body << pDP;

		dp::DSColumns cols(pDP->allocBindColumns(5));
		cols << "Datum" << td::date
			<< "Name" << td::string8
			<< "Quantity" << td::decimal3
			<< "Price" << td::decimal4
			<< "Mult" << td::int4;

		td::Date fromDate(2015, 1, 1);
		td::Date toDate;
		toDate.now();
		td::INT4 mult = 1;

		dp::Params pars = pDP->allocParams();
		pars << fromDate << toDate << mult;

		if (!pDP->execute())
		{
            td::String strErr = pDP->getErrorStr();
			return;
		}
		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			return;
		}
	}

	//all entries
	++it;
	{
		dp::IDataSet* pDP(pDB->createDataSet("select N.Name, I.Datum, I.Mult, I.Quantity, I.Price"
			" from Naming N, ItemsCardDemo I"
			" where N.NT = I.ItemNT and N.ID = I.ItemID and I.Datum >= ? and I.Datum <= ?"
			" ORDER BY I.Datum"));

		it << rpt::ContainerType::CNT_DocFooter << rpt::ContainerType::CNT_Table << "AllEntries" << rpt::ContainerType::CNT_Body << pDP;
		dp::DSColumns cols(pDP->allocBindColumns(5));
		cols << "Datum" << td::date
			<< "Name" << td::string8
			<< "Quantity" << td::decimal3
			<< "Price" << td::decimal4
			<< "Mult" << td::int4;

		td::Date fromDate(2010, 1, 1);
		td::Date toDate;
		toDate.now();

		dp::Params pars = pDP->allocParams();
		pars << fromDate << toDate;

		if (!pDP->execute())
		{
            td::String strErr = pDP->getErrorStr();
            int k=0;
            return;
		}
		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			return;
		}
	}
	//
    td::String configName("InvoiceParallelTables");

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr("InvoiceParallelTables"), pImage);
    }
}


void racunV5(const gui::Image* pImage, bool forExport)
{
	dp::IDatabase* pDB = dp::getMainDatabase();

	DocumentData docData;
	CompanyData companyData;
	ObjectData objectData;

	rpt::ReportData repData(7, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

	if (!prepareInvoice(repData, true))
		return;

	//Note 
	rpt::ReportData::iterator it(repData.at(5));
	//it.setLayoutNo(0);

	td::String note = getNote();	//note is local variable, can be sent to report only from this point
	{
		it << rpt::ContainerType::CNT_DocFooter << rpt::ShapeType::SHP_Note << "Note" << &note;
	}

	//Signature
	++it;

	{
		dp::IDataSet* pDP(dp::createConnectionlessDataSet(dp::IDataSet::Size::SingleRow)); //sta ovdje??

		it << rpt::ContainerType::CNT_DocFooter << rpt::ContainerType::CNT_Row << "Signature" << pDP; //define field and its data
		dp::DSColumns cols(pDP->allocBindColumns(3));
		cols << "PreparedBy" << td::string8
			<< "CtrlBy" << td::string8
			<< "ApprovedBy" << td::string8;

        pDP->execute();
		dp::IDataSet::Row row(pDP->getRow(0));
		row[0] = "Meho Mehic"; //CustomerID
		row[1] = "Juso Jusic"; //Name
		row[2] = "Kemo Kemic"; //Subname	
//        pDP->push_back();
	}

	////Read RT Strings
	//mu::RTStringManager rtStrings;

	//fo::fs::path resPath(mu::getAppSettings()->getResPath());
	//if (!rtStrings.load(resPath, "RTStrings.xml"))
	//{
	//	return;
	//}

	//repData.setRTStringManager(&rtStrings);

	const char* repName = "Invoice_Domestic";
	if (forExport)
		repName = "Invoice_Export";
    
    td::String configName(repName);

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr(repName), pImage);
    }

	//repName

}


class DSCardDelegate : public dp::IDataSetDelegate
{
protected:
    td::Decimal3 quantSaldo = 0;
    td::Decimal2 valueSaldo = 0;
    int _colQuant = -1;
    int _colMult = -1;
    int _colPrice = -1;
    int _colQuantitySaldo = -1;
    int _colValueSaldo = -1;
public:
    
    void init(dp::IDataSet* pDS) override
    {
        _colQuant = pDS->getColIndex("Quantity");
        assert(_colQuant >= 0);
        _colMult = pDS->getColIndex("Mult");
        assert(_colMult >= 0);
        _colPrice = pDS->getColIndex("Price");
        assert(_colPrice >= 0);
        _colQuantitySaldo = pDS->getColIndex("fSaldo");
        assert(_colQuantitySaldo >= 0);
        _colValueSaldo = pDS->getColIndex("fVSaldo");
        assert(_colValueSaldo >= 0);
    }
    
    bool onLoad(dp::IDataSet* pDS, dp::RowWrapper& row, size_t) override
    {
        td::Variant quant(row[_colQuant]);
        td::Variant price(row[_colPrice]);
        td::Decimal2 decQuantity = quant.dec3Val();
        td::Decimal2 value = decQuantity * price.dec4Val();
        td::Variant mult(row[_colMult]);
        //if mult > 0: input (hence increase stock), else output (decrease value and quantity)
        if (mult.i2Val() > 0)
        {
            quantSaldo += quant.dec3Val();
            valueSaldo += value;
        }
        else
        {
            quantSaldo -= quant.dec3Val();
            valueSaldo -= value;
        }
        
        row[_colQuantitySaldo].setValue(quantSaldo);
        row[_colValueSaldo].setValue(valueSaldo);
        return true;
    }
};

static DSCardDelegate s_cardDelegate;

void karticaReport1(const gui::Image* pImage)
{
	td::Date fromDate(2015, 1, 5);
	td::Date toDate;
	toDate.now();
	td::INT4 itemID = 387001;
	td::String itemName("Item 387001");

	dp::IDatabase* pDB = dp::getMainDatabase();

	DocumentData docData;
	CompanyData companyData;
	ObjectData objectData;

	docData[RPT_TXT_DOCUMENT_TYPE] = "Kartica robe";
	docData[RPT_TXT_DOCUMENT_ID] = "";
	docData[RPT_DT_DOCUMENT_FROM_DATETIME] = td::DateTime(fromDate);
	docData[RPT_DT_DOCUMENT_TO_DATETIME] = td::DateTime(toDate);

	rpt::ReportData repData(2, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

	rpt::ReportData::iterator it(repData.begin());
	//it.setLayoutNo(0);
    
	//Object DataGroup
	{
        dp::IDataSet* pDP(dp::createConnectionlessDataSet(dp::IDataSet::Size::SingleRow));
		it << rpt::ContainerType::CNT_DocHeader << pDP; //define field and its data
		dp::DSColumns cols(pDP->allocBindColumns(2));
		cols << "ID" << td::int4
			<< "Name" << td::string8;
        
        pDP->execute();
		
        dp::IDataSet::Row row(pDP->getRow(0));
		row[0] = itemID;
		row[1] = itemName;
//        pDP->push_back();
	}

	//Body 
	++it;
	{
		dp::IDataSet* pDP(pDB->createDataSet("SELECT Datum, CAST('M1R174082/15' as text) as DocID, Mult, Quantity, Price from ItemsCardDemo"
			" WHERE Datum >= ? and Datum <= ? and ItemNT = ? and ItemID = ?"
			" ORDER BY [Datum]"));

		it << rpt::ContainerType::CNT_Body << pDP; //define field and its data

		dp::DSColumns cols(pDP->allocBindColumns(7));
		cols << "Datum" << td::date
			<< "DocID" << td::string8
			<< "Mult" << td::int2
			<< "Quantity" << td::decimal3 
			<< "Price" << td::decimal4
			<< "fSaldo" << dp::DSColumn::Type::CalcOnLoad << td::decimal3
			<< "fVSaldo" << dp::DSColumn::Type::CalcOnLoad<< td::decimal2;

		td::INT2 namingType = 1;
		td::INT4 namingID = 387001;

		dp::Params pars = pDP->allocParams();
		pars << fromDate << toDate << namingType << namingID;

//        pDP->setDelegate(&s_cardDelegate);
        
		if (!pDP->execute())
		{
            auto errStr  = pDP->getErrorStr();
            int k = 0;
			return;
		}

		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			return;
		}
	}

	//SingleCard_v1
    td::String configName("SingleCard_v1");

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr("SingleCard_v1"), pImage);
    }
}


void multiLayout(const gui::Image* pImage, int removeTestCase)
{
	td::Date fromDate(2015, 1, 5);
	td::Date toDate;
	toDate.now();
	td::INT2 namingType = 1;
	td::INT4 itemID = 387001;
	td::String itemName("Item 387001");

	dp::IDatabase* pDB = dp::getMainDatabase();


	DocumentData docData;
	CompanyData companyData;
	ObjectData objectData;

	docData[RPT_TXT_DOCUMENT_TYPE] = "Multi-Layout";
	docData[RPT_TXT_DOCUMENT_ID] = "";
	docData[RPT_DT_DOCUMENT_FROM_DATETIME] = td::DateTime(fromDate);
	docData[RPT_DT_DOCUMENT_TO_DATETIME] = td::DateTime(toDate);

	cnt::SafeFullVector<int> layoutsToBeRemovedFromConfig; //0-based, restriction first layout cannot be removed

	int numberOfDataProviders = 6;

	if (removeTestCase == 1)
	{
		//TestCase1 - remove layout 1
		//layout 1 uses 1 data provider
		numberOfDataProviders -= 1;
		layoutsToBeRemovedFromConfig.reserve(1);
		layoutsToBeRemovedFromConfig(0) = 1;
	}
	else if (removeTestCase == 2)
	{
		//TestCase2 - remove layout 2
		//layout 2 uses 1 data provider
		numberOfDataProviders -= 1;
		layoutsToBeRemovedFromConfig.reserve(1);
		layoutsToBeRemovedFromConfig(0) = 2;
	}
	else if (removeTestCase == 3)
	{
		//TestCase3 - remove layout 3
		//layout 2 uses 1 data provider
		numberOfDataProviders -= 1;
		layoutsToBeRemovedFromConfig.reserve(1);
		layoutsToBeRemovedFromConfig(0) = 3;
	}
	else if (removeTestCase == 4)
	{
		//TestCase4 - remove layout 1 and 3
		//layout 1- uses 1 DP, layout 3 uses 1 data provider -> in total 2 DP
		numberOfDataProviders -= 2;
		layoutsToBeRemovedFromConfig.reserve(2);
		layoutsToBeRemovedFromConfig(0) = 1;
		layoutsToBeRemovedFromConfig(1) = 3;
	}


	rpt::ReportData repData(numberOfDataProviders, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());
	repData.setLayoutsToBeRemovedFromConfig(&layoutsToBeRemovedFromConfig);

	rpt::ReportData::iterator it(repData.begin());
	//it.setLayoutNo(0);#

	//CardFilter DataGroup 0
	{
		//Object DataGroup
		{
            dp::IDataSet* pDP(dp::createConnectionlessDataSet(dp::IDataSet::Size::SingleRow));
			it << rpt::ContainerType::CNT_DocHeader << pDP; //define field and its data
			dp::DSColumns cols(pDP->allocBindColumns(2));
			cols << "ID" << td::int4
				<< "Name" << td::string8;
            pDP->execute();
			dp::IDataSet::Row row(pDP->getRow(0));
			row[0] = itemID;
			row[1] = itemName;
//            pDP->push_back();
		}
	}
	//Header - donos , 1
	++it;

	{
		dp::IDataSet* pDP = pDB->createSmallDataSet("SELECT SUM(Mult * Quantity) as Quantity, SUM(CAST(Mult * Quantity * Price as integer)) as Value from ItemsCardDemo "
			" WHERE [Datum] < ? and ItemNT = ? and ItemID = ? ");

		it << rpt::ContainerType::CNT_Header << pDP; //define field and its data
        
		dp::DSColumns cols(pDP->allocBindColumns(2));
		cols << "Quantity" << td::decimal3
			<< "Value" << td::decimal2;

		dp::Params pars = pDP->allocParams();
		pars << fromDate << namingType << itemID;
			
		if (!pDP->execute())
		{
			return;
		}

		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			return;
		}
	}

	//Body , 2
	++it;

	{

		dp::IDataSet* pDP(pDB->createDataSet("SELECT [Datum], CAST('M1R174082/15' as text) as DocID, Mult, Quantity, Price from ItemsCardDemo"
			" WHERE [Datum] >= ? and [Datum] <= ? and ItemNT = ? and ItemID = ?"
			" ORDER BY [Datum]"));


		it << rpt::ContainerType::CNT_Body << pDP; //define field and its data


		dp::DSColumns cols(pDP->allocBindColumns(7));
		cols << "Datum" << td::date
			<< "DocID" << td::string8
			<< "Mult" << td::int2
			<< "Quantity" << td::decimal3
			<< "Price" << td::decimal4
            << "fSaldo" << dp::DSColumn::Type::CalcOnLoad << td::decimal3
            << "fVSaldo" << dp::DSColumn::Type::CalcOnLoad<< td::decimal2;

        

		dp::Params pars = pDP->allocParams();
		pars << fromDate << toDate << namingType << itemID;

        pDP->setDelegate(&s_cardDelegate);

		if (!pDP->execute())
		{
            auto strErr = pDP->getErrorStr();
            int k = 0;
			return;
		}
		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			return;
		}
	}

	if (!layoutsToBeRemovedFromConfig.find(1))
	{

		++it;
		it.nextLayout();

		//second layout - Landscape, 3, positive and negative entries
		{
			dp::IDataSet* pDP(pDB->createDataSet("select N.Name, I.Datum, I.Mult, I.Quantity, I.Price"
				" from Naming N, ItemsCardDemo I"
				" where N.NT = I.ItemNT and N.ID = I.ItemID and I.Datum >= ? and I.Datum <= ? and I.Mult <> ?"
				" ORDER BY I.Datum"));


			it << rpt::ContainerType::CNT_Body << pDP;


			dp::DSColumns cols(pDP->allocBindColumns(5));
			cols << "Datum" << td::date
				<< "Name" << td::string8
				<< "Quantity" << td::decimal3
				<< "Price" << td::decimal4
				<< "Mult" << td::int4;

			td::Date fromDate(2015, 1, 1);
			td::Date toDate;
			toDate.now();
			td::INT4 mult = 0;

			dp::Params pars = pDP->allocParams();
			pars << fromDate << toDate << mult;


			if (!pDP->execute())
			{
				return;
			}
			size_t nRows = pDP->getNumberOfRows();
			if (nRows == 0)
			{
				return;
			}
		}
	}

	if (!layoutsToBeRemovedFromConfig.find(2))
	{

		++it;
		it.nextLayout();

		//third layout - Portrait, only positive entries
		{
			dp::IDataSet* pDP(pDB->createDataSet("select N.Name, I.Datum, I.Mult, I.Quantity, I.Price"
				" from Naming N, ItemsCardDemo I"
				" where N.NT = I.ItemNT and N.ID = I.ItemID and I.Datum >= ? and I.Datum <= ? and I.Mult = ?"
				" ORDER BY I.Datum"));


			it << rpt::ContainerType::CNT_Body << pDP;


			dp::DSColumns cols(pDP->allocBindColumns(5));
			cols << "Datum" << td::date
				<< "Name" << td::string8
				<< "Quantity" << td::decimal3
				<< "Price" << td::decimal4
				<< "Mult" << td::int4;

			td::Date fromDate(2015, 1, 1);
			td::Date toDate;
			toDate.now();

			td::INT4 mult = 1;

			dp::Params pars = pDP->allocParams();
			pars << fromDate << toDate << mult;
			if (!pDP->execute())
			{
				return;
			}
			size_t nRows = pDP->getNumberOfRows();
			if (nRows == 0)
			{
				return;
			}
		}
	}


	if (!layoutsToBeRemovedFromConfig.find(3))
	{

		++it;
		it.nextLayout();

		//fourth layout - Portrait, only negative entries
		{
			dp::IDataSet* pDP(pDB->createDataSet("select N.Name, I.Datum, I.Mult, I.Quantity, I.Price"
				" from Naming N, ItemsCardDemo I"
				" where N.NT = I.ItemNT and N.ID = I.ItemID and I.Datum >= ? and I.Datum <= ? and I.Mult = ?"
				" ORDER BY I.Datum"));


			it << rpt::ContainerType::CNT_Body << pDP;

			dp::DSColumns cols(pDP->allocBindColumns(5));
			cols << "Datum" << td::date
				<< "Name" << td::string8
				<< "Quantity" << td::decimal3
				<< "Price" << td::decimal4
				<< "Mult" << td::int4;

			td::Date fromDate(2015, 1, 1);
			td::Date toDate;
			toDate.now();

			td::INT4 mult = -1;

			dp::Params pars = pDP->allocParams();
			pars << fromDate << toDate << mult;

			if (!pDP->execute())
			{
				return;
			}
			size_t nRows = pDP->getNumberOfRows();
			if (nRows == 0)
			{
				return;
			}
		}
	}

	//MultiLayout

    td::String configName("MultiLayout");

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr("MultiLayout"), pImage);
    }
    
}

void itemsForStoreWindow6x3(const gui::Image* pImage)
{
	dp::IDatabase* pDB = dp::getMainDatabase();

	td::INT2 nt = 1;
	td::INT4 fromID = 0;
	td::INT4 toID = 500000;

	DocumentData docData;
	CompanyData companyData;
	ObjectData objectData;

	docData[RPT_TXT_DOCUMENT_TYPE] = "Robe za izlog";

	rpt::ReportData repData(1, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());
	rpt::ReportData::iterator it(repData.begin());
	//it.setLayoutNo(0);

	//CardFilter DataGroup
	{		

			dp::IDataSet* pDP = pDB->createDataSet("SELECT I.ID, N.Name, CAST('M1N17582/15' as text) as DocID, CAST(I.Price as integer) as Price, I.Unit "
				" from Items I, Naming N "
				" WHERE I.NT = ? and I.ID >= ? and I.ID <= ? and I.NT = N.NT and I.ID = N.ID "
				" ORDER BY I.ID");


			it << rpt::ContainerType::CNT_Replicable << pDP; //define field and its data


		dp::DSColumns cols(pDP->allocBindColumns(5));
		cols << "ID" << td::int4
			<< "Name" << td::string8
			<< "Price" << td::decimal2
			<< "DocID" << td::string8
			<< "Unit" << td::string8;

		td::INT4 mult = 0;

		dp::Params pars = pDP->allocParams();
		pars << nt << fromID << toID;

		if (!pDP->execute())
		{
			return;
		}
		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			return;
		}
	}
	//ShowWindowItems6x3
    td::String configName("ShowWindowItems6x3");

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr("ShowWindowItems6x3"), pImage);
    }

}
//
void itemsForStoreWindow7x3(const gui::Image* pImage)
{
	dp::IDatabase* pDB = dp::getMainDatabase();

	td::INT2 nt = 1;
	td::INT4 fromID = 0;
	td::INT4 toID = 500000;

	DocumentData docData;
	CompanyData companyData;
	ObjectData objectData;

	docData[RPT_TXT_DOCUMENT_TYPE] = "Robe za izlog";

	rpt::ReportData repData(1, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());
	rpt::ReportData::iterator it(repData.begin());
	//it.setLayoutNo(0);

	//CardFilter DataGroup
	{
		dp::IDataSet* pDP = pDB->createDataSet("SELECT I.ID, N.Name, CAST('M1N17582/15' as text) as DocID, CAST(I.Price as integer) as Price, I.Unit "
			" from Items I, Naming N "
			" WHERE I.NT = ? and I.ID >= ? and I.ID <= ? and I.NT = N.NT and I.ID = N.ID "
			" ORDER BY I.ID");


		it << rpt::ContainerType::CNT_Replicable << pDP; //define field and its data


		dp::DSColumns cols(pDP->allocBindColumns(5));
		cols << "ID" << td::int4
			<< "Name" << td::string8
			<< "Price" << td::decimal2
			<< "DocID" << td::string8
			<< "Unit" << td::string8;

		dp::Params pars = pDP->allocParams();
		pars << nt << fromID << toID;

		if (!pDP->execute())
		{
			return;
		}
		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			return;
		}
	}
	//ShowWindowItems7x3
    td::String configName("ShowWindowItems7x3");

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr("ShowWindowItems7x3"), pImage);
    }
}

void dynamicBodyWithParallelTables(const gui::Image* pImage, gui::Frame* pContextManger)
{
	td::INT4 fromID = 0;
	td::INT4 toID = 500000;
	dp::IDatabase* pDB = dp::getMainDatabase();

	DocumentData docData;
	CompanyData companyData;
	ObjectData objectData;

	docData[RPT_TXT_DOCUMENT_TYPE] = "Dynamic body";

	rpt::ReportData repData(4, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());
	rpt::ReportData::iterator it(repData.begin());

	populateFromToCardData(pDB, it, fromID, toID);
	++it;

	{
		dp::IDataSet* pDP = pDB->createDataSet("SELECT I.ItemNT as NT, I.ItemID as ID, I.IngredientID as ExtID, G.Name, I.Value "
			" FROM Items_Ingredients I, Ingredients G "
			" WHERE I.IngredientID = G.ID and I.ItemID >= ? and I.ItemID <= ? "
			" ORDER BY I.ItemNT, I.ItemID, I.IngredientID");   


		it << rpt::ContainerType::CNT_Body << rpt::ContainerType::CNT_Table << "Ingredients" << rpt::ContainerType::CNT_Body << pDP; //define field and its data


		dp::DSColumns cols(pDP->allocBindColumns(5));
		cols << "NT" << td::int2
			<< "ID" << td::int4
			<< "ExtID" << td::int4
			<< "Name" << td::string8
			<< "Value" << td::real8;

		dp::Params pars = pDP->allocParams();
		pars << fromID << toID;



		if (!pDP->execute())
		{
			return;
		}
		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			return;
		}
	}

	++it;

	{
		dp::IDataSet* pDP = pDB->createDataSet("SELECT I.ItemNT as NT, I.ItemID as ID, I.NutritionID as ExtID, N.Name, I.Value "
			" FROM Items_Nutritions I, Nutritions N "
			" WHERE I.NutritionID = N.ID and I.ItemID >= ? and I.ItemID <= ? "
			" ORDER BY I.ItemNT, I.ItemID, I.NutritionID");


		it << rpt::ContainerType::CNT_Body << rpt::ContainerType::CNT_Table << "Nutritions" << rpt::ContainerType::CNT_Body << pDP; //define field and its data


		dp::DSColumns cols(pDP->allocBindColumns(5));
		cols << "NT" << td::int2
			<< "ID" << td::int4
			<< "ExtID" << td::int4
			<< "Name" << td::string8
			<< "Value" << td::real8;

		dp::Params pars = pDP->allocParams();
		pars << fromID << toID;


		if (!pDP->execute())
		{
			return;
		}
		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			return;
		}
	}

	//Body, has to be placed after tables	

	++it;

	{
		dp::IDataSet* pDP = pDB->createDataSet("select N.NT, N.ID, N.Name, I.Quantity, I.Price, T.ID as TaxID, T.Name as TaxName, T.Value as TaxPercent, "
			" A.ID as AssortID, A.Name as AssortName, I.Description "
			" FROM Naming N, Items I, Tax T, Assortment A "
			" where N.NT = I.NT and N.ID = I.ID and I.TaxID = T.ID and I.IDAssort = A.ID and N.ID >= @fromID and N.ID <= @toID "
			" ORDER BY N.NT, N.ID");


		it << rpt::ContainerType::CNT_Body << pDP; //define field and its data				



		dp::DSColumns cols(pDP->allocBindColumns(11));
		cols << "NT" << td::int2
			<< "ID" << td::int4
			<< "Name" << td::string8
			<< "Quantity" << td::decimal3
			<< "Price" << td::decimal4;

		cols << "TaxID" << td::int2
			<< "TaxName" << td::string8
			<< "TaxPercent" << td::smallDecimal2
			<< "AssortID" << td::int2
			<< "AssortName" << td::string8
			<< "Description" << td::string8;

		dp::Params pars = pDP->allocParams();
		pars << fromID << toID;


		if (!pDP->execute())
		{
			return;
		}
		size_t nRows = pDP->getNumberOfRows();
		if (nRows == 0)
		{
			return;
		}
	}

	//this report will contain context menu manager
	repData.setContextManager(pContextManger);

	//ItemDetails_DynamicBody
    td::String configName("ItemDetails_DynamicBody");

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr("ItemDetails_DynamicBody"), pImage);
    }

}



void showSingleCard(const gui::Image* pImage, const char* repConfigName)
{
    td::Date fromDate(2015, 1, 5);
    td::Date toDate;
    toDate.now();
    td::INT2 namingType = 1;
    td::INT4 itemID = 387001;
    td::String itemName("Item 387001");
    dp::IDatabase* pDB = dp::getMainDatabase();

    DocumentData docData;
    CompanyData companyData;
    ObjectData objectData;

    docData[RPT_TXT_DOCUMENT_TYPE] = "Kartica robe";
    docData[RPT_TXT_DOCUMENT_ID] = "";
    docData[RPT_DT_DOCUMENT_FROM_DATETIME] = td::DateTime(fromDate);
    docData[RPT_DT_DOCUMENT_TO_DATETIME] = td::DateTime(toDate);

    rpt::ReportData repData(3, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

    rpt::ReportData::iterator it(repData.begin());

    //it.setLayoutNo(0);


    //CardFilter DataGroup 0
    {
        //Object DataGroup
        {
            dp::IDataSet* pDP(dp::createConnectionlessDataSet(dp::IDataSet::Size::SingleRow));
            it << rpt::ContainerType::CNT_DocHeader << pDP; //define field and its data
            dp::DSColumns cols(pDP->allocBindColumns(2));
            
            cols << "ID" << td::int4
                << "Name" << td::string8;
            pDP->execute();
            auto& row = pDP->getRow(0);
            row[0] = itemID;
            row[1] = itemName;
//            pDP->push_back();

        }
    }

    //Header - donos , 1
    ++it;


    {
        dp::IDataSet* pDP = pDB->createSmallDataSet("SELECT SUM(Mult * Quantity) as Quantity, SUM(CAST(Mult * Quantity * Price as integer)) as Value from ItemsCardDemo "
            " WHERE [Datum] < ? and ItemNT = ? and ItemID = ? ");

        it << rpt::ContainerType::CNT_Header << pDP; //define field and its data

        dp::DSColumns cols(pDP->allocBindColumns(2));
        cols << "Quantity" << td::decimal3
            << "Value" << td::decimal2;

        dp::Params pars = pDP->allocParams();
        pars << fromDate << namingType << itemID;



        if (!pDP->execute())
        {
            return;
        }

        size_t nRows = pDP->getNumberOfRows();
        if (nRows == 0)
        {
            return;
        }
    }

    //Body , 2
    ++it;

    {

        dp::IDataSet* pDP(pDB->createDataSet("SELECT Datum, CAST('M1R174082/15' as text) as DocID, Mult, Quantity, Price from ItemsCardDemo "
            " WHERE ItemNT = ? and ItemID = ? "
            " ORDER BY [Datum]"));


        it << rpt::ContainerType::CNT_Body << pDP; //define field and its data


        dp::DSColumns cols(pDP->allocBindColumns(7));
        cols << "Datum" << td::date
            << "DocID" << td::string8
            << "Mult" << td::int2
            << "Quantity" << td::decimal3
            << "Price" << td::decimal4
            << "fSaldo" << dp::DSColumn::Type::CalcOnLoad << td::decimal3
            << "fVSaldo" << dp::DSColumn::Type::CalcOnLoad<< td::decimal2;


        dp::Params pars = pDP->allocParams();
        pars << namingType << itemID;


        if (!pDP->execute())
        {
            auto strErr = pDP->getErrorStr();
            int k = 0;
            return;
        }
        size_t nRows = pDP->getNumberOfRows();
        if (nRows == 0)
        {
            return;
        }
    }

    //repConfigName
    td::String configName(repConfigName);

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr(repConfigName), pImage);
    }

}


void landscapeTest(const gui::Image* pImage, const char* reportName)
{
    td::Date fromDate(2015, 1, 5);
    td::Date toDate;
    toDate.now();
    td::INT2 namingType = 1;
    td::INT4 itemID = 387001;
    td::String itemName("Item 387001");

    dp::IDatabase* pDB = dp::getMainDatabase();


    DocumentData docData;
    CompanyData companyData;
    ObjectData objectData;

    docData[RPT_TXT_DOCUMENT_TYPE] = "LandscapeLayout";
    docData[RPT_TXT_DOCUMENT_ID] = "";
    docData[RPT_DT_DOCUMENT_FROM_DATETIME] = td::DateTime(fromDate);
    docData[RPT_DT_DOCUMENT_TO_DATETIME] = td::DateTime(toDate);

    rpt::ReportData repData(2, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

    rpt::ReportData::iterator it(repData.begin());

    //it.setLayoutNo(0);

    {
        dp::IDataSet* pDP(pDB->createDataSet("select SUM(CAST(cast(Mult>0 as integer) * Quantity*Price as integer)) as InVal, "
            " SUM(CAST(cast(Mult < 0 as integer) * Quantity * Price as integer)) as OutVal "
            " from ItemsCardDemo where Datum < ?"));

        it << rpt::ContainerType::CNT_Header << pDP;


        dp::DSColumns cols(pDP->allocBindColumns(2));
        cols << "InVal" << td::decimal2
            << "OutVal" << td::decimal2;


        dp::Params pars = pDP->allocParams();
        pars << fromDate;


        if (!pDP->execute())
        {
            return;
        }
        size_t nRows = pDP->getNumberOfRows();
        if (nRows == 0)
        {
            return;
        }


    }

    ++it;

    {
        dp::IDataSet* pDP(pDB->createDataSet("select N.Name, I.Datum as Date, I.Mult, I.Quantity, I.Price"
            " from Naming N, ItemsCardDemo I"
            " where N.NT = I.ItemNT and N.ID = I.ItemID and I.Datum >= ? and I.Datum <= ?"
            " ORDER BY I.Datum"));


        it << rpt::ContainerType::CNT_Body << pDP;


        dp::DSColumns cols(pDP->allocBindColumns(5));
        cols << "Name" << td::string8
            << "Date" << td::date
            << "Mult" << td::int4
            << "Quantity" << td::decimal3
            << "Price" << td::decimal4;

        dp::Params pars = pDP->allocParams();
        pars << fromDate << toDate;


        if (!pDP->execute())
        {
            return;
        }
        size_t nRows = pDP->getNumberOfRows();
        if (nRows == 0)
        {
            return;
        }
    }

    //reportName
    td::String configName(reportName);

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr(reportName), pImage);
    }
}


void port2LandscapeRotation(const gui::Image* pImage)
{
    landscapeTest(pImage, "Port2LandscapeRotation2");
}

void landScapeWithFormulaTest(const gui::Image* pImage)
{
    landscapeTest(pImage, "LandscapeWithFormulaTest2");
}

void karticaReport4(const gui::Image* pImage)
{
    showSingleCard(pImage, "SingleCard_v4");
}


void karticaMultiReport(const gui::Image* pImage)
{
    td::Date fromDate(2015, 1, 5);
    td::Date toDate;
    toDate.now();
    td::INT2 namingType = 1;
    td::INT4 fromItemID = 0;
    td::INT4 toItemID = 1000000000;

    dp::IDatabase* pDB = dp::getMainDatabase();

    DocumentData docData;
    CompanyData companyData;
    ObjectData objectData;

    docData[RPT_TXT_DOCUMENT_TYPE] = "Kartice roba";
    docData[RPT_TXT_DOCUMENT_ID] = "";
    docData[RPT_DT_DOCUMENT_FROM_DATETIME] = td::DateTime(fromDate);
    docData[RPT_DT_DOCUMENT_TO_DATETIME] = td::DateTime(toDate);

    rpt::ReportData repData(3, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());
    rpt::ReportData::iterator it(repData.begin());


    //it.setLayoutNo(0);


        //CardFilter DataGroup 0
    {
        //Object DataGroup
        {
            dp::IDataSet* pDP(dp::createConnectionlessDataSet(dp::IDataSet::Size::SingleRow));

            it << rpt::ContainerType::CNT_DocHeader << pDP; //define field and its data

            dp::DSColumns cols(pDP->allocBindColumns(2));
            cols << "IDFrom" << td::int4
                << "IDTo" << td::int4;

            pDP->execute();
            auto& row = pDP->getRow(0);
            row[0] = fromItemID;
            row[1] = toItemID;
//            pDP->push_back();

        }
    }


    //Header - donos, ovo je leading DP., Svaki row u ovom DP ce poceti na novoj strani -> ukupan broj strana u izvjestaju je >= pDP->getNumberOfRows()
    ++it;


    {
        dp::IDataSet* pDP = pDB->createSmallDataSet("select I.NT, I.ID, I.Name, IfNULL(D.Quantity, 0) as Quantity, IfNULL(D.Value, 0) as Value from vwItems as I "
            " LEFT JOIN "
            " (SELECT ItemNT, ItemID, SUM(Mult * Quantity) as Quantity, SUM(CAST(Mult * Quantity * Price as integer)) as Value from ItemsCardDemo "
                " WHERE ItemNT = ? and ItemID > ? and ItemID <= ? "
            "    GROUP BY ItemNT, ItemID) as D "
            " ON I.NT = D.ItemNT and I.ID = D.ItemID "
            " WHERE I.NT = ? and I.ID >= ? and I.ID <= ? ");

        it << rpt::ContainerType::CNT_Header << pDP; //define field and its data


        dp::DSColumns cols(pDP->allocBindColumns(4));
        cols << "ID" << td::int4
            << "Name" << td::string8
            << "Quantity" << td::decimal3
            << "Value" << td::decimal2;

        dp::Params pars = pDP->allocParams();
        pars << namingType << fromItemID << toItemID;
        pars << namingType << fromItemID << toItemID;

    
        if (!pDP->execute())
        {
            return;
        }

        size_t nRows = pDP->getNumberOfRows();
        if (nRows == 0)
        {
            return;
        }
    }


    //Body
    ++it;


    {
        dp::IDataSet* pDP = pDB->createSmallDataSet("SELECT ItemID, [Datum] as Date, CAST('M1R174082/15' as varchar(32)) as DocID, Mult, Quantity, Price  "
            " from ItemsCardDemo "
            " WHERE[Datum] >= ? and [Datum] <= ? and ItemNT = ? "
            " and ItemID >= ? and ItemID <= ? "
            " ORDER BY ItemID, [Datum]");

        it << rpt::ContainerType::CNT_Body << pDP; //define field and its data


        dp::DSColumns cols(pDP->allocBindColumns(8));
        cols << "ItemID" << td::int4
            << "Date" << td::date
            << "DocID" << td::string8
            << "Mult" << td::int2
            << "Quantity" << td::decimal3
            << "Price" << td::decimal4
            << "fSaldo" << dp::DSColumn::Type::CalcOnLoad << td::decimal3
            << "fVSaldo" << dp::DSColumn::Type::CalcOnLoad<< td::decimal2;


        dp::Params pars = pDP->allocParams();
        pars << fromDate << toDate << namingType << fromItemID << toItemID;


        if (!pDP->execute())
        {
            return;
        }

        size_t nRows = pDP->getNumberOfRows();
        int x;
        if (nRows == 0)
        {
            return;
        }
    }

    //MultiCard_v1
    td::String configName("MultiCard_v1");

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr("MultiCard_v1"), pImage);
    }
}


//Demonstrates excludes zeros and qtu::Dialog overloading
void karticaMultiReport2(const gui::Image* pImage)
{
    td::Date fromDate(2015, 1, 5);
    td::Date toDate;
    toDate.now();
    td::INT2 namingType = 1;
    td::INT4 fromItemID = 0;
    td::INT4 toItemID = 1000000000;

    dp::IDatabase* pDB = dp::getMainDatabase();


    DocumentData docData;
    CompanyData companyData;
    ObjectData objectData;

    docData[RPT_TXT_DOCUMENT_TYPE] = "Kartice roba";
    docData[RPT_TXT_DOCUMENT_ID] = "";
    docData[RPT_DT_DOCUMENT_FROM_DATETIME] = td::DateTime(fromDate);
    docData[RPT_DT_DOCUMENT_TO_DATETIME] = td::DateTime(toDate);

    rpt::ReportData repData(3, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());
    rpt::ReportData::iterator it(repData.begin());

    //it.setLayoutNo(0);


        //CardFilter DataGroup 0
    {
        //Object DataGroup
        {
            dp::IDataSet* pDP = dp::createConnectionlessDataSet(dp::IDataSet::Size::SingleRow);
            it << rpt::ContainerType::CNT_DocHeader << pDP; //define field and its data
            dp::DSColumns cols(pDP->allocBindColumns(2));
            cols << "IDFrom" << td::int4
                << "IDTo" << td::int4;

            pDP->execute();
            auto& row = pDP->getRow(0);
            row[0] = fromItemID;
            row[1] = toItemID;
//            pDP->push_back();

        }
    }

    //Header - donos, ovo je leading DP., Svaki row u ovom DP ce poceti na novoj strani -> ukupan broj strana u izvjestaju je >= pDP->getNumberOfRows()
    ++it;

    {
        dp::IDataSet* pDP = pDB->createSmallDataSet("select I.NT, I.ID, I.Name, IfNULL(D.Quantity, 0) as Quantity, IfNULL(D.Value, 0) as Value from vwItems as I "
            " LEFT JOIN "
            " (SELECT ItemNT, ItemID, SUM(Mult * Quantity) as Quantity, SUM(CAST(Mult * Quantity * Price as integer)) as Value from ItemsCardDemo "
            " WHERE ItemNT = ? and ItemID > ? and ItemID <= ? "
            "    GROUP BY ItemNT, ItemID) as D "
            " ON I.NT = D.ItemNT and I.ID = D.ItemID "
            " WHERE I.NT = ? and I.ID >= ? and I.ID <= ? and (Value <> 0) ");

        it << rpt::ContainerType::CNT_Header << pDP; //define field and its data


        dp::DSColumns cols(pDP->allocBindColumns(4));
        cols << "ID" << td::int4
            << "Name" << td::string8
            << "Quantity" << td::decimal3
            << "Value" << td::decimal2;

        dp::Params pars = pDP->allocParams();
        pars << namingType << fromItemID << toItemID;
        pars << namingType << fromItemID << toItemID;


        if (!pDP->execute())
        {
            return;
        }

        size_t nRows = pDP->getNumberOfRows();
        if (nRows == 0)
        {
            return;
        }
    }


    //Body
    ++it;
    {
        dp::IDataSet* pDP = pDB->createSmallDataSet("SELECT ItemID, [Datum] as Date, CAST('M1R174082/15' as varchar(32)) as DocID, Mult, Quantity, Price  "
            " from ItemsCardDemo "
            " WHERE[Datum] >= ? and [Datum] <= ? and ItemNT = ? "
            " and ItemID >= ? and ItemID <= ? "
            " ORDER BY ItemID, [Datum]");

        it << rpt::ContainerType::CNT_Body << pDP; //define field and its data


        dp::DSColumns cols(pDP->allocBindColumns(8));
        cols << "ItemID" << td::int4
            << "Date" << td::date
            << "DocID" << td::string8
            << "Mult" << td::int2
            << "Quantity" << td::decimal3
            << "Price" << td::decimal4
            << "fSaldo" << dp::DSColumn::Type::CalcOnLoad << td::decimal3
            << "fVSaldo" << dp::DSColumn::Type::CalcOnLoad<< td::decimal2;


        dp::Params pars = pDP->allocParams();
        pars << fromDate << toDate << namingType << fromItemID << toItemID;


        if (!pDP->execute())
        {
            return;
        }

        size_t nRows = pDP->getNumberOfRows();
        if (nRows == 0)
        {
            return;
        }
    }
    //MultiCard_v1
    td::String configName("MultiCard_v1");

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr("MultiCard_v1"), pImage);
    }

}

//demonstracija multikartice sa fall report tipom
void karticaMultiReport_WF(const gui::Image* pImage)
{
    td::Date fromDate(2015, 1, 5);
    td::Date toDate;
    toDate.now();
    td::INT2 namingType = 1;
    td::INT4 fromItemID = 0;
    td::INT4 toItemID = 1000000000;

    dp::IDatabase* pDB = dp::getMainDatabase();



    DocumentData docData;
    CompanyData companyData;
    ObjectData objectData;

    docData[RPT_TXT_DOCUMENT_TYPE] = "Kartice roba";
    docData[RPT_TXT_DOCUMENT_ID] = "";
    docData[RPT_DT_DOCUMENT_FROM_DATETIME] = td::DateTime(fromDate);
    docData[RPT_DT_DOCUMENT_TO_DATETIME] = td::DateTime(toDate);

    rpt::ReportData repData(3, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());
    rpt::ReportData::iterator it(repData.begin());


    //it.setLayoutNo(0);


        //CardFilter DataGroup 0
    {
        //Object DataGroup
        {
            dp::IDataSet* pDP(dp::createConnectionlessDataSet(dp::IDataSet::Size::SingleRow));

            it << rpt::ContainerType::CNT_DocHeader << pDP; //define field and its data

            dp::DSColumns cols(pDP->allocBindColumns(2));
            cols << "IDFrom" << td::int4
                << "IDTo" << td::int4;


            pDP->execute();
            
            auto& row = pDP->getRow(0);
            row[0] = fromItemID;
            row[1] = toItemID;
            
//            pDP->push_back();
        }
    }


    //Header - donos, ovo je leading DP., Svaki row u ovom DP ce poceti na novoj strani -> ukupan broj strana u izvjestaju je >= pDP->getNumberOfRows()
    ++it;


    {
        dp::IDataSet* pDP = pDB->createSmallDataSet("select I.NT, I.ID, I.Name, IfNULL(D.Quantity, 0) as Quantity, IfNULL(D.Value, 0) as Value from vwItems as I "
            " LEFT JOIN "
            " (SELECT ItemNT, ItemID, SUM(Mult * Quantity) as Quantity, SUM(CAST(Mult * Quantity * Price as integer)) as Value from ItemsCardDemo "
            " WHERE ItemNT = ? and ItemID > ? and ItemID <= ? "
            "    GROUP BY ItemNT, ItemID) as D "
            " ON I.NT = D.ItemNT and I.ID = D.ItemID "
            " WHERE I.NT = ? and I.ID >= ? and I.ID <= ? ");

        it << rpt::ContainerType::CNT_Grouper << pDP; //define field and its data


        dp::DSColumns cols(pDP->allocBindColumns(4));
        cols << "ID" << td::int4
            << "Name" << td::string8
            << "Quantity" << td::decimal3
            << "Value" << td::decimal2;

        dp::Params pars = pDP->allocParams();
        pars << namingType << fromItemID << toItemID;
        pars << namingType << fromItemID << toItemID;


        if (!pDP->execute())
        {
            return;
        }

        size_t nRows = pDP->getNumberOfRows();
        if (nRows == 0)
        {
            return;
        }
    }


    //Body
    ++it;


    {
        dp::IDataSet* pDP = pDB->createSmallDataSet("SELECT ItemID, [Datum] as Date, CAST('M1R174082/15' as varchar(32)) as DocID, Mult, Quantity, Price  "
            " from ItemsCardDemo "
            " WHERE[Datum] >= ? and [Datum] <= ? and ItemNT = ? "
            " and ItemID >= ? and ItemID <= ? "
            " ORDER BY ItemID, [Datum]");

        it << rpt::ContainerType::CNT_Body << pDP; //define field and its data


        dp::DSColumns cols(pDP->allocBindColumns(8));
        cols << "ItemID" << td::int4
            << "Date" << td::date
            << "DocID" << td::string8
            << "Mult" << td::int2
            << "Quantity" << td::decimal3
            << "Price" << td::decimal4
            << "fSaldo" << dp::DSColumn::Type::CalcOnLoad << td::decimal3
            << "fVSaldo" << dp::DSColumn::Type::CalcOnLoad<< td::decimal2;


        dp::Params pars = pDP->allocParams();
        pars << fromDate << toDate << namingType << fromItemID << toItemID;


        if (!pDP->execute())
        {
            return;
        }

        size_t nRows = pDP->getNumberOfRows();
        if (nRows == 0)
        {
            return;
        }
    }
    //MultiCard_WF
    td::String configName("MultiCard_WF");

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr("MultiCard_WF"), pImage);
    }

}


void showWaterfall(const gui::Image* pImage, const char* reportName)
{
    dp::IDatabase* pDB = dp::getMainDatabase();

    DocumentData docData;
    CompanyData companyData;
    ObjectData objectData;

    docData[RPT_TXT_DOCUMENT_TYPE] = "Pregled prodaje po tarifi i asortimanu";

    rpt::ReportData repData(4, docData, companyData, objectData, pDB, mu::getAppSettings()->getTranslationExtension().c_str());

    rpt::ReportData::iterator it(repData.begin());
    //it.setLayoutNo(0);


    td::INT4 fromID = 1;
    td::INT4 toID = 99999;


    populateFromToCardData(pDB, it, fromID, toID);
    ++it;


    //Grouper: Tax
    {
        dp::IDataSet* pDP = pDB->createDataSet("SELECT ID, Name, Value FROM Tax");

        it << rpt::ContainerType::CNT_Grouper << pDP; //define field and its data

        dp::DSColumns cols(pDP->allocBindColumns(3));
        cols << "ID" << td::int4
            << "Name" << td::string8
            << "Value" << td::decimal1;

        if (!pDP->execute())
        {
            return;
        }

        size_t nRows = pDP->getNumberOfRows();
        if (nRows == 0)
        {
            return;
        }
    }

    //Sub-Grouper Assortment

    ++it;

    {
        dp::IDataSet* pDP = pDB->createDataSet("SELECT ID, Name FROM Assortment");

        it << rpt::ContainerType::CNT_Grouper << rpt::ContainerType::CNT_Grouper << pDP; //define field and its data

        dp::DSColumns cols(pDP->allocBindColumns(2));
        cols << "ID" << td::int4
            << "Name" << td::string8;


        if (!pDP->execute())
        {
            return;
        }

        size_t nRows = pDP->getNumberOfRows();
        if (nRows == 0)
        {
            return;
        }
    }

    //Body

    ++it;

    {


        dp::IDataSet* pDP = pDB->createDataSet("select I.TaxID, A.ID as AssortID, I.NT, I.ID, N.Name, SUM(I.Quantity) as Quant,  "
            " SUM(CAST((I.Quantity * I.Price) as integer)) as Value "
            " from Naming N, Items I, Assortment A "
            " where N.NT = I.NT and N.ID = I.ID and N.NT = ? and I.IDAssort = A.ID and N.ID >= ? and N.ID <= ?  "
            " Group BY I.TaxID, A.ID, I.NT, I.ID, N.Name  "
            " ORDER BY I.TaxID, A.ID, I.NT, I.ID");

        it << rpt::ContainerType::CNT_Body << pDP; //define field and its data

        dp::DSColumns cols(pDP->allocBindColumns(7));
        cols << "TaxID" << td::int2
            << "AssortID" << td::int2
            << "NT" << td::int2
            << "ID" << td::int4
            << "Name" << td::string8
            << "Quant" << td::decimal3
            << "Value" << td::decimal2;

        //set parameters
        td::INT2 nt = 1;
        dp::Params pars = pDP->allocParams();
        pars << nt << fromID << toID;


        if (!pDP->execute())
        {
            return;
        }
        size_t nRows = pDP->getNumberOfRows();
        if (nRows == 0)
        {
            return;
        }
    }
    //reportName
    td::String configName(reportName);

    rpt::IViewer* pRepView = rpt::IViewer::create(configName, repData);
    if (pRepView)
    {
        pRepView->show(gui::tr(reportName), pImage);
    }

}


void showWaterfallSimple(const gui::Image* pImage)
{
    showWaterfall(pImage, "WF_ItemsByTaxAndAssort");
}
void showWaterfallAdvanced(const gui::Image* pImage)
{
    showWaterfall(pImage, "WF_ItemsByTaxAndAssort_Advanced");
}
void showWaterfallAdvanced2(const gui::Image* pImage)
{
    showWaterfall(pImage, "WF_ItemsByTaxAndAssort_Advanced2");
}



