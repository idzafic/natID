#pragma once
#include "Socket.h"
#include <iostream>
#include <fo/FileOperations.h>
#include <cnt/StringBuilder.h>
#include <mu/mu.h>
#include <future>

class TCPServer : public Socket
{
protected:
	fo::fs::path _homePath;

	td::String handleListOpcode(td::String& path)
	{
		fo::fs::path dir(path.c_str());
		if (!fo::fs::is_directory(dir))
		{
			return "Not a folder.";
		}

		fo::fs::directory_iterator iter(dir);
		cnt::StringBuilderSmall sb;
		for (auto f : iter)
		{
			td::String fpath = f.path();
			sb << fpath << '\n';
		}
		return sb.toString();
	}

	td::String handleOpenOpcode(td::String& path, const td::String& folder)
	{
		fo::fs::path dir(path.c_str());
		fo::fs::path newDir = dir / folder.c_str();
		if (!fo::fs::is_directory(dir) || !fo::fs::is_directory(newDir))
		{
			return "Not a folder.";
		}
		path = newDir.c_str();

		return path;
	}

	td::String handleBackOpcode(td::String& path)
	{
		fo::fs::path dir(path.c_str());
		if (!fo::fs::is_directory(dir))
		{
			return "Not a folder.";
		}
		if (!dir.has_parent_path())
		{
			return "Cannot go back.";

		}
		dir = dir.parent_path();
		path = dir.c_str();
		return path;
	}

	td::String handleGetOpcode(td::String& path, const td::String& file)
	{
		fo::fs::path dir(path.c_str());
		fo::fs::path fileDir = dir / file.c_str();

		if(fo::fs::is_directory(fileDir))
		{
			return "Not a file.";
		}
		else
		{
			td::String strSend;
			bool loaded = fo::loadBinaryFile(fileDir.c_str(), strSend);
			return loaded ? strSend : "Cannot open.";
		}
	}

	void handleClient()
	{
		sockaddr clientAdr;

		SOCKET clientSocket = acceptConnection(clientAdr);
		if (clientSocket == INVALID_SOCKET)
			return;

		std::cout << "Connection made with client on: "
			<< (int)clientAdr.sa_data[2] << '.'
			<< (int)clientAdr.sa_data[3] << '.'
			<< (int)clientAdr.sa_data[4] << '.'
			<< (int)clientAdr.sa_data[5] << '\n';

		td::String path(_homePath);

		try
		{
			while (true)
			{
				td::String message = receiveMsg(clientSocket);

				std::cout << "Message received: " << message << '\n';

				td::String retMsg;
				td::String opcode = message.subStr(8, 15).trim();
				if (opcode.cCompareConstStr("-Close"))
				{
					shutdownClient(clientSocket, SHUT_RDWR);
					closesocket(clientSocket);
					return;
				}
				else if (opcode.cCompareConstStr("-List"))
				{
					retMsg = handleListOpcode(path);
				}
				else if (opcode.cCompareConstStr("-Open"))
				{
					td::String folderName = message.subStr(16, 256).trim();
					retMsg = handleOpenOpcode(path, folderName);
				}
				else if (opcode.cCompareConstStr("-Back"))
				{
					retMsg = handleBackOpcode(path);
				}
				else if (opcode.cCompareConstStr("-Get"))
				{
					td::String fileName = message.subStr(16, 256).trim();
					retMsg = handleGetOpcode(path, fileName);
				}
				if (retMsg.length() > 0)
					sendMsg(clientSocket, retMsg);
			}
		}
		catch (...)
		{
			closesocket(clientSocket);
			return;
		}
	}

	void shutdownClient(SOCKET client, int how)
	{
		int iRes = shutdown(client, how);
		if (iRes != 0)
		{
			throw std::runtime_error("WinSock error");
		}
	}

	SOCKET acceptConnection(sockaddr& client)
	{
		std::memset(&client, 0, sizeof client);
        socklen_t clientAdrSize = sizeof client;
//        accept(int, struct sockaddr * __restrict, socklen_t * __restrict)
        
		return accept(_socket, &client, &clientAdrSize);
	}

public:

	void initAndListenForConnections(td::String port, Protocol protocol)
	{
		try
		{
			initSocketLib();

			addrinfo* result = nullptr, hints;
			std::memset(&hints, 0, sizeof(addrinfo));
			hints.ai_family = AF_INET;
			hints.ai_socktype = protocol == Protocol::TCP ? SOCK_STREAM : SOCK_DGRAM;
			hints.ai_protocol = protocol == Protocol::TCP ? IPPROTO_TCP : IPPROTO_UDP;
			hints.ai_flags = AI_PASSIVE;
			getAdrInfo(nullptr, port.c_str(), &hints);
		
			initSocket();

			bindSocketToAddress();

			listenSocket(SOMAXCONN);


			while (true)
			{

				handleClient();
			}
		}
		catch (...)
		{
			std::cout << "WinSock error code: " << getLastError();
			freeaddrinfo(_adrInfo);
			closesocket(_socket);
			cleanSocketLib();
		}
	}

	TCPServer()
	{
		mu::getHomePath(_homePath);
	}

};
