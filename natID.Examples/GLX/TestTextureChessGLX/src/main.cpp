//#include "main.hpp"
#include "Application.h"
#include <td/StringConverter.h>
#include <gui/WinMain.h>
#include "ViewGLX.h"
#include <mu/DebugConsoleLog.h>

#define ENABLE_CONSOLE

#if defined(MU_WINDOWS) && defined(ENABLE_CONSOLE)
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>
#endif

int main(int argc, const char * argv[])
{
#if defined(MU_WINDOWS) && defined(ENABLE_CONSOLE)
    if (AllocConsole()) {
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
        freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
        freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
        std::ios::sync_with_stdio(true);
        SetConsoleTitle("TestTextureChessGLX - Debug Console");
        mu::DebugConsoleLog::info() << "Debug console initialized for TestTextureChessGLX application";
    }
#endif

    Application app(argc, argv);
    app.init("EN");
    return app.run();
}
