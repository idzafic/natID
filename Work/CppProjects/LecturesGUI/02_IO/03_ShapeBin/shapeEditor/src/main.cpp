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
        std::cout << "4. Snimi" << td::endl;
        std::cout << "5. Ucitaj" << td::endl;
        std::cout << "6. Prikazi model na konzoli" << td::endl;
        std::cout << "0. Kraj" << td::endl;
        std::cout << "Unesize vas izbor:";
        std::cin >> odabir;
        if (odabir > 6){std::cout << "Pogresna meni opcija." << td::endl; continue; }
        switch (odabir)
        {
            case 1: model.addRect();  break;
            case 2: model.addRoundedRect(); break;
            case 3: model.addCircle(); break;
            case 4: model.save("/Volumes/RAMDisk/Shapes.bin"); break;
            case 5: model.load("/Volumes/RAMDisk/Shapes.bin"); break;
            case 6: model.show(std::cout); break;
        }
    }
    return 0; 
}
