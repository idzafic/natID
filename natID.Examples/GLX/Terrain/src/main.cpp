//#include "main.hpp"
#include "Application.h"
#include <td/StringConverter.h>
#include <gui/WinMain.h>
#include <mu/DebugConsoleLog.h>
#include "ViewGLX.h"

#if defined(MU_WINDOWS) && defined(ENABLE_CONSOLE)
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>
#endif


#define ENABLE_CONSOLE

int main(int argc, const char * argv[])
{
#if defined(MU_WINDOWS) && defined(ENABLE_CONSOLE)
    if (AllocConsole()) {
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
        freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
        freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
        std::ios::sync_with_stdio(true);
        SetConsoleTitle("Terrain GLX - Debug Console");
        mu::DebugConsoleLog::info() << "Debug console initialized for Terrain GLX application";
    }
#endif

    Application<ViewGLX> app(argc, argv);
    app.init("Terrain GLX - Metal API Demo");
    return app.run();
}
