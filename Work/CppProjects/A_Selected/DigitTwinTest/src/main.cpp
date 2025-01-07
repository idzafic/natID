//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
//
#include <mem/LeakDetector.h>
#include <gui/WinMain.h>
#include <td/StringConverter.h>
#include "Application.h"

int main(int argc, const char * argv[])
{
    Application app(argc, argv);
    //enable these lines to test memory leak detector (definition MU_SHOW_MEM_LEAKS must be available for the project)
    td::UINT4* leak = new td::UINT4[4];  //<--- Memory leak (4 x 4 bytes = 16 bytes)
    leak = new  td::UINT4[10]; // this one is deleted
    delete[] leak;
    //load properties from OS environment (registry on windows, plist on mac, settings scheme on linux...)
    auto appProperties = app.getProperties();
    td::String trLang = appProperties->getValue("translation", "EN");
    app.init(trLang);
    return app.run();
}
