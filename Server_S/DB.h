#pragma once
//DATABASE

#include <mysql.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#pragma comment(lib, "libmysql.lib")

using namespace std;


class Database {
public:
	Database() {};

	~Database()
	{
		mysql_close(_mysql);
	}

	void connect(const string& host, const string& user, const string& pass, const string& name) {

		_dbHost = host;
		_dbUser = user;
		_dbPass = pass;
		_dbName = name;

		_mysql = mysql_init(NULL);

		if (_mysql == NULL) {

			cout << "Error: can't create MySQL-descriptor" << endl;
			return;
		}

		if (!mysql_real_connect(_mysql, _dbHost.c_str(), _dbUser.c_str(), _dbPass.c_str(), NULL, 0, NULL, 0)) {

			cout << "Error: can't connect to database " << mysql_error(_mysql) << endl;
			return;
		}
		

		string query = "create database if not exists " + _dbName;
		int result = mysql_query(_mysql, query.c_str());
		if (result == 0) {
			cout << "DB exists & connected!" << endl;
		}

		//  ¬±¬à¬Õ¬Ü¬Ý¬ð¬é¬Ö¬ß¬Ú¬Ö ¬Ü ¬¢¬¥
		_mysql = mysql_init(NULL);
		if (!mysql_real_connect(_mysql, _dbHost.c_str(), _dbUser.c_str(), _dbPass.c_str(), _dbName.c_str(), 0, NULL, 0)) {
			// ¬°¬ê¬Ú¬Ò¬Ü¬Ñ ¬á¬à¬Õ¬Ü¬Ý¬ð¬Ø¬Ö¬ß¬Ú¬ñ
			cout << "Error: can't connect to database " << _dbName << "  " << mysql_error(_mysql) << endl;
			return;
		}
		else {
			// ¬µ¬ã¬ä¬Ñ¬ß¬à¬Ó¬Ü¬Ñ ¬ã¬à¬Ö¬Õ¬Ú¬ß¬Ö¬ß¬Ú¬ñ    "Success!"
			cout << _dbName << " is on air! " << endl;
		}

		createTable(); // ¬³¬à¬Ù¬Õ¬Ñ¬ß¬Ú¬Ö ¬ä¬Ñ¬Ò¬Ý¬Ú¬è¬í
		
		//mysql_set_character_set(_mysql, "utf8");
		mysql_set_character_set(_mysql, "utf8mb4");
	}

	bool checkLogin(string& login) {
		string query = "select id from Users where login='" + login + "'";
		mysql_query(_mysql, query.c_str());
		MYSQL_RES* res;
		MYSQL_ROW row;
		if (res = mysql_store_result(_mysql)) {
			if (row = mysql_fetch_row(res))
				return true;
			else
				return false;
		}
	}

	void addUser(const string& name, const string& login, const string& pass) {
		string query = "insert into Users(id, name, login, password) values(default, '" +
			name + "', '" + login + "', '" + pass + "')";
		int res = mysql_query(_mysql, query.c_str());
		if (res == 0) {
			cout << "User " << login << " successfully added" << endl;
		}
		else {
			cout << "Failed to add user " << login << " - " << mysql_error(_mysql) << endl;
		}
	}

	bool checkLoginPass(const string& login, const string& pass) {
		string query = "select password from Users where login='" + login + "'";
		mysql_query(_mysql, query.c_str());
		MYSQL_RES* res;
		MYSQL_ROW row;
		if (res = mysql_store_result(_mysql)) {
			while (row = mysql_fetch_row(res)) {
				if (row[0] == pass)
					return true;
				else
					return false;
			}
		}
	}

	vector<string> allUsers() {
		vector<string> users{};
		string query = "select login from Users";
		mysql_query(_mysql, query.c_str());
		MYSQL_RES* res;
		MYSQL_ROW row;
		if (res = mysql_store_result(_mysql)) {
			while (row = mysql_fetch_row(res)) {
				users.push_back(row[0]);
			}
		}
		return users;
	}

	string getUserId(const string& login) {
		string query = "select id from Users where login='" + login + "'";
		mysql_query(_mysql, query.c_str());
		MYSQL_RES* res;
		MYSQL_ROW row;
		if (res = mysql_store_result(_mysql)) {
			row = mysql_fetch_row(res);
			return row[0];
		}
	}

	string LoginById(const string& id) {
		string query = "select login from Users where id='" + id + "'";
		mysql_query(_mysql, query.c_str());
		MYSQL_RES* res;
		MYSQL_ROW row;
		if (res = mysql_store_result(_mysql)) {
			row = mysql_fetch_row(res);
			return row[0];
		}
	}

	void add_privateMsg(const string& sender, const string& receiver, const string& text) {
		string senderId = getUserId(sender);
		string receiverId = getUserId(receiver);
		string query = "insert into PrivateMessages(id, senderId, receiverId, text) values(default, '" +
			senderId + "', '" + receiverId + "', '" + text + "')";
		int result = mysql_query(_mysql, query.c_str());
		if (result == 0) {
			cout << "The message was added" << endl;
		}
		else {
			cout << "Failed to add message - " << mysql_error(_mysql) << endl;
		}
	}

	void add_publicMsg(const string& sender, const string& text) {
		string senderId = getUserId(sender);
		string query = "insert into Messages(id, senderId, text) values(default, '" +
			senderId + "', '" + text + "')";
		int result = mysql_query(_mysql, query.c_str());
		if (result == 0) {
			cout << "The message was added" << endl;
		}
		else {
			cout << "Failed to add message - " << mysql_error(_mysql) << endl;
		}
	}

	std::vector<string> get_privateMsg(const string& receiver) {
		vector<string> PrivateMsg;
		string message{};
		string query = "select * from PrivateMessages";
		mysql_query(_mysql, query.c_str());
		MYSQL_RES* res;
		MYSQL_ROW row;
		if (res = mysql_store_result(_mysql)) {
			while (row = mysql_fetch_row(res)) {
				if (LoginById(row[2]) == receiver) {
					message = "From: " + LoginById(row[1]) + "\tTo: Me" + "\nText: " + row[3];
					PrivateMsg.push_back(message);
				}
			}
		}
		return PrivateMsg;
	}

	vector<string> get_publicMsg() {
		vector<string> AllMsg{};
		string message{};
		string query = "select * from Messages";
		mysql_query(_mysql, query.c_str());
		MYSQL_RES* res;
		MYSQL_ROW row;
		if (res = mysql_store_result(_mysql)) {
			while (row = mysql_fetch_row(res)) {
				message = "From: " + LoginById(row[1]) + "\tTo: All" + "\nText: " + row[2];
				AllMsg.push_back(message);
			}
		}
		return AllMsg;
	}

	
private:
	MYSQL* _mysql;				// ¬Õ¬Ö¬ã¬Ü¬â¬Ú¬á¬ä¬à¬â ¬ã¬à¬Ö¬Õ¬Ú¬ß¬Ö¬ß¬Ú¬ñ ¬ã ¬¢¬¥
	string _dbHost;		// ¬ç¬à¬ã¬ä ¬¢¬¥
	string _dbUser;		// ¬á¬à¬Ý¬î¬Ù¬à¬Ó¬Ñ¬ä¬Ö¬Ý¬î ¬¢¬¥
	string _dbPass;	// ¬á¬Ñ¬â¬à¬Ý¬î
	string _dbName;		// ¬Ú¬Þ¬ñ ¬¢¬¥

	void createTable() {
		int result{};
		// ¬ä¬Ñ¬Ò¬Ý¬Ú¬è¬Ñ ¬á¬à¬Ý¬î¬Ù¬à¬Ó¬Ñ¬ä¬Ö¬Ý¬Ö¬Û
		result = mysql_query(_mysql, "create table if not exists Users"
			"(id int auto_increment primary key, "
			"name varchar(255), "
			"login varchar(255) not null, "
			"password varchar(255) not null)");
		if (result == 0) {
			cout << "Table 'Users' is ready!" << endl;
		}
		else {
			cout << "No Table 'Users' is found!" << endl;
		}

		//  ¬ä¬Ñ¬Ò¬Ý¬Ú¬è¬Ñ ¬Ý¬Ú¬é¬ß¬í¬ç ¬ã¬à¬à¬Ò¬ë¬Ö¬ß¬Ú¬Û
		result = mysql_query(_mysql, "create table if not exists PrivateMessages"
			"(id int auto_increment primary key,"
			"senderId int references Users(id), "
			"receiverId int references Users(id), "
			"text varchar(1024))");
		if (result == 0) {
			cout << " 'Private Messages' Table is ready!" << endl;
		}
		else {
			cout << " No 'Private Messages' Table is found!" << endl;
		}

		// ¬ä¬Ñ¬Ò¬Ý¬Ú¬è¬Ñ ¬à¬Ò¬ë¬Ú¬ç ¬ã¬à¬à¬Ò¬ë¬Ö¬ß¬Ú¬Û
		result = mysql_query(_mysql, "create table if not exists Messages"
			"(id int auto_increment primary key,"
			"senderId int references Users(id), "
			"text varchar(1024))");
		if (result == 0) {
			cout << "Table 'Messages' is ready!" << endl;
		}
		else {
			cout << "No Table 'Messages' is found!" << endl;
		}
	}
};

