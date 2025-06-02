#include <iostream>
#include "Prodaja.h"

int main()
{
    fo::fs::path homePath;
    mu::getHomePath(homePath);
    fo::fs::path testDBPath(homePath / "other_bin/TestData/SQLite/Prodaja.db");
    td::String strConnection(testDBPath.string());
    
    Prodaja prodaja;
    
    size_t odabir = 10;
    while (odabir > 0)
    {
        std::cout << " 1. Spoji se na bazu (Connect)" << td::endl;
        std::cout << " 2. Dodaj kupce (INSERT)" << td::endl;
        std::cout << " 3. Promjeni naziv kupca (UPDATE)" << td::endl;
        std::cout << " 4. Dodaj racune (INSERT)" << td::endl;
        std::cout << " 5. Izbrisi racun (DELETE)" << td::endl;
        std::cout << " 6. Prikazi kupce po abecedi (SELECT ORDER abc)" << td::endl;
        std::cout << " 7. Prikazi kupce po sifri (SELECT)" << td::endl;
        std::cout << " 8. Prikazi racune grupisano po kupcima (Select->DataSet)" << td::endl;
        std::cout << " 9. Prikazi racune po broju racuna (Select->DataSet)" << td::endl;
        std::cout << "10. Eksportuj racune u xml (Select->DataSet->XML)" << td::endl;
        std::cout << "11. Izbrisi sve podatke iz baze (DELETE)" << td::endl;
        std::cout << " 0. Kraj" << td::endl;
        std::cout << "Unesize vas izbor:";
        std::cin >> odabir;
        if (odabir > 11)
        {
            std::cout << "Pogresna meni opcija." << td::endl;
            continue;
        }
        switch (odabir)
        {
            case 1:
                if (prodaja.connect(strConnection))
                    std::cout << "Connection OK!" << td::endl;
                else
                    std::cout << "Error! Cannot connect to: " << strConnection << td::endl;
                break;
            case 2:
                if (prodaja.addCustomers())
                    std::cout << "Kupci uspjesno upisani u bazu:" << td::endl;
                else
                    std::cout << "Greska! Kupci NISU upisani u bazu:" << td::endl;
                break;
            case 3: prodaja.updateCustomer(); break;
            case 4:
                if (prodaja.addInvoices())
                    std::cout << "Racuni uspjesno upisani u bazu:" << td::endl;
                else
                    std::cout << "Greska! Racuni NISU upisani u bazu:" << td::endl;
                break;
            case 5: prodaja.deleteInvoice(); break;
            case 6: prodaja.showCustomers(true); break;
            case 7: prodaja.showCustomers(false); break;
            case 8: prodaja.showInvoices(true); break;
            case 9: prodaja.showInvoices(false); break;
            case 10: prodaja.exportToXML("/Volumes/RAMDisk/Prodaja.xml"); break;
            case 11: prodaja.deleteAll(); break;
        }
    }
    
    return 0; 
}
