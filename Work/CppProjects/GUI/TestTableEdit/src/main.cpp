//#include "main.hpp"
#include "Application.h"
#include <td/StringConverter.h>
#include <gui/WinMain.h>
#include <mu/IAppSettings.h>

int main(int argc, const char * argv[])
{
    Application app(argc, argv);
    app.init("EN");
    mu::IAppSettings* pSettings = app.getSettings();
    const auto& logPath = pSettings->getLogPath();
    td::String logFolder;
    pSettings->getLogFolder(logFolder);
    
    td::String strLocalAppPath = pSettings->getLocalDataAppPath(true);
    return app.run();
}
