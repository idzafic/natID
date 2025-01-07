#include <iostream>
#include <td/String.h>
#include <iostream>
#include "Model.h"

int main()
{
    Model model;
    
    size_t odabir = 10;
    while (odabir > 0)
    {
        std::cout << "1. Kreiraj Rect" << td::endl;
        std::cout << "2. Kreiraj RoundedRect" << td::endl;
        std::cout << "3. Kreiraj Circ" << td::endl;
        std::cout << "4. Snimi (bin)" << td::endl;
        std::cout << "5. Ucitaj (bin)" << td::endl;
        std::cout << "6. Snimi (xml)" << td::endl;
        std::cout << "7. Ucitaj (xml)" << td::endl;
        std::cout << "8. Prikazi model na konzoli" << td::endl;
        std::cout << "0. Kraj" << td::endl;
        std::cout << "Unesize vas izbor:";
        std::cin >> odabir;
        if (odabir > 8)
        {
            std::cout << "Pogresna meni opcija." << td::endl;
            continue;
        }
        switch (odabir)
        {
            case 1: model.addRect();  break;
            case 2: model.addRoundedRect(); break;
            case 3: model.addCircle(); break;
#ifdef MU_MACOS
            case 4: model.save("/Volumes/RAMDisk/Shapes.bin"); break;
            case 5: model.load("/Volumes/RAMDisk/Shapes.bin"); break;
            case 6: model.saveXml("/Volumes/RAMDisk/Shapes.xml"); break;
            case 7: model.loadXml("/Volumes/RAMDisk/Shapes.xml"); break;
#else
            case 4: model.save("R:/Shapes.bin"); break;
            case 5: model.load("R:/Shapes.bin"); break;
            case 6: model.saveXml("R:/Shapes.xml"); break;
            case 7: model.loadXml("R:/Shapes.xml"); break;
#endif
            case 8: model.show(std::cout); break;
        }
    }
    
    return 0; 
}
