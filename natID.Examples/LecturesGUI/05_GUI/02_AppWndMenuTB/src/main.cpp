//
//  Created by Izudin Dzafic on 10 Nov 2022.
//  Copyright © 2022 IDz. All rights reserved.
//
#include "Application.h"
#include <gui/WinMain.h>

#ifdef USE_WINAPPSDK
namespace gui
{
	NATGUI_API int startDemoApp();
}
#endif
//Napomena: dodati slijedecu liniju u ResPaths.txt (prilagoditi po potrebi):
//appWndMenuTB,0=Work/CPProjects/Lectures/PZI/05_GUI/02_AppWndMenuTB

int main(int argc, const char * argv[])
{
#ifdef USE_WINAPPSDK
    gui::startDemoApp();
#else
   Application app(argc, argv);
   app.init("EN"); //Promjeniti u app.init("EN"); za engleski
//    auto appProperties = app.getProperties();
//    td::String trLang = appProperties->getValue("translation", "BA");
//    app.init(trLang);
   return app.run();
#endif
}
