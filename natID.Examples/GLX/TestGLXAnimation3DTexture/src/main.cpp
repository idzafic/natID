//#include "main.hpp"
#include "Application.h"
#include <td/StringConverter.h>
#include <gui/WinMain.h>

#include <mu/DebugConsole.h>

int main(int argc, const char * argv[])
{
    mu::DebugConsole debugConsole;
    Application app(argc, argv);
    app.init("BA");
    return app.run();
}
