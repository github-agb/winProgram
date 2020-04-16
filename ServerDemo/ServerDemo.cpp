#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <iostream>
#include <set>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

typedef struct UserForClient
{
	int id;
	int gender;
	char nane[32];
}USERFORCLIENT, * PUSERFORCLIENT;

typedef struct Message
{
	int type;
	USERFORCLIENT USERFORCLIENT;
	char message_data[512];
}MESSAGE, * PMESSAGE;

//int-> userID;
set<SOCKET> user_sockets;

CRITICAL_SECTION cs;

void sendMessageToUsers(MESSAGE ms, std::set<SOCKET> user_sockets);

#define DEFAULT_BUFFER_LEN sizeof(MESSAGE)

#define MS_TYPE_CHAT_CONTENT (0)
#define MS_TYPE_UPDATE_USERLIST (1)

DWORD ThreadPro(LPVOID lpParameter)
{
	SOCKET s = *((SOCKET*)lpParameter);
	int iCurrentResult;
	int recvbuflen = DEFAULT_BUFFER_LEN;
	char recvbuf[DEFAULT_BUFFER_LEN];
	do
	{
		iCurrentResult = recv(s, recvbuf, DEFAULT_BUFFER_LEN, 0);
		if (iCurrentResult > 0)
		{
			MESSAGE ms = *(PMESSAGE)recvbuf;
			if (ms.type == MS_TYPE_CHAT_CONTENT)
			{
				printf("%s\n", ms.message_data);
				EnterCriticalSection(&cs);
				sendMessageToUsers(ms, user_sockets);
				LeaveCriticalSection(&cs);
				continue;
			}
			if (ms.type == MS_TYPE_UPDATE_USERLIST)
			{
				char temp[512];
				sprintf_s(temp, 512, "%s", ms.message_data);
				memset(ms.message_data, 0, 512);
				sprintf_s(ms.message_data, 512, "%s update", temp);
				EnterCriticalSection(&cs);
				sendMessageToUsers(ms, user_sockets);
				LeaveCriticalSection(&cs);
				continue;
			}

		}
		else if (iCurrentResult == 0)
		{
			printf("Connection closing ...\n");
			break;
		}
		else
		{
			printf("client thread erro : %d ...\n", WSAGetLastError());
			break;
		}
	} while (iCurrentResult > 0);
	EnterCriticalSection(&cs);
	user_sockets.erase(s);
	LeaveCriticalSection(&cs);
	return 0;
}

int main()
{
	InitializeCriticalSection(&cs);
	//Initializing Winsock
	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	//Creating a Socket for the Server
	struct addrinfoW* result = NULL, * ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = GetAddrInfoW(NULL, L"20000", &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	SOCKET listenSocket = INVALID_SOCKET;
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (listenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		FreeAddrInfoW(result);
		WSACleanup();
		return 1;
	}

	//Binding a Socket
	iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		FreeAddrInfoW(result);
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	FreeAddrInfoW(result);

	//Listening on a Socket
	iResult = listen(listenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	//Accepting a Connection
	SOCKET clientSocket = INVALID_SOCKET;
	SOCKADDR newuser;
	int addr_len = sizeof(newuser);
	while (clientSocket = accept(listenSocket, &newuser, &addr_len))
	{

		in_addr aa = ((SOCKADDR_IN*)&newuser)->sin_addr;
		char destIP[20];
		inet_ntop(AF_INET, (void*)&aa, destIP, 20);
		printf("the %s user is in ...\n", destIP);
		if (clientSocket != INVALID_SOCKET)
		{
			EnterCriticalSection(&cs);
			user_sockets.insert(clientSocket);
			LeaveCriticalSection(&cs);

			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadPro, (LPVOID)(&clientSocket), 0, NULL);
		}
		else
		{
			closesocket(clientSocket);
		}
	}

	closesocket(listenSocket);
	WSACleanup();

	return 0;
}

void sendMessageToUsers(MESSAGE ms, std::set<SOCKET> user_sockets)
{
	std::set<SOCKET>::iterator us_it;
	SOCKET soc = INVALID_SOCKET;
	//int res;
	char* data = (char*)&ms;
	int datalen = sizeof(MESSAGE);
	int send_state;
	for (us_it = user_sockets.begin(); us_it != user_sockets.end(); us_it++)
	{
		soc = *us_it;
		send_state = send(soc, data, datalen, 0);
		if (send_state == SOCKET_ERROR)
		{
			//user_sockets.erase(soc);
			//closesocket(soc);
			printf("send erro : %d ...", WSAGetLastError());
		}
	}
}

