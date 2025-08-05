
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream> 
#include <string>
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")

using namespace std;

constexpr int messageLen = 1024;

class Client
{
public:
	Client() {}

	~Client() {}

	bool sockInit(string ip, int port) {

		_ip = ip;
		_port = port;

		WSADATA WSAData; //¬¥¬Ñ¬ß¬ß¬í¬Ö
		int wsInit = WSAStartup(MAKEWORD(2, 2), &WSAData);

		if (wsInit != 0) {
			cout << "Failed to initialize Winsock." << endl;
			return false;
		}
		return true;
	}

	void startClient() {

		client = socket(AF_INET, SOCK_STREAM, 0); // ¬¬¬Ý¬Ú¬Ö¬ß¬ä

		if (client == INVALID_SOCKET) {
			cout << "Failed to create socket : " << WSAGetLastError() << endl;
			return;
		}

		SOCKADDR_IN clientAddr; //¬¡¬Õ¬â¬Ö¬ã ¬ã¬à¬Ü¬Ö¬ä¬Ñ
		clientAddr.sin_addr.s_addr = inet_addr(_ip.c_str()); ///¬ã¬à¬Ö¬Õ¬Ú¬ß¬Ö¬ß¬Ú¬Ö ¬ã ¬ã¬Ö¬â¬Ó¬Ö¬â¬à¬Þ
		clientAddr.sin_family = AF_INET;
		clientAddr.sin_port = htons(_port); // ¬á¬à¬â¬ä
		if (connect(client, (SOCKADDR*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
			cout << "Failed to connect server : " << WSAGetLastError() << endl;
			return;
		}

		cout << "Server connected!" << endl;
	}

	void closeClient() {
		closesocket(client); ////¬©¬Ñ¬Ü¬â¬í¬ä¬î ¬ã¬à¬Ü¬Ö¬ä
		WSACleanup();
	}

	string Receive() {
		if (recv(client, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
			//¬°¬ê¬Ú¬Ò¬Ü¬Ñ ¬á¬à¬Ý¬å¬é¬Ö¬ß¬Ú¬ñ ¬Õ¬Ñ¬ß¬ß¬í¬ç ¬Ú ¬Ó¬í¬ç¬à¬Õ
			cout << "Receiving is failed : " << WSAGetLastError() << endl;
			return "";
		}
		message = buffer;
		memset(buffer, 0, sizeof(buffer));
		return message;
	}

	void Send(const string& mes) {
		if (send(client, mes.c_str(), sizeof(mes), 0) == SOCKET_ERROR) {
			cout << "sending is failed : " << WSAGetLastError() << endl;
			return;
		}
	}

private:
	string _ip{};
	int _port{};
	SOCKET client; //¬³¬à¬Ü¬Ö¬ä ¬Ü¬Ý¬Ú¬Ö¬ß¬ä¬Ñ
	char buffer[messageLen] = { 0 }; //¬¢¬å¬æ¬Ö¬â ¬Õ¬Ñ¬ß¬ß¬í¬ç
	string message{};
};

