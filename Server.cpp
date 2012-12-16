#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>
#include <iostream>
#include <Windows.h>
#include <algorithm>
#include <string>

using namespace std;


SOCKADDR_IN addr;
WORD DLLVERSION = MAKEWORD(2, 1);
WSADATA wsaData;

int RetVal = 0;
int addrLen = sizeof(addr);
char* NAME = new char[256];
char* MESSAGE = new char[256];

SOCKET sConnect;
SOCKET sListen;
SOCKET* Connections;

int ConCounter = 0;

int ServerThread()
{
  ZeroMemory(NAME, sizeof(NAME));
	for(;; Sleep(50))
	{
		if(sConnect = accept(sListen, (SOCKADDR*)&addr, &addrLen))
		{
			recv(sConnect, NAME, 256, NULL);
			printf("%s, has connected.\n", NAME);

		}
	}
	return 0;
}

int RecieveThread()
{
	ZeroMemory(MESSAGE, sizeof(MESSAGE));
	for (;; Sleep(50))
	{
		if(recv(sConnect, MESSAGE, 256, NULL) != SOCKET_ERROR)
		{
			printf("<%s:> %s\n", NAME, MESSAGE);
		}
	}

	return 0;
}

int main()
{
	SetConsoleTitle(L"Server");

	int ID = 1;
	char* cID = new char[64];

	RetVal = WSAStartup(DLLVERSION, &wsaData);

	Connections = (SOCKET*)calloc(64, sizeof(SOCKET));
	sConnect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	char* message = new char[256];
	ZeroMemory(message, 256);

	addr.sin_addr.s_addr = inet_addr("192.168.1.5");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1337);

	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	if (listen(sListen, 64) != SOCKET_ERROR)
	{
		printf("Listening for connections\n\n");
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) ServerThread, NULL, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) RecieveThread, NULL, NULL, NULL);
	
		itoa(ID, cID, 10);

		for(;; Sleep(10))
		{
			cin.getline(message, 256);
			if(message[0] == 's' && message[1] == 'a' && message[2] == 'y' && message[3] == ' ')
			{
				send(sConnect, message, 256, NULL);
				send(sConnect, cID, 64, NULL);
			}
		}
	}
	return 0;
}
