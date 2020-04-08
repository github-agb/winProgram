#pragma once
#ifndef __NETWORK
#define __NETWORK
#define WIN32_LEAN_AND_MEAN

#include <afxwin.h>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "model.h"
#include <stdlib.h>
#include "Resource.h"

#ifndef DEFAULT_ADDR
#define DEFAULT_ADDR "127.0.0.1"
#endif // !DEFAULT_ADDR

#ifndef DEFAULT_PORT
#define DEFAULT_PORT "20000"
#endif // !DEFAULT_PORT

DWORD connect2server(SOCKET *ClientSocket,addrinfo *addr);
//DWORD ThreadPro_updateuserlist(LPVOID addrinfohints);
//DWORD ThreadPro_RecvMsg(LPVOID clientSocket);
char* CString2CHAR(CString cstring);

CString CHAR2CString(CHAR* ch);
#endif // !__NETWORK