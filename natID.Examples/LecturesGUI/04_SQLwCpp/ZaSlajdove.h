#include <iostream>
#include "Prodaja.h"
#include <cnt/PushBackVector.h>

typedef int TipPodatka1;
typedef int TipPodatka2;
typedef int TipPodatka3;

//za slajdove - ODAVDE
int dajPripremljeniPodatak(size_t i, size_t j)
{
    return 1;
}

bool kreateConnection()
{
    //samo jednom
    dp::IDatabasePtr pDB(dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3));
    if (!pDB->connect("R/mojaSQLiteBaza.db"))
        return false;
    return true;
}

class NekaStruktura
{
public:
    TipPodatka1 valKol1;
    TipPodatka2 valKol2;
    TipPodatka3 valKol3;
};

//pseudo dijagram
bool pseudoInsert(dp::IDatabasePtr& pDB, const cnt::SafeFullVector<NekaStruktura>& podaci)
{
    //kreiranje statement-a
    dp::IStatementPtr pStat(pDB->createStatement("INSERT INTO NazivTabele (KOL1,KOL2,KOL3) VALUES(?,?,?)"));
    //deklarisi lokalne varijable iz kojih ce podaci biti transferirani kao parametri
    TipPodatka1 lokalVar1;
    TipPodatka2 lokalVar2;
    TipPodatka3 lokalVar3;
    //uvezi lokalne varijable sa parametrima
    dp::Params par(pStat->allocParams());
    par << lokalVar1 << lokalVar2 << lokalVar3;
    
    dp::Transaction tr(pDB); //startanje transakcije
    size_t nRecordsToTransfer = podaci.size();
    for (size_t i=0; i<nRecordsToTransfer; ++i)
    {
        //postavi vrijesnosti lokalnih varijabli
        lokalVar1 = podaci[i].valKol1; //daj vrijednost prve varijable(parametra)
        lokalVar2 = podaci[i].valKol2; //daj vrijednost druge varijable(parametra)
        lokalVar3 = podaci[i].valKol3; //daj vrijednost trece varijable(parametra)
        
        if (!pStat->execute()) //execute uzima vrijednosti lokalnih varijabli kao vr. parametara
        {
            tr.rollBack(); //prva stvar koja se radi ako nesto nije ok
            td::String strErr;
            pStat->getErrorStr(strErr); //daj opis greske
            std::cout << "Greska! " << strErr << td::endl; //ispisi gresku
            return false;
        }
    }
    tr.commit();
    return true;
}

bool pseudoSelect(dp::IDatabasePtr& pDB, cnt::PushBackVector<NekaStruktura>& podaci, TipPodatka1 par1)
{
    //kreiranje SELECT statement-a
    dp::IStatementPtr pStat(pDB->createStatement("SELECT KOL1, KOL2 FROM NazivTabele WHERE KOL3=?"));
    //moze i ovako ukoliko se KOL1 i KOL2 nalazi u vracenom setu (varijanta sa nevedenim kolonama je bolja)
    //dp::IStatementPtr pStat(pDB->createStatement("SELECT * FROM NazivTabele WHERE KOL3=?"));
    
    //uvezi lokalne varijable sa parametrima
    TipPodatka1 lokalVar1 = par1; //parametar se postavlja samo jednom
    dp::Params par(pStat->allocParams());
    par << lokalVar1;
    
    //uvezi lokalne varijable sa nazivima kolona
    TipPodatka2 lokalCol1;
    TipPodatka3 lokalCol2;
    dp::Columns cols(pStat->allocBindColumns(2));
    cols << "KOL1" << lokalCol1 << "KOL2" << lokalCol2;
    
    if (!pStat->execute()) //samo jednom
        return false;
    
    while (pStat->moveNext()) //iteriraj kroz vraceni set podataka
    {
        //postavi vrijesnosti lokalnih varijabli u povratni vektor podataka
        NekaStruktura strukturaPodataka;
        strukturaPodataka.valKol1 = lokalCol1;
        strukturaPodataka.valKol2 = lokalCol2;
        podaci.push_back(strukturaPodataka);
    }
    return true;
}
//za slideove - DO OVDJE