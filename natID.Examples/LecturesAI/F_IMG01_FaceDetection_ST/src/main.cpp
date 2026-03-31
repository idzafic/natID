//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//
#include "Application.h"
#include <td/StringConverter.h>
#include <gui/WinMain.h>

int main(int argc, const char * argv[])
{
    Application app(argc, argv);
    mu::dbgLog("INFO: Application just created!");
    //load properties from OS environment (registry on windows, plist on mac, settings scheme on linux...)
    auto appProperties = app.getProperties();
    td::String trLang = appProperties->getValue("translation", "EN");
    app.init(trLang);
    return app.run();
}
