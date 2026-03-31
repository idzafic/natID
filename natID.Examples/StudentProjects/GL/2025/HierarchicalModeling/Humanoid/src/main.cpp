//#include "main.hpp"
#include "Application.h"
#include <td/StringConverter.h>
#include <gui/WinMain.h>



int main(int argc, const char * argv[])
{
    Application app(argc, argv);
    app.init("BA");
    return app.run();
}
