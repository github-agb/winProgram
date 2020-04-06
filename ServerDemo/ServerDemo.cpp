#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <iostream>
#include <map>
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
map<int, SOCKET> users;
set<SOCKET> user_sockets;
void sendMessageToUsers(MESSAGE ms, std::set<SOCKET> user_sockets);

#define DEFAULT_BUFFER_LEN sizeof(MESSAGE)

DWORD ThreadPro(LPVOID lpParameter)
{
	SOCKET s = *((SOCKET*)lpParameter);
	int iCurrentResult;
	int  iSendResult;
	int recvbuflen = DEFAULT_BUFFER_LEN;
	char recvbuf[DEFAULT_BUFFER_LEN];
	while (1)
	{
		do
		{
			iCurrentResult = recv(s, recvbuf, sizeof(MESSAGE), 0);
			if (iCurrentResult > 0)
			{
				MESSAGE ms = *(PMESSAGE)recvbuf;
				users[ms.USERFORCLIENT.id] = s;
				printf("%s\n", ms.message_data);
				//iSendResult = send(s, recvbuf, sizeof(MESSAGE), 0);
				sendMessageToUsers(ms, user_sockets);
				
				break;
			}
			else if (iCurrentResult == 0)
			{
				//printf("Connection closing ...\n");
				break;
			}
			else
			{
				break;
			}
		} while (iCurrentResult > 0);
	}
	return 0;
}

int main()
{
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
		user_sockets.insert(clientSocket);
		in_addr aa = ((SOCKADDR_IN*)&newuser)->sin_addr;
		char destIP[20];
		inet_ntop(AF_INET, (void*)&aa, destIP, 20);
		printf("the %s user is in ...\n", destIP);
		if (clientSocket != SOCKET_ERROR)
		{
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadPro, (LPVOID)(&clientSocket), 0, NULL);
		}
		else
		{
			closesocket(clientSocket);
		}
	}

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
			user_sockets.erase(soc);
			closesocket(soc);
		}
		//res = connect(soc,)
	}
}

