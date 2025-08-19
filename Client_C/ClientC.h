
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

		WSADATA WSAData; //Данные
		int wsInit = WSAStartup(MAKEWORD(2, 2), &WSAData);

		if (wsInit != 0) {
			cout << "Failed to initialize Winsock." << endl;
			return false;
		}
		return true;
	}

	void startClient() {

		client = socket(AF_INET, SOCK_STREAM, 0); // Клиент

		if (client == INVALID_SOCKET) {
			cout << "Failed to create socket : " << WSAGetLastError() << endl;
			return;
		}

		SOCKADDR_IN clientAddr; //Адрес сокета
		clientAddr.sin_addr.s_addr = inet_addr(_ip.c_str()); ///соединение с сервером
		clientAddr.sin_family = AF_INET;
		clientAddr.sin_port = htons(_port); // порт
		if (connect(client, (SOCKADDR*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
			cout << "Failed to connect server : " << WSAGetLastError() << endl;
			return;
		}

		cout << "Server connected!" << endl;
	}

	void closeClient() {
		closesocket(client); ////Закрыть сокет
		WSACleanup();
	}

	string Receive() {
		if (recv(client, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
			//Ошибка получения данных и выход
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
	SOCKET client; //Сокет клиента
	char buffer[messageLen] = { 0 }; //Буфер данных
	string message{};
};
