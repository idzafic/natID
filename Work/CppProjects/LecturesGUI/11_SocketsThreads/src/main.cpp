#include "WinSockLib.h"
#include "TCPClient.h"
#include "TCPServer.h"
#include "ThreadExample.h"

int main()
{
	int opt;
	std::cout << "Enter 1 for TCPServer, 2 for TCPClient, 3 for TreadTest:";
	std::cin >> opt;
	if (opt == 1)
	{
		td::String port = "44201";
		//std::cout << "Enter server port number: ";
		//std::cin >> port;

		TCPServer s;

		s.initAndListenForConnections(port, TCPServer::Protocol::TCP);
	}
	else if (opt == 2)
	{
		td::String serverIP = "localhost";
		//std::cout << "Enter server IP: ";
		//std::cin >> serverIP;

		td::String port = "44201";
		//std::cout << "Enter port number: ";
		//std::cin >> port;

		TCPClient c;
		if(c.init(serverIP, port, TCPServer::Protocol::TCP))
			c.handleConnected();
	}
    else if (opt == 3)
    {
        testThread();
    }

	return 0;
}
