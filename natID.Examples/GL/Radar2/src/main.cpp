//#include "main.hpp"
#include "Application.h"
#include <td/StringConverter.h>
#include <gui/WinMain.h>
#include "ViewGL.h"

int main(int argc, const char * argv[])
{

        Application<ViewGL_PixelShader> app(argc, argv);
        app.init("BA");
        return app.run();
 
}
