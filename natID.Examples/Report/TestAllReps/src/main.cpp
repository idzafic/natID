//#include "main.hpp"
#include <td/StringConverter.h>
#include "Application.h"
#include <gui/WinMain.h>
//#include "Resources.h"

int main(int argc, const char * argv[])
{
    Application app(argc, argv);
    mu::setLogLevel(20000);
    app.init("BA");

    return app.run();

}
