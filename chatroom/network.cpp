#include "network.h"

DWORD connect2server(SOCKET *ClientSocket,addrinfo *addr)
{
	*ClientSocket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (*ClientSocket == INVALID_SOCKET)
	{
		freeaddrinfo(addr);
		WSACleanup();
		return 1;
	}
	int iResult = connect(*ClientSocket, addr->ai_addr, addr->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		freeaddrinfo(addr);
		WSACleanup();
		return 1;
	}
	return 0;
}

DWORD ThreadPro_RecvMsg(LPVOID clientSocket)
{
	SOCKET soc = INVALID_SOCKET;
	soc = *((SOCKET*)clientSocket);
	char recv[DEFAULT_BUFFER_LEN];
	int recvlen;
	while (1)
	{
		do
		{
			recvlen = recv();
		} while (recvlen > 0);
	}
	return 0;
}

/*
DWORD ThreadPro_updateuserlist(LPVOID addrinfohints)
{
	addrinfo *ad = (addrinfo*)addrinfohints;
	SOCKET ClientSocket = INVALID_SOCKET;
	DWORD iState = connect2server(&ClientSocket, ad);
	if (iState != 0)
	{
		closesocket(ClientSocket);
		return 1;
	}
	char* updatestatement = "update_user_list";
	int res = send(ClientSocket, updatestatement, strlen(updatestatement), 0);
	if (res == SOCKET_ERROR)
	{
		closesocket(ClientSocket);
		return 1;
	}
	res = shutdown(ClientSocket, SD_SEND);
	if (res == SOCKET_ERROR)
	{
		closesocket(ClientSocket);
		return TRUE;
	}
	
	do
	{
		res = recv(ClientSocket,)
	} while (res>0);
	return 0;
}
*/