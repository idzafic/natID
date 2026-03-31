//#include "main.hpp"
#include "Application.h"
#include <td/StringConverter.h>
#include <gui/WinMain.h>

#if defined(MU_WINDOWS) && defined(ENABLE_CONSOLE)
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>
#endif

#include <mu/DebugConsoleLog.h>

#define ENABLE_CONSOLE

int main(int argc, const char * argv[])
{
#if defined(MU_WINDOWS) && defined(ENABLE_CONSOLE)
    if (AllocConsole()) {
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
        freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
        freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
        std::ios::sync_with_stdio(true);
        SetConsoleTitle("TestSkybox - Debug Console");
        mu::DebugConsoleLog::ok() << "Debug console initialized for TestSkybox application";
    }
#endif

    Application app(argc, argv);
    app.init("BA");
    return app.run();
}
