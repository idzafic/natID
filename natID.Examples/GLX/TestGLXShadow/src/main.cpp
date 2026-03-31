//#include "main.hpp"
#include "Application.h"
#include <td/StringConverter.h>
#include <gui/WinMain.h>

#include <mu/DebugConsole.h>    //creates debug console for gui app (visible only in MU_DEBUG mode)
#include <mu/DebugConsoleLog.h> //logs (only active in MU_DEBUG mode) with colors

int main(int argc, const char * argv[])
{
    mu::DebugConsole console("MyApp Debug Console");
    MU_DEBUG_LOG_INFO("App started");

    Application app(argc, argv);
    app.init("BA");
    return app.run();
}
