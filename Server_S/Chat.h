#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <map>
#include <mysql.h>


using namespace std;
class Chat
{
public:
	Chat();
	
	bool parser(const char* mes);
	void createTables();
	void connect(const std::string& host, const std::string& user, const std::string& pass, const std::string& name);
	
	bool check(string name, string login, string pass);
	bool check_user(string name);
	bool check_entry(string login);
	bool check_pass(string login, string pass);

	void add(string name, string login, string pass);

	string print();
	
	string id(string name);
	
	string get_name(string id);
	
	string get(const char* mes);

	string send(string sender, string receiver, string text);
	
	string receive(string name);

	void parser_mes(const char* mes);
};