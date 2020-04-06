#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "model.h"

#ifndef DEFAULT_ADDR
#define DEFAULT_ADDR "127.0.0.1"
#endif // !DEFAULT_ADDR

#ifndef DEFAULT_PORT
#define DEFAULT_PORT "20000"
#endif // !DEFAULT_PORT


DWORD connect2server(SOCKET *ClientSocket,addrinfo *addr);
//DWORD ThreadPro_updateuserlist(LPVOID addrinfohints);
DWORD ThreadPro_RecvMsg(LPVOID clientSocket);
