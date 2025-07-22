#include <iostream>
#include <winsock2.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable:4996) 

#define SERVER "127.0.0.1"  // or "localhost" - ip address of UDP server
#define BUFLEN 512  
#define PORT 9001


int main()
{
    system("title UDP Client");

    WSADATA ws;
    printf("Initialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
    {
        cout << "Failed. Error Code: " << WSAGetLastError()<<endl;
        return 1;
    }
    cout << "Initialised.\n";

    sockaddr_in server;
    int client_socket;
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
    {
        cout << "socket() failed with error code: "<< WSAGetLastError()<< endl;
        return 2;
    }

    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.S_un.S_addr = inet_addr(SERVER);
    string name;
    cout << "name: ";
    cin >> name;
    cout << endl;
    string login;
    cout << "login: ";
    cin >> login;
    cout << endl;
    string pass;
    cout << "pass: ";
    cin >> pass;
    cout << endl;
    string use = name + "_" + login + "_" + pass;

    while (true)
    {
        char message[BUFLEN];
        if (sendto(client_socket, use.c_str(), strlen(use.c_str()), 0, (sockaddr*)&server, sizeof(sockaddr_in)) == SOCKET_ERROR)
        {
            cout << "sendto() failed with error code: "<< WSAGetLastError()<<endl;
            return 3;
        }
        char user_answer[BUFLEN] = {};
        int answer_;
        int slenmes = sizeof(sockaddr_in);
        if (answer_ = recvfrom(client_socket, user_answer, BUFLEN, 0, (sockaddr*)&server, &slenmes) == SOCKET_ERROR)
        {
            cout<< "recvfrom() failed with error code: " << WSAGetLastError()<<endl;
            exit(0);
        }
        cout << user_answer << "\n";
        char recieved[BUFLEN] = {};
        string en = "_";
        printf("Enter recieved: ");
        cin >> recieved;
        char mes[BUFLEN] = {};
        printf("Enter message: ");
        cin >> mes;
        string mesmes = name + en + recieved + en + mes + ";";
        if (sendto(client_socket, mesmes.c_str(), strlen(mesmes.c_str()), 0, (sockaddr*)&server, sizeof(sockaddr_in)) == SOCKET_ERROR)
        {
            cout<< "sendto() failed with error code: " << WSAGetLastError() <<endl;
            return 3;
        }
       
        char answer[BUFLEN] = {};

        int slen = sizeof(sockaddr_in);
        int answer_length;
        if (answer_length = recvfrom(client_socket, answer, BUFLEN, 0, (sockaddr*)&server, &slen) == SOCKET_ERROR)
        {
            cout<< "recvfrom() failed with error code: "<< WSAGetLastError()<< endl;
            exit(0);
        }

        cout << answer << "\n";
    }

    closesocket(client_socket);
    WSACleanup();
}