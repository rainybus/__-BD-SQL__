#include "Chat.h"

using namespace std;


Chat::Chat() {};

bool Chat::parser(const char* mes)
{
	char name[20];
	
	char login[20];
	char pass[20];
	const char* str = strstr(mes, "_");
	if (str)
	{
		int size = strlen(mes) - strlen(str);
		strncpy(name, mes, size);
		name[size] = 0;
		str = str + 1;
		size = 0;
		const char* str1 = strstr(str, "_");
		if (str1)
		{
			size = strlen(str) - strlen(str1);
			strncpy(login, str, size);
			login[size] = 0;
			str1 = str1 + 1;
			size = 0;
			const char* str2 = strstr(str1, "_");
			if (str2)
			{
				size = strlen(str1) - strlen(str2);
				strncpy(login, str1, size);
				login[size] = 0;
				str2 = str2 + 1;
				size = 0;
				const char* str3 = strstr(str2, ";");
				if (str3)
				{
					size = strlen(str2) - strlen(str3);
					strncpy(pass, str2, size);
					pass[size] = 0;
				}
			}

		}

	}
	else
	{
		cout << "error" << endl;
	}
	if (check(name, login, pass) == true)
	{
		return true;
	}

}
string Chat::get(const char* mes)
{
	char name[2500];
	const char* str = strstr(mes, "_");
	if (str)
	{
		int size = strlen(mes) - strlen(str);
		strncpy_s(name, mes, size);
		name[size] = 0;
	}
	return name;

}
void Chat::parser_mes(const char* mes)
{
	char name[20];
	char recieved[20];
	char text[2000];
	const char* str = strstr(mes, "_");
	if (str)
	{
		int size = strlen(mes) - strlen(str);
		strncpy_s(name, mes, size);
		name[size] = 0;
		str = str + 1;
		size = 0;
		const char* str1 = strstr(str, "_");
		if (str1)
		{
			size = strlen(str) - strlen(str1);
			strncpy_s(recieved, str, size);
			recieved[size] = 0;
			str1 = str1 + 1;
			size = 0;
			const char* str2 = strstr(str1, ";");
			if (str2)
			{
				size = strlen(str1) - strlen(str2);
				strncpy_s(text, str1, size);
				text[size] = 0;
			}
		}
	}
	send(name, recieved, text);
}

bool Chat::check(string name, string login, string pass)
{
if (check_user(name) == true)
{
	if (check_entry(login) == true)
	{
		if (check_pass(login, pass) == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
else
{
	add(name, login, pass);
	return true;
}
}
//check nick
bool Chat::check_user(string name)
{
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	string m_name;
	
	mysql_init(&mysql);
	if (&mysql == nullptr) {
		
		cout << "Error: can't create MySQL-descriptor" << endl;
	}
	
	if (!mysql_real_connect(&mysql, "localhost", "root", "toor", "tabledb", NULL, NULL, 0)) {
		
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}

	mysql_set_character_set(&mysql, "utf8");
	
	mysql_query(&mysql, "SELECT * FROM Users"); 
	
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			m_name = row[1];
			if (m_name == name)
			{				
				mysql_close(&mysql);
				return true;
			}
			else
				continue;
		}
	}
	else
	{
		mysql_close(&mysql);
		return false;
	}
}

void Chat::connect(const std::string& host, const std::string& user, const std::string& pass, const std::string& name) {
	MYSQL mysql;
	mysql_init(&mysql);
	
	if (!mysql_real_connect(&mysql, "localhost", "root", "toot", "tabledb", NULL, NULL, 0)) {

		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;

	}
	else {
		
		std::cout << "Connect to database - success!" << std::endl;
	}

	std::string query = "create database if not exists tabledb ";
	int result = mysql_query(&mysql, query.c_str());
	if (result == 0) {
		std::cout << "Data base is on air!" << std::endl;
	}

	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, "localhost", "root", "toot", "tabledb", NULL, NULL, 0)) {
		
		std::cout << "Error: can't connect to database tabledb" << "  " << mysql_error(&mysql) << std::endl;

	}
	else {
		
		std::cout << "Connect to database tabledb" << " - success!" << std::endl;
	}

	createTables(); 
	mysql_set_character_set(&mysql, "utf8");
}


bool Chat::check_entry(string login)
{
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	string m_login;
	
	mysql_init(&mysql);
	if (&mysql == nullptr) {
		
		cout << "Error: can't create MySQL-descriptor" << endl;
	}	
	if (!mysql_real_connect(&mysql, "localhost", "root", "toor", "tabledb", NULL, NULL, 0)) {
		
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	mysql_set_character_set(&mysql, "utf8");
	
	mysql_query(&mysql, "SELECT * FROM Users");
	
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			m_login = row[3];
			if (m_login == login)
			{
				mysql_close(&mysql);
				return true;
			}
			else
				continue;
		}
	}
	else
	{
		mysql_close(&mysql);
		return false;
	}
}
//check pass
bool Chat::check_pass(string login, string pass)
{
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	string m_login;
	string m_pass;
	
	mysql_init(&mysql);
	if (&mysql == nullptr) {
		
		cout << "Error: can't create MySQL-descriptor" << endl;
	}
	if (!mysql_real_connect(&mysql, "localhost", "root", "toor", "testdb", NULL, NULL, 0)) {
		
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}

	mysql_set_character_set(&mysql, "utf8");
	
	mysql_query(&mysql, "SELECT * FROM Users"); 
	
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			m_login = row[3];
			if (m_login == login)
			{
				m_pass = row[4];
				if (m_pass == pass)
				{
					mysql_close(&mysql);
					return true;
				}
			}
			else
			continue;
		}
	}
	else
	{
		mysql_close(&mysql);
		return false;
	}
}

void Chat::add(string name, string login, string pass)
{
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;

	int i = 0;

	mysql_init(&mysql);
	if (&mysql == nullptr) {
	
		cout << "Error: can't create MySQL-descriptor" << endl;
	}

	if (!mysql_real_connect(&mysql, "localhost", "root", "toor", "tabledb", NULL, NULL, 0)) {
		
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	else {
		cout << "Success!" << endl;
	}

	mysql_set_character_set(&mysql, "utf8");
	int n = 0;
	mysql_query(&mysql, "SELECT * FROM Users"); 
	if (res = mysql_store_result(&mysql))
	{
		n = mysql_num_rows(res);
	}
	n = n + 1;
	cout << "Count: " << n << endl;
	string str_n = to_string(n);
	str_n = str_n + " ";
	string s = "', '";
	string s1 = "')";
	string str = "INSERT INTO users(id, name, login, pass) values('" + str_n + s + name + s + login + s + pass + s1;

	mysql_query(&mysql, str.c_str()); 
	
	mysql_close(&mysql);
}

string Chat::print()
{
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;

	int i = 0;

	mysql_init(&mysql);
	if (&mysql == nullptr) {
		
		cout << "Error: can't create MySQL-descriptor" << endl;
	}

	if (!mysql_real_connect(&mysql, "localhost", "root", "toor", "tabledb", NULL, NULL, 0)) {
		
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	string users;
	mysql_set_character_set(&mysql, "utf8");
	mysql_query(&mysql, "SELECT * FROM Users"); 
	
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			users.append(row[1]);
			users.append(" ");
		}
	}
	else
		cout << "MySql error : " << mysql_error(&mysql);

	return users;
	mysql_close(&mysql);
}
string Chat::id(string name)
{
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;
	string m_name;
	int n;
	
	if (&mysql == nullptr) {
		
		cout << "Error: can't create MySQL-descriptor" << endl;
	}

	if (!mysql_real_connect(&mysql, "localhost", "root", "toor", "tabledb", NULL, NULL, 0)) {

		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	string id;
	mysql_set_character_set(&mysql, "utf8");
	
	mysql_query(&mysql, "SELECT * FROM users"); 
	
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			m_name = row[1];
			if (m_name == name)
			{
				id = row[0];
				return id;
				
				mysql_close(&mysql);
			}
			else
			continue;
		}
	}
	else
	{
		mysql_close(&mysql);
		return "-1";
	}
}
string Chat::send(string sender, string receiver, string text)
{
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;

	mysql_init(&mysql);
	if (&mysql == nullptr) {
		
		cout << "Error: can't create MySQL-descriptor" << endl;
	}
	if (!mysql_real_connect(&mysql, "localhost", "root", "toor", "tabledb", NULL, NULL, 0)) {
		
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	else {
		cout << "Success!" << endl;
	}

	mysql_set_character_set(&mysql, "utf8");
	int n = 0;
	mysql_query(&mysql, "SELECT * FROM Message"); 
	if (res = mysql_store_result(&mysql))
	{
		n = mysql_num_rows(res);
	}
	n = n + 1;
	string str_n = to_string(n);
	str_n = str_n + " ";
	string s = "', '";
	string s1 = "')";
	string s2 = "', ";
	string idsender = id(sender);
	string idreciver = id(receiver);

	string str1 = "INSERT INTO message(idm, senderId, idreciverId, `text`) values(" + str_n + ", " + idsender + ", " + idreciver + ", '" + text + s2 + to_string(0) + ", '" + "0" + s1;
	mysql_query(&mysql, str1.c_str()); 
	
	mysql_close(&mysql);
	return text;
}
string Chat::get_name(string id)
{
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;

	string str_id;
	string user;
	
	mysql_init(&mysql);
	if (&mysql == nullptr) {
		
		cout << "Error: can't create MySQL-descriptor" << endl;
	}
	if (!mysql_real_connect(&mysql, "localhost", "root", "toor", "tabledb", NULL, NULL, 0)) {
		
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	mysql_set_character_set(&mysql, "utf8");

	mysql_query(&mysql, "SELECT * FROM Users"); 
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			str_id = row[0];
			if (str_id == id)
			{
				user = row[1];
				
				mysql_close(&mysql);
				return user;
			}
			else
			continue;
		}
	}
	else
	{
		mysql_close(&mysql);
		cout << "error" << endl;
	}
}
string Chat::receive(string name)
{
	MYSQL mysql;
	MYSQL_RES* res;
	MYSQL_ROW row;

	mysql_init(&mysql);
	if (&mysql == nullptr) {
	
		cout << "Error: can't create MySQL-descriptor" << endl;
	}
	if (!mysql_real_connect(&mysql, "localhost", "root", "12345", "sys", NULL, NULL, 0)) {
		
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}

	string s_name = id(name);
	string m_name;
	string sender;
	string text;
	string date;
	mysql_set_character_set(&mysql, "utf8");

	mysql_query(&mysql, "SELECT * FROM Message");
	if (res = mysql_store_result(&mysql)) {
		while (row = mysql_fetch_row(res)) {
			m_name = row[2];
			if (m_name == s_name)
			{
				sender = get_name(row[1]);
				text = row[3];
				date = row[4];
				// ��������� ���������� � �������� ���� ������
				mysql_close(&mysql);
				string ret = date + " " + sender + ": " + text;
				return ret;
			}
			else
				continue;
		}
	}
	else
	{
		// ��������� ���������� � �������� ���� ������
		mysql_close(&mysql);
		cout << "no Message!" << endl;
	}
}

void Chat::createTables() {
	MYSQL mysql;
	mysql_init(&mysql);
	int result{};
	//  Создаем таблицу пользователей
	result = mysql_query(&mysql, "create table if not exists Users"
		"(id int auto_increment primary key, "
		"name varchar(255), "
		"login varchar(255) not null, "
		"password varchar(255) not null)");
	if (result == 0) {
		std::cout << "Table Users is created or finded!" << std::endl;
	}
	else {
		std::cout << "Table Users is not created or not finded!" << std::endl;
	}

	//  Создаем таблицу индивидуальных сообщений
	result = mysql_query(&mysql, "create table if not exists MessagesPrivate"
		"(id int auto_increment primary key,"
		"senderId int references Users(id), "
		"recieverId int references Users(id), "
		"text varchar(1024))");
	if (result == 0) {
		std::cout << "Table MessagesPrivate is created or finded!" << std::endl;
	}
	else {
		std::cout << "Table MessagesPrivate is not created or not finded!" << std::endl;
	}
}