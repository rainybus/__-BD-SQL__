#include <iostream>
#include "ClientC.h"
#include "ChatC.h"

int main() {

	Sleep(3000);

	string ip{ "127.0.0.1" };
	uint16_t port{ 8080 };

	Chat_C chat(ip, port);
	chat.run();

	return 0;
}