#pragma once
#include "Socket.h"
#include <iostream>
#include <fstream>
#include <fo/FileOperations.h>

class TCPClient : public Socket
{
protected:
	void connectToServer()
	{
		for (auto adr = _adrInfo; adr != nullptr && _socket == INVALID_SOCKET; adr = adr->ai_next) {

			_socket = socket(adr->ai_family, adr->ai_socktype, adr->ai_protocol);
			if (_socket == INVALID_SOCKET)
				continue;

			int iResult = connect(_socket, adr->ai_addr, (int)adr->ai_addrlen);
			if (iResult != 0)
			{
				if (closesocket(_socket) != 0)
					throw std::runtime_error("WinSock error");
				_socket = INVALID_SOCKET;
			}
		}
	}
public:

	void handleConnected()
	{
		try
		{
			while (true)
			{
				std::cout <<
					"Enter 1 to display current path"
					"\nEnter 2 followed by folder name to open folder"
					"\nEnter 3 to go back to parent folder"
					"\nEnter 4 followed by file name to download file"
					"\nEnter 5 to disconnect\n";

				int option;
				std::cin >> option;
				if (option == 5)
				{
					td::String message = "00000016" "-Close  ";
					sendMsg(_socket, message);
				}
				else if (option == 1)
				{
					td::String message = "00000016" "-List   ";
					sendMsg(_socket, message);

					td::String answer = receiveMsg(_socket);
					std::cout << "Answer:\n" << answer << "\n";
				}
				else if (option == 2)
				{
					td::String message = "00000016" "-Open   ";
					td::String folder;
					std::cin >> folder;
					message += folder;
					putLengthToStringFront(message, 8);
					sendMsg(_socket, message);

					td::String answer = receiveMsg(_socket);
					std::cout << "Answer:\n" << answer << "\n";
				}
				else if (option == 3)
				{
					td::String message = "00000016" "-Back   ";
					sendMsg(_socket, message);

					td::String answer = receiveMsg(_socket);
					std::cout << "Answer:\n" << answer << "\n";
				}
				else if (option == 4)
				{
					td::String message = "00000016" "-Get    ";
					td::String file;
					std::cin >> file;
					message += file;
					putLengthToStringFront(message, 8);

					sendMsg(_socket, message);

					td::String answer = receiveMsg(_socket);

					fo::fs::path homePath;
					mu::getHomePath(homePath);

					fo::fs::path downLoadsPath = homePath /= "Downloads";
					fo::fs::path filePath = downLoadsPath / file.c_str();

					td::String stemToFind = filePath.stem();
					td::String extension = filePath.extension();
					td::UINT4 stemLenToLook = stemToFind.length() - 1;
					td::UINT4 highestStem = 0;

					fo::fs::directory_iterator iterFolder(downLoadsPath);
					for (auto dir : iterFolder)
					{
						if (fo::fs::is_directory(dir))
							continue;
						td::String stemNew = dir.path().stem();
						if (stemNew.subStr(0, stemLenToLook) == stemToFind)
						{
							auto lPos = stemNew.find("(");
							auto rPos = stemNew.findFromRight(')');
							stemNew = stemNew.subStr(lPos + 1, rPos - 1);
							highestStem = std::max(stemNew.toUINT4(), highestStem);
						}

					}
					cnt::StringBuilderSmall sb;
					sb << stemToFind << " (" << highestStem + 1 << ")" << extension;
					fo::fs::path savePath = downLoadsPath /= sb.toString().c_str();


					std::ofstream fileStream;
					fileStream.open(savePath, std::ios::out | std::ios::binary);
					if (!fileStream.is_open())
						continue;
					fo::writeString(fileStream, answer);
					fileStream.close();

					std::cout << "Answer:\n" << "File saved to: " << savePath << "\n";
				}
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

	bool init(td::String serverIP, td::String port, Protocol protocol)
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
			getAdrInfo(serverIP.c_str(), port.c_str(), &hints);

			connectToServer();
			if (_socket == INVALID_SOCKET)
			{
				std::cout << "WinSock error code: " << getLastError();
				freeaddrinfo(_adrInfo);
				closesocket(_socket);
				cleanSocketLib();
				return false;
			}
			return true;
		}
		catch (...)
		{
			std::cout << "WinSock error code: " << getLastError();

			freeaddrinfo(_adrInfo);
			closesocket(_socket);
			cleanSocketLib();
			return false;
		}

	}

};
