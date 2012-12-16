#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>
#include <iostream>
#include <Windows.h>
#include <algorithm>
#include <string>

using namespace std;

int GetInfo();

int RetVal = 0;
SOCKET sConnect;
char* IP = new char[256];
u_short PORT;
char* NAME = new char[256];
char* MESSAGE = new char[256];

int ClientThread()
{
  int ID;
	char* cID = new char[64];
	string status;
	ZeroMemory(cID, 64);

	printf("\nConnected\n\n");

	char* message = new char[256];
	ZeroMemory(message, 256);
	for(;; Sleep(10))
	{
		if(recv(sConnect, message, 256, NULL) != SOCKET_ERROR && recv(sConnect, cID, 64, NULL) != SOCKET_ERROR)
		{
			ID = atoi(cID);
			if (ID == 1)
			{
				status = "<Server:> ";

			}
			cout << status << message + 4 << endl;
		}
	}
	return 0;
}

int main()
{
	SetConsoleTitle(L"Client");
	    
	ZeroMemory(NAME, sizeof(NAME));
	ZeroMemory(MESSAGE, sizeof(MESSAGE));

	WSADATA wsaData;
	WORD DLLVERSION = MAKEWORD(2, 1);
	SOCKADDR_IN addr;

	RetVal = WSAStartup(DLLVERSION, &wsaData);
	if (RetVal != 0)
	{
		MessageBoxA(NULL, "WinSock startup failed!", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	sConnect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	GetInfo();

	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);

	RetVal = connect(sConnect, (SOCKADDR*)&addr, sizeof(addr));
	if(RetVal != 0)
	{
		MessageBoxA(NULL, "Could not connect to server!", "Error", MB_OK | MB_ICONERROR);
		main();
	}
	else
	{
		send(sConnect, NAME, 256, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) ClientThread, NULL, NULL, NULL);
	}
	for(;; Sleep(10))
	{
		cin.getline(MESSAGE, 256);
		send(sConnect, MESSAGE, 256, NULL);

	}

	return 0;
}

int GetInfo()
{
	printf("IP of Server: \n");
	cin >> IP;

	system("cls");

	printf("Server PORT: \n");
	cin >> PORT;

	system("cls");

	cin.ignore();

	printf("Give yourself a Name: \n");
	cin.getline(NAME, 256);


	system("cls");

	return 0;
}
