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

    fo::fs::path homePath;
    mu::getHomePath(homePath);
	fo::fs::path testDBPath = (homePath / "other_bin/TestData/natGUITest/TestAllReps.db");

	dp::IDatabase* pDB = dp::create(dp::IDatabase::ConnType::CT_SQLITE, dp::IDatabase::ServerType::SER_SQLITE3);
	if (!pDB->connect(testDBPath.string().c_str()))
        return 1;

    dp::setMainDatabase(pDB);

    return app.run();

}
