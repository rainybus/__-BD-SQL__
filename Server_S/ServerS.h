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

		WSADATA WSAData; // ���Ѭ߬߬�� 
		WORD ver = MAKEWORD(2, 2);

		int wsInit = WSAStartup(MAKEWORD(2, 2), &WSAData);

		if (wsInit != 0) {
			cout << "Error: can't initialize Winsock." << endl;
			return false;
		}
		return true;
	}

	void startServer() {

		server = socket(AF_INET, SOCK_STREAM, 0); // ��֬�Ӭ֬�

		if (server == INVALID_SOCKET) {
			cout << "Socket creation failed with error:" << WSAGetLastError() << endl;
			return;
		}
		SOCKADDR_IN serverAddr, clientAddr; // ���լ�֬�� ���ܬ֬���
		serverAddr.sin_addr.s_addr = INADDR_ANY;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(_port);

		if (bind(server, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
			cout << "Bind function failed with error: " << WSAGetLastError() << endl;
			return;
		}

		if (listen(server, 0) == SOCKET_ERROR) { // ����ݬ� �߬� ��լѬݬ��� ���ݬ��ڬ�� �٬Ѭ����
			cout << "Listen function failed with error:" << WSAGetLastError() << endl;
			return;
		}
		cout << "Listening for incoming connections...." << endl;

		int clientAddrSize = sizeof(clientAddr); // ���߬ڬ�ڬѬݬڬ٬ڬ��ӬѬ�� �Ѭլ֬�� �ܬݬڬ֬߬��
		if ((client = accept(server, (SOCKADDR*)&clientAddr, &clientAddrSize)) != INVALID_SOCKET) {
			// ����ݬ� ���֬լڬ߬֬߬ڬ� ����Ѭ߬�Ӭݬ֬߬�
			cout << "Client connected!" << endl;
		}
	}

	void closeServer() const {
		closesocket(client); // ���Ѭܬ���� ���ܬ֬�

		if (closesocket(server) == SOCKET_ERROR) { // ����ڬҬܬ� �٬Ѭܬ���ڬ� ���ܬ֬��
			cout << "Close socket failed with error: " << WSAGetLastError() << endl;
			return;
		}
		WSACleanup();
	}

	string Receive() {
		message.clear();
		if (recv(client, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
			// // ����ݬ� �߬� ��լѬݬ��� ���ݬ��ڬ�� �լѬ߬߬�� �Ҭ��֬��, ����Ҭ�ڬ�� ��� ���ڬҬܬ� �� �Ӭ�۬��
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
	SOCKET server, client; //  ����ܬ֬�� ��֬�Ӭ֬�� �� �ܬݬڬ֬߬��
	char buffer[messageLen] = { 0 }; // �����֬� �լѬ߬߬��
	string message{};

};