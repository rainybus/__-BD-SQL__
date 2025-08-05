#pragma once

#include "ClientC.h"
#include <string>
#include <iostream>

using namespace std;

class Chat_C {
public:
	Chat_C(const string& ip, uint16_t port) :
		_ip(ip), _port(port), _conn(false), _entry(false) {

		if (_client.sockInit(_ip, _port)) _client.startClient();
	}

	~Chat_C() = default;

	void run() {
		_conn = true;
		string msg{};
		
		while (_conn)
		{
			// ¬Þ¬Ö¬ß¬ð ¬â¬Ö¬Ô¬Ú¬ã¬ä¬â¬Ñ¬è¬Ú¬Ú ¬Ú¬Ý¬Ú ¬Ñ¬Ó¬ä¬à¬â¬Ú¬Ù¬Ñ¬è¬Ú¬Ú ¬á¬à¬Ý¬î¬Ù¬à¬Ó¬Ñ¬ä¬Ö¬Ý¬ñ
			if (!_entry) {
				cout << " \n1 - Authentication \n2 - Registration \n0 - Exit" << endl;
			}
			else {
				cout << "\n User menu " << endl;
				cout << "\n3 - Show all users \n4 - Show all messages"
					<< "\n5 - Write message to chat \n6 - Sign Out" << endl;
			}

			int op{};
			cout << "\n Choose an Opt: " << endl;
			cin >> op;
			_client.Send(to_string(op));
			msg = _client.Receive();
			cout << "Server: " << msg << endl;
			if (msg[0] == '1') {
				string name, login, pass;
				int userCount{};
				bool loginOk = false, passOk = false;
				string to, text;
				int msgCount{};
				string newPass{};

				switch (op)
				{
				case 1:
					cout << "\n\t SIGN_IN" << endl;
					loginOk = false;
					passOk = false;
					while (!loginOk) {
						cout << "Login: " << endl;
						cin.clear();
						cin >> login;
						_client.Send(login);
						if (_client.Receive() == "1") {
							loginOk = true;
							while (!passOk) {
								cout << "Password: " << endl;
								cin.clear();
								cin >> pass;
								_client.Send(pass);
								if (_client.Receive() == "1") {
									std::cout << "\n*Hello " << login << "!*" << endl;
									_currUser = login;
									_entry = true;
									passOk = true;
								}
								else {
									cout << "Incorrect Password, try the one!" << endl;
								}
							}
						}
						else {
							cout << "Incorrect Login, try the one!" << endl;
						}
					}
					break;
				case 2:
					cout << "\n SIGN_UP" << endl;
					cout << "Name: " << endl;
					cin >> name;
					_client.Send(name);
					while (!passOk) {
						cout << "Login: " << endl;
						cin >> login;
						_client.Send(login);
						if (_client.Receive() == "0") {
							cout << "Password: " << endl;
							cin >> pass;
							_client.Send(pass);
							passOk = true;
						}
						else
							cout << "Login is in use, try the one" << endl;
					}
					cout << "Server: " << _client.Receive() << endl;
					_entry = true;
					_currUser = login;
					break;
				case 3:
					cout << "\n\t USERS ONLINE" << endl;
					userCount = stoi(_client.Receive());
					_client.Send(" ");
					cout << "Users' q'ty: " << userCount << endl;
					cout << "Logins:\n";
					for (int i = 0; i < userCount; i++) {
						login = _client.Receive();
						_client.Send(" ");
						if (login == _currUser)
							cout << login << " (You)" << endl;
						else
							cout << login << endl;
					}
					break;
				case 4:
					cout << "\n\t MESSAGES IN CHAT " << endl;
					cout << " \n1 - Public messages \n2 - Private messages" << endl;
					cout << _currUser << ": " << endl;
					cin.ignore();
					cin >> op;
					_client.Send(to_string(op));
					switch (op)
					{
					case 1:
						msgCount = stoi(_client.Receive());
						if (msgCount == 0)
							cout << "No public messages!" << endl;
						else {
							cout << "Public messages : " << msgCount << endl;
							_client.Send(" ");
							for (int i = 0; i < msgCount; i++) {
								cout << _client.Receive() << endl;
								_client.Send(" ");
							}
						}
						break;
					case 2:
						msgCount = stoi(_client.Receive());
						if (msgCount == 0)
							cout << "No messages to you, " << _currUser << endl;
						else {
							cout << "Messages to you: " << msgCount << endl;
							_client.Send(" ");
							for (int i = 0; i < msgCount; i++) {
								cout << _client.Receive() << endl;
								_client.Send(" ");
							}
						}
						break;
					default:
						break;
					}
					break;
				case 5:
					cout << "\n\t WRITE MESSAGE TO CHAT " << endl;
					loginOk = false;
					while (!loginOk) {
						cout << "To (Login of user or to \"all\" users): " << endl;
						cin >> to;
						_client.Send(to);
						if (!(to == "all")) {
							if (_client.Receive() == "1") {
								loginOk = true;
								cout << "Write text:" << endl;
								cin.ignore();
								getline(cin, text);
								_client.Send(text);
							}
							else
								cout << "No username " << to << endl;
						}
						else {
							loginOk = true;
							cout << "Write text:" << endl;
							cin.ignore();
							getline(cin, text);
							_client.Send(text);
						}
					}
					break;
					
				case 6:
					cout << "\n\t SIGN OUT " << endl;
					cout << "User is offline" << endl;
					_entry = false;
					break;
				case 0:
					cout << "\n\t EXIT " << endl;

					_conn = false;
					_client.closeClient();
					break;
				default:
					break;
				}
			}
		}
	}

private:
	Client _client;
	string _ip;		//IP ¬ã¬Ö¬â¬Ó¬Ö¬â¬Ñ 
	uint16_t _port;			//No. ¬á¬à¬â¬ä¬Ñ ¬ã¬Ö¬â¬Ó¬Ö¬â¬Ñ
	bool _conn;
	bool _entry;			//¬±¬â¬Ú¬ã¬å¬ä¬ã¬â¬Ó¬Ú¬Ö = ¬Ó¬ç¬à¬Õ ¬á¬à¬Ý¬î¬Ù¬à¬Ó¬Ñ¬ä¬Ö¬Ý¬ñ '¬à¬ß¬Ý¬Ñ¬Û¬ß'
	string _currUser;	// ¬±¬à¬Ý¬î¬Ù¬à¬Ó¬Ñ¬ä¬Ö¬Ý¬î '¬à¬ß¬Ý¬Ñ¬Û¬ß'
};

