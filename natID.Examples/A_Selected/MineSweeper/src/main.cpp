
#include "Application.h"
#include <td/StringConverter.h>
#include <gui/WinMain.h>
#include <functional>


int main(int argc, const char * argv[])
{
    Application app(argc, argv);
    auto appProperties = app.getProperties();
    td::String trLang = appProperties->getValue("translation", "BA");
    app.init(trLang);
    int toRet = app.run();
    mu::dbgLog("THE END");
    return toRet;
}


#include "GlobalEvents.h"
std::vector<std::function<void(bool)>> systemAppearanceChangedEvent;
