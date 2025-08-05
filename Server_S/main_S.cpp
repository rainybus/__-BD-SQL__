#include <iostream>
#include "ServerS.h"
#include "ChatS.h"


using namespace std;

int main() {
	string ip{ "127.0.0.1" };
	uint16_t port{ 8080 };
	string dbHost{ "localhost" };
	string dbUser{ "root" };
	string dbPass{ "toor" };
	string dbName{ "dbChat" };

	Chat_S chat(ip, port, dbHost, dbUser, dbPass, dbName);

	chat.run();

	return 0;
}