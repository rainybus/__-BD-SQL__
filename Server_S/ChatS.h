#pragma once

#include"logger.h"
#include "ServerS.h"
#include "DB.h"
#include <fstream>
#include <string>

class Chat_S {
public:
	Chat_S(const string& ip, const uint16_t port,
		const string& dbHost, const string& dbUser, const string& dbPass, const string& dbName) :
		_entry(false), _conn(false), _ip(ip), _port(port),
		_dbHost(dbHost), _dbUser(dbUser), _dbPass(dbPass), _dbName(dbName)

	{
		_db.connect(_dbHost, _dbUser, _dbPass, _dbName);
		if (_server.sockInit(_ip, _port)) _server.startServer();
	}
	//void* param = static_cast<string*>;

	~Chat_S() = default;

	void run() {
		_conn = true;
		while (_conn)
		{

			string msg{ };
			msg = _server.Receive();
			string name, login, pass;
			vector<string> users{};
			bool loginOk = false, passOk = false;
			string to, text;
			vector<string> userMsg;
			string newPass{};

			cout << "User: " << msg << endl;
			if (!_entry && (msg == "1" || msg == "2" || msg == "0"))
				_server.Send("1 - correct : proceed");
			else if (_entry && msg == "3" || msg == "4" || msg == "5" || msg == "6")
				_server.Send("1 - correct : proceed");
			else
				_server.Send("2 - operation is incorrect");

			switch (stoi(msg))
			{
			case 1:
				cout << "LOGIN MENU" << endl;
				loginOk = false;
				passOk = false;
				while (!loginOk) {
					login = _server.Receive();
					if (_db.checkLogin(login)) {
						loginOk = true;
						cout << "User's login: " << login << endl;
						_server.Send("1");
					}
					else {
						_server.Send("0");
					}
					while (!passOk) {
						pass = _server.Receive();
						if (_db.checkLoginPass(login, pass)) {
							_currUser = login;
							_entry = true;
							passOk = true;
							cout << "User's password is correct!" << endl;
							_server.Send("1");
						}
						else
							_server.Send("0");
					}
				}
				break;
			case 2:
				cout << "SIGN_UP MENU" << endl;
				name = _server.Receive();
				cout << "User's name: " << name << endl;
				while (!loginOk) {
					msg = _server.Receive();
					if (_db.checkLogin(msg))
						_server.Send("1");
					else {
						login = msg;
						cout << "User's login: " << login << endl;
						loginOk = true;
						_server.Send("0");
					}
				}
				pass = _server.Receive();
				cout << "User's password: " << pass << endl;
				_db.addUser(name, login, pass);
				_server.Send("User was added!");
				_currUser = login;
				_entry = true;
				break;
			case 3:
				cout << "USERS IN CHAT MENU" << endl;
				users = _db.allUsers();
				_server.Send(to_string(users.size()));
				_server.Receive();
				for (int i = 0; i < users.size(); i++) {
					_server.Send(users[i]);
					_server.Receive();
				}
				break;
			case 4:
				cout << "MESSAGES" << endl;
				msg = _server.Receive();
				switch (stoi(msg))
				{
				case 1:
					userMsg.clear();
					userMsg = _db.get_publicMsg();
					_server.Send(to_string(userMsg.size()));
					if (userMsg.size() == 0)
						cout << "No messages in DB!" << endl;
					else {
						cout << "Messages : " << userMsg.size() << endl;
						_server.Receive();
						for (int i = 0; i < userMsg.size(); i++) {
							_server.Send(userMsg[i]);
							_server.Receive();
						}
					}
					break;
				case 2:
					userMsg.clear();
					userMsg = _db.get_privateMsg(_currUser);
					_server.Send(to_string(userMsg.size()));
					if (userMsg.size() == 0)
						cout << "No messages to  " << _currUser << " in DB!" << endl;
					else {
						cout << "Messages to " << _currUser << ": " << userMsg.size() << endl;
						_server.Receive();
						for (int i = 0; i < userMsg.size(); i++) {
							_server.Send(userMsg[i]);
							_server.Receive();
						}
					}
					break;
				default:
					break;
				}
				break;
			case 5:
				cout << "WRITE MESSAGE TO CHAT" << endl;
				loginOk = false;
				while (!loginOk) {
					to = _server.Receive();
					
					if (!(to == "all")) {
						if (_db.checkLogin(to)) {
							loginOk = true;
							cout << "Message to: " << to << endl;
							_server.Send("1");
							text = _server.Receive();
							_db.add_privateMsg(_currUser, to, text);
							log.write(to, _currUser, text);
						}
						else
							_server.Send("0");
					}
					else {
						loginOk = true;
						text = _server.Receive();
						_db.add_publicMsg(_currUser, text);
						log.write(to, _currUser, text);
					} 
				}
				break;
			case 6:
				cout << "Enter SIGN OUT menu" << endl;
				cout << "User is offline" << endl;
				_entry = false;
				break;
			case 0:
				cout << "Enter EXIT menu" << endl;

				_conn = false;
				_server.closeServer();
				break;
			default:
				break;

			}
		}
	}

private:
	string _currUser;	//Пользователь он-лайн
	bool _entry;			// Проверка вошедших = присутствующих
	bool _conn;

	Server _server;
	string _ip;		    // IP сервер
	uint16_t _port;			    // порт сервера

	Database _db;			/// Объект БД
	string _dbHost;		// Адрес БД
	string _dbUser;		// Пользователь БД
	string _dbPass;	// Пароль пользователя БД
	string _dbName;		// Имя БД
	Logger log;
};
