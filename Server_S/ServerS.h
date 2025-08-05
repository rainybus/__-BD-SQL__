#pragma once
#include <iostream> 
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma warning(disable: 4996)
#pragma comment(lib, "ws2_32.lib")
using namespace std;

constexpr int messageLen = 1024;

class Server
{
public:
	Server() {}

	~Server() {}

	bool sockInit(string ip, uint16_t port) {

		_ip = ip;
		_port = port;

		WSADATA WSAData; // ¬¥¬Ñ¬ß¬ß¬í¬Ö 
		WORD ver = MAKEWORD(2, 2);

		int wsInit = WSAStartup(MAKEWORD(2, 2), &WSAData);

		if (wsInit != 0) {
			cout << "Error: can't initialize Winsock." << endl;
			return false;
		}
		return true;
	}

	void startServer() {

		server = socket(AF_INET, SOCK_STREAM, 0); // ¬ã¬Ö¬â¬Ó¬Ö¬â

		if (server == INVALID_SOCKET) {
			cout << "Socket creation failed with error:" << WSAGetLastError() << endl;
			return;
		}
		SOCKADDR_IN serverAddr, clientAddr; // ¬¡¬Õ¬â¬Ö¬ã¬Ñ ¬ã¬à¬Ü¬Ö¬ä¬à¬Ó
		serverAddr.sin_addr.s_addr = INADDR_ANY;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(_port);

		if (bind(server, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
			cout << "Bind function failed with error: " << WSAGetLastError() << endl;
			return;
		}

		if (listen(server, 0) == SOCKET_ERROR) { // ¬¦¬ã¬Ý¬Ú ¬ß¬Ö ¬å¬Õ¬Ñ¬Ý¬à¬ã¬î ¬á¬à¬Ý¬å¬é¬Ú¬ä¬î ¬Ù¬Ñ¬á¬â¬à¬ã
			cout << "Listen function failed with error:" << WSAGetLastError() << endl;
			return;
		}
		cout << "Listening for incoming connections...." << endl;

		int clientAddrSize = sizeof(clientAddr); // ¬ª¬ß¬Ú¬è¬Ú¬Ñ¬Ý¬Ú¬Ù¬Ú¬â¬à¬Ó¬Ñ¬ä¬î ¬Ñ¬Õ¬Ö¬â¬ã ¬Ü¬Ý¬Ú¬Ö¬ß¬ä¬Ñ
		if ((client = accept(server, (SOCKADDR*)&clientAddr, &clientAddrSize)) != INVALID_SOCKET) {
			// ¬¦¬ã¬Ý¬Ú ¬ã¬à¬Ö¬Õ¬Ú¬ß¬Ö¬ß¬Ú¬Ö ¬å¬ã¬ä¬Ñ¬ß¬à¬Ó¬Ý¬Ö¬ß¬à
			cout << "Client connected!" << endl;
		}
	}

	void closeServer() const {
		closesocket(client); // ¬©¬Ñ¬Ü¬â¬í¬ä¬î ¬ã¬à¬Ü¬Ö¬ä

		if (closesocket(server) == SOCKET_ERROR) { // ¬°¬ê¬Ú¬Ò¬Ü¬Ñ ¬Ù¬Ñ¬Ü¬â¬í¬ä¬Ú¬ñ ¬ã¬à¬Ü¬Ö¬ä¬Ñ
			cout << "Close socket failed with error: " << WSAGetLastError() << endl;
			return;
		}
		WSACleanup();
	}

	string Receive() {
		message.clear();
		if (recv(client, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
			// // ¬¦¬ã¬Ý¬Ú ¬ß¬Ö ¬å¬Õ¬Ñ¬Ý¬à¬ã¬î ¬á¬à¬Ý¬å¬é¬Ú¬ä¬î ¬Õ¬Ñ¬ß¬ß¬í¬Ö ¬Ò¬å¬æ¬Ö¬â¬Ñ, ¬ã¬à¬à¬Ò¬ë¬Ú¬ä¬î ¬à¬Ò ¬à¬ê¬Ú¬Ò¬Ü¬Ö ¬Ú ¬Ó¬í¬Û¬ä¬Ú
			std::cout << "recv function failed with error " << WSAGetLastError() << endl;
			return "";
		}
		message = buffer;
		memset(buffer, 0, sizeof(buffer));

		return message;

	}


	void Send(const string& mes) {
		if (send(client, mes.c_str(), sizeof(mes), 0) == SOCKET_ERROR) {
			cout << "send failed with error " << WSAGetLastError() << endl;
			return;
		}
	}

private:
	string _ip{};
	uint16_t _port{};
	SOCKET server, client; //  ¬³¬à¬Ü¬Ö¬ä¬í ¬ã¬Ö¬â¬Ó¬Ö¬â¬Ñ ¬Ú ¬Ü¬Ý¬Ú¬Ö¬ß¬ä¬Ñ
	char buffer[messageLen] = { 0 }; // ¬¢¬å¬æ¬Ö¬â ¬Õ¬Ñ¬ß¬ß¬í¬ç
	string message{};

};