#include <mu/Application.h>
#include "HTTPTester.h"
#include "MixTests.h"


int main(int argc, const char * argv[])
{
    mu::Application app(argc, argv);
    auto intSize = sizeof(int);
    std::cout << "Size of int = " << intSize << std::endl;
    
    bool work = true;
    do
    {
        std::cout<< "Menu:" << td::endl
            << "\t 1. Show Network Interfaces" << td::endl
            << "\t 2. Test encrypt/decrypt" << td::endl
            << "\t 3. Test HTTP POST and GET" << td::endl
            << "\t 4. Test map downloader (returns data in td::String)" << td::endl
            << "\t 5. Test map downloader (returns data via HTTP::IConsumer)" << td::endl
            << "\t 0. Exit" << td::endl
            << "Your selection:";
        
        int selection = 0;
        std::cin >> selection;
        if (selection == 0)
            return 0;
        const char* pLaptopIP="192.168.0.14";
        const char* pi7_9700K = "localhost";
        const char* pLocalHost = "localhost";
        const char* pServerAddress = pLocalHost;
        
        switch(selection)
        {
            case 1:
                testNIFs();
                break;
            case 2:
                testSecu();
                break;
            case 3:
                testTTPGetAndPos();
                break;
            case 4:
                testMapDownloaderStr();
                break;
            case 5:
                testMapDownloaderConsumer();
                break;
            default:
            {
                std::cout << "Wrong selection! Please try again..." << td::endl << td::endl;
                continue;
            }
        }
        
    }while (true);
   
}
