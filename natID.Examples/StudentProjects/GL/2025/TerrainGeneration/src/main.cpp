/* Terrain generation project */
/* Created by Kovacevic Emina and Tinjak Amina */

#include "Application.h"
#include <td/StringConverter.h>
#include <gui/WinMain.h>

// Main function: initializes and runs the application
int main(int argc, const char * argv[])
{
    Application app(argc, argv);
    app.init("BA");
    return app.run();
}
