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

private:
	MYSQL* _mysql;				// дескриптор соединения с БД
	string _dbHost;		// хост БД
	string _dbUser;		// пользователь БД
	string _dbPass;	// пароль
	string _dbName;		// имя БД

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

		//  Подключение к БД
		_mysql = mysql_init(NULL);
		if (!mysql_real_connect(_mysql, _dbHost.c_str(), _dbUser.c_str(), _dbPass.c_str(), _dbName.c_str(), 0, NULL, 0)) {
			// Ошибка подклюжения
			cout << "Error: can't connect to database " << _dbName << "  " << mysql_error(_mysql) << endl;
			return;
		}
		else {
			// Установка соединения    "Success!"
			cout << _dbName << " is on air! " << endl;
		}

		createTable(); // Создание таблицы
		
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

	void createTable() {
		int result{};
		// таблица пользователей
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

		//  таблица личных сообщений
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

		// таблица общих сообщений
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



