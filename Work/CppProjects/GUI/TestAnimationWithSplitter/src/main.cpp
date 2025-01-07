//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
//
#include "Application.h"
#include <td/StringConverter.h>
#include <gui/WinMain.h>

//Napomena: korigovati ResPaths.txt

int main(int argc, const char * argv[])
{
    Application app(argc, argv);
    //load properties from OS environment (registry on windows, plist on mac, settings scheme on linux...)
    auto appProperties = app.getProperties();
    td::String trLang = appProperties->getValue("translation", "BA");
    app.init(trLang);
    return app.run();
}
