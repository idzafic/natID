#include <iostream>
#include <td/String.h>
#include <dp/IDatabase.h>
#include <cnt/PushBackVector.h>

class Prodaja
{
    dp::IDatabasePtr _pDP;
    class Customer
    {
    public:
        td::UINT4 sifra;
        td::String naziv;
        td::String adresa;
    };
    
    class Invoice
    {
    public:
        td::UINT4 broj;
        td::Date datum;
        td::UINT4 sifraKlijenta;
        td::Decimal2 iznos;
    };
    
    class InvoiceDetail : public Invoice
    {
    public:
        td::String nazivKlijenta;
    };
    
    bool insertCustomers(const cnt::PushBackVector<Customer>& customers)
    {
        //lokalni data holderi
        td::UINT4 sifra;
        td::Variant naziv(td::string8, td::nch, 40);
        td::Variant adresa(td::string8, td::nch, 80);
        //kreiraj sql naredbu
        dp::IStatementPtr pStat(_pDP->createStatement("INSERT INTO Klijenti (Sifra,Naziv,Adresa) VALUES(?,?,?)"));
        dp::Params par(pStat->allocParams());
        //bind parametre
        par << sifra << naziv << adresa;
        //otvori transakciju
        dp::Transaction tr(_pDP);
        for(auto& c : customers)
        {
            sifra = c.sifra;
            naziv.setValue(c.naziv);
            adresa.setValue(c.adresa);
            if (!pStat->execute())
            {
                tr.rollBack();
                std::cout << "ERROR! Ne mogu unijeti zeljenog korisnika sa sifrom:" << sifra << td::endl;
                return false;
            }
        }
        
        if (tr.commit())
            return true;
        
        return false;
    }
    
    td::UINT4 getLastInvoice()
    {
        dp::IStatementPtr pStat(_pDP->createStatement("SELECT IFNULL(MAX(Broj), 0) as LastInv FROM Racuni"));
        dp::Columns cols(pStat->allocBindColumns(1));
        td::UINT4 lastNo = 1;
        cols << "LastInv" << lastNo;
        if (!pStat->execute())
            return 0;
        if(pStat->moveNext())
            return lastNo;
        return 0;
    }
    
    bool insertInvoices(const cnt::PushBackVector<Invoice>& invoices)
    {
        //lokalni data holderi
        td::UINT4 broj;
        td::Date datum;
        td::UINT4 sifraKlijenta;
        td::Decimal2 iznos;
        //kreiraj sql naredbu
        dp::IStatementPtr pStat(_pDP->createStatement("INSERT INTO Racuni(Broj,Datum,SifraKlijenta,Iznos) VALUES(?,?,?,?)"));
        dp::Params par(pStat->allocParams());
        //bind parametre
        par << broj << datum << sifraKlijenta << iznos;
        //otvori transakciju
        broj = getLastInvoice();
        
        dp::Transaction tr(_pDP);
        for(auto& inv : invoices)
        {
            ++broj;
            datum = inv.datum;
            sifraKlijenta = inv.sifraKlijenta;
            iznos = inv.iznos;
            if (!pStat->execute())
            {
                tr.rollBack();
                std::cout << "ERROR! Ne mogu unijeti broj racuna:" << broj << td::endl;
                return false;
            }
        }
        
        if (tr.commit())
            return true;
        
        return false;
    }
    
    bool isConnectionOK() const
    {
        if (!_pDP->isConnected())
        {
            std::cout << "Error! Niste spojeni sa bazom!" << td::endl;
            return false;
        }
        return true;
    }
    
    void enterCustomer(Customer& c) const
    {
        std::cout << "Unesite sifru korisnika:";
        std::cin >> c.sifra;
        std::cout << "Unesite naziv korisnika (bez razmaka):";
        std::cin >> c.naziv;
        std::cout << "Unesite adresu korisnika (bez razmaka):";
        std::cin >> c.adresa;
    }
    
    void enterInvoice(Invoice& inv) const
    {
        std::cout << "Unesite datum (yyyy-mm-dd):";
        td::String strDatum;
        std::cin >> strDatum;
        inv.datum.fromString(strDatum.c_str());
        std::cout << "Unesite sifru korisnika:";
        std::cin >> inv.sifraKlijenta;
        std::cout << "Unesite iznos racuna:";
        std::cin >> inv.iznos;
    }
    
    bool populateInvDataSet(dp::IDataSetPtr& pDS, bool groupByCustomer, InvoiceDetail& invDet)
    {
        td::Date odDatuma;
        td::Date doDatuma;
        odDatuma.setDate(2022, 1, 1); //od pocetka godine
        doDatuma.setDate(2022, 12, 31); //do kraja godine
        
        std::cout << "Odabrati izbor:\n1.Tacune u ovoj (" << odDatuma.getYear()
                    << ".) godini.\nX. Odabrati od-do datuma\nIzbor:";
        int izbor;
        std::cin >> izbor;
        if (izbor != 1)
        {
            //ovdje se moze odabrati neki drugi datum
            std::cout << "Od datuma (yyyy-mm-dd):";
            std::cin >> odDatuma;
            std::cout << "Do datuma (yyyy-mm-dd):";
            std::cin >> doDatuma;
        }
        
        if (groupByCustomer)
            pDS = _pDP->createDataSet("SELECT R.Broj, R.Datum, R.SifraKlijenta as SK, K.Naziv, R.Iznos "
                                      "FROM Racuni R, Klijenti K "
                                      "WHERE R.SifraKlijenta=K.Sifra and R.Datum>=? and R.Datum<=? "
                                      "ORDER BY R.SifraKlijenta, R.Datum, R.Broj");
        else
            pDS = _pDP->createDataSet("SELECT R.Broj, R.Datum, R.SifraKlijenta as SK, K.Naziv, R.Iznos "
                                      "FROM Racuni R, Klijenti K "
                                      "WHERE R.SifraKlijenta=K.Sifra and R.Datum>=? and R.Datum<=? "
                                      "ORDER BY R.Broj");
        
        dp::DSColumns cols(pDS->allocBindColumns(5));
        cols << "Broj" <<invDet.broj << "Datum" << invDet.datum << "SK" << invDet.sifraKlijenta <<
                "Naziv" << invDet.nazivKlijenta << "Iznos" << invDet.iznos;
        dp::Params params(pDS->allocParams());
        params << odDatuma << doDatuma;
        if (pDS->execute())
            return true;
        return true;
    }
    
public:
    Prodaja()
    {
        //kreirati interface za sqlite
        _pDP = dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3);
    }
    
    bool connect(const td::String& dbConnectionString)
    {
        if (!_pDP.getPtr())
            return false;
        
        if (_pDP->isConnected())
            return true;
        
        return _pDP->connect(dbConnectionString);
    }
    
    bool addCustomers()
    {
        if (!isConnectionOK())
            return false;
        
        size_t nCustomers;
        std::cout << "Koliko kupaca zelite unijeti:";
        std::cin >> nCustomers;
        if (nCustomers == 0)
            return false;
    
        cnt::PushBackVector<Customer> customers(nCustomers);
        for (size_t i=0; i<nCustomers; ++i)
        {
            Customer c;
            enterCustomer(c);
            customers.push_back(c);
        }
        
        return insertCustomers(customers);
    }
    
    bool addInvoices()
    {
        if (!isConnectionOK())
            return false;
        
        size_t nInvoices;
        std::cout << "Koliko racuna zelite unijeti:";
        std::cin >> nInvoices;
        if (nInvoices == 0)
            return false;
    
        cnt::PushBackVector<Invoice> invoices(nInvoices);
        for (size_t i=0; i<nInvoices; ++i)
        {
            Invoice inv;
            enterInvoice(inv);
            invoices.push_back(inv);
        }
        
        return insertInvoices(invoices);
    }
    
    void deleteInvoice()
    {
        if (!isConnectionOK())
            return;
        
        td::UINT4 brRac;
        std::cout << "Unesite broj racuna za brisanje:";
        std::cin >> brRac;
        dp::Transaction tr(_pDP);
        dp::IStatementPtr pStat(_pDP->createStatement("DELETE FROM Racuni WHERE Broj=?"));
        dp::Params par(pStat->allocParams());
        par << brRac;
        if (!pStat->execute())
        {
            tr.rollBack();
            std::cout << "ERROR! Ne mogu izbrisati racun broj:" << brRac << td::endl;
            return;
        }
        tr.commit();
        std::cout << "Uspjesno izbrisan racun broj:" << brRac << td::endl;
    }
    
    void updateCustomer()
    {
        if (!isConnectionOK())
            return;
        
        Customer c;
        enterCustomer(c);
        
        td::UINT4 sifra;
        td::Variant naziv(td::string8, td::nch, 40); //zbog servera (unicode)
        td::Variant adresa(td::string8, td::nch, 80); //zbog servera (unicode)
        //kreiraj sql naredbu
        dp::IStatementPtr pStat(_pDP->createStatement("UPDATE Klijenti SET Naziv=?, Adresa=? WHERE Sifra=?"));
        dp::Params par(pStat->allocParams());
        //bind parametre
        par << naziv << adresa << sifra;
        sifra = c.sifra;
        naziv.setValue(c.naziv);
        adresa.setValue(c.adresa);
        //otvori transakciju
        dp::Transaction tr(_pDP);
        if (!pStat->execute())
        {
            tr.rollBack();
            std::cout << "ERROR! Ne mogu editovati korisnika sa sifrom:" << sifra << td::endl;
            return;
        }
        
        tr.commit();
        std::cout << "Korisnik sa sifrom: " << sifra << " uspjesno editovan!" << td::endl;
    }
    
    void showCustomers(bool poAbecedi)
    {
        if (!isConnectionOK())
            return;
        const char* strSQLStatement = "SELECT Sifra,Naziv,Adresa FROM Klijenti ORDER BY Sifra";
        if (poAbecedi)
            strSQLStatement = "SELECT Sifra,Naziv,Adresa FROM Klijenti ORDER BY Naziv ASC";
        dp::IStatementPtr pStat(_pDP->createStatement(strSQLStatement));
        dp::Columns cols(pStat->allocBindColumns(3));
        td::UINT4 sifra;
        td::String naziv;
        td::String adresa;
        cols << "Sifra" << sifra << "Naziv" << naziv << "Adresa" << adresa;
        if (!pStat->execute())
        {
            std::cout << "ERROR! Ne mogu selektovati podatke iz baze" << td::endl;
            td::String strErr;
            pStat->getErrorStr(strErr);
            std::cout << strErr << td::endl;
            return;
        }
        std::cout << "------------------------------------------------------" << td::endl;
        //prikazati sva selektovana polja
        while (pStat->moveNext()) //svaki uspjesan moveNext upisuje podatke u sifra, naziv, adresa
            std::cout << "Sifra=" << sifra << ", naziv=" << naziv << ", adresa=" << adresa << td::endl;
        
        std::cout << "------------------------------------------------------" << td::endl;
    }
    
    void showInvoices(bool groupByCustomer)
    {
        if (!isConnectionOK())
            return;
        dp::IDataSetPtr pDS;
        InvoiceDetail invDet;
        if (!populateInvDataSet(pDS, groupByCustomer, invDet))
        {
            std::cout << "ERROR! Cannot populate Invoice Data Set!" << td::endl;
            return;
        }
        size_t nRows = pDS->getNumberOfRows();
        if (nRows == 0)
        {
            std::cout << "Nema niti jednog racuna u odabranom periodu!" << td::endl;
            return;
        }
        
        pDS->moveToPos(0);
        std::cout << "----------------------------------------------------------" << td::endl;
        while (pDS->moveNext())
        {
            if (groupByCustomer)
            {
                std::cout << "KlijentID=" << invDet.sifraKlijenta << ", naziv=" << invDet.nazivKlijenta
                    << ", datum=" << invDet.datum << ", brRac=" << invDet.broj << ", iznos=" << invDet.iznos << td::endl;
            }
            else
            {
                std::cout << "BrRac=" << invDet.broj  << ", datum=" << invDet.datum << ", klijentID=" << invDet.sifraKlijenta
                    << ", naziv=" << invDet.nazivKlijenta << ", iznos=" << invDet.iznos << td::endl;
            }
        }
        std::cout << "----------------------------------------------------------" << td::endl;
    }
    
    void exportToXML(const td::String& xmlName)
    {
        if (!isConnectionOK())
            return;
        dp::IDataSetPtr pDS;
        InvoiceDetail invDet;
        if (!populateInvDataSet(pDS, false, invDet))
        {
            std::cout << "ERROR! Cannot populate Invoice Data Set!" << td::endl;
            return;
        }
        
        size_t nRows = pDS->getNumberOfRows();
        if (nRows == 0)
        {
            std::cout << "Nema niti jednog racuna u odabranom periodu!" << td::endl;
            return;
        }
        
        pDS->exportToXML(xmlName, "Racuni");
    }
    
    void deleteAll()
    {
        if (!isConnectionOK())
            return;
        dp::IStatementPtr delInv(_pDP->createStatement("DELETE FROM Racuni"));
        dp::IStatementPtr delCli(_pDP->createStatement("DELETE FROM Klijenti"));
        dp::Transaction tr(_pDP);
        if (!delInv->execute())
        {
            tr.rollBack();
            std::cout << "ERROR! Ne mogu izbrisati racune!" << td::endl;
            return;
        }
        
        if (!delCli->execute())
        {
            tr.rollBack();
            std::cout << "ERROR! Ne mogu izbrisati klijente!" << td::endl;
            return;
        }
        tr.commit();
        std::cout << "Svi podaci u bazi su izbrisani" << td::endl;
    }
};
