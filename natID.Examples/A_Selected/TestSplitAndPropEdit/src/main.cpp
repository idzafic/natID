//#include "main.hpp"
#include "Application.h"
#include <td/StringConverter.h>
#include <gui/WinMain.h>
//#include <cnt/Vector.h>
// #include <iostream>
#include "property/DefaultSettings.h"
#include <gui/WinMain.h>

//void testVector()
//{
//    cnt::Vector<int, false> v(20, 10);
//    v.setAll(0);
//    std::cout << v << std::endl;
//    size_t size = v.size();
//    int i= 10;
//    for (auto& iVal : v)
//        iVal = i++;
//    std::cout << v << std::endl;
//}

//global parameters
DefaultSettings g_defaultSettings;

int main(int argc, const char * argv[])
{
    //testVector();
    //return 1;
    
    Application app(argc, argv);
    auto appProperties = app.getProperties();
    td::String trLang = appProperties->getValue("translation", "BA");
    app.init(trLang);
    return app.run();
}
