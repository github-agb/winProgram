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
	CWnd* PChatroomDlg = ::AfxGetApp()->GetMainWnd();
	CString show;
	SOCKET soc = INVALID_SOCKET;
	soc = *((SOCKET*)clientSocket);
	char recvbuf[DEFAULT_BUFFER_LEN];
	int recvlen = 0;;
	while (1)
	{
		do
		{
			recvlen = recv(soc, recvbuf, DEFAULT_BUFFER_LEN, 0);
			if (recvlen > 0)
			{
				MESSAGE ms = *(PMESSAGE)recvbuf;
				if (ms.type == MS_TYPE_CHAT_CONTENT)
				{
					PChatroomDlg->GetDlgItem(IDC_EDIT_SHOWCHATCONTENT)->GetWindowTextW(show);
					show = show + CHAR2CString(ms.user.name) + CHAR2CString("\r\n") + CHAR2CString(ms.message_data) + CHAR2CString("\r\n");

					PChatroomDlg->GetDlgItem(IDC_EDIT_SHOWCHATCONTENT)->SetWindowTextW(show);
					break;
				}
				if (ms.type == MS_TYPE_UPDATE_USERLIST)
				{

				}

				break;
			}
			else if (recvlen == 0)
			{
				//printf("Connection closing ...\n");
				break;
			}
			else
			{
				break;
			}
		} while (recvlen>0);
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
char* CString2CHAR(CString cstr)
{
	LPWSTR temp = cstr.AllocSysString();
	if (temp == NULL)
	{
		return NULL;
	}
	CHAR* res = "";
	int needBytes = WideCharToMultiByte(CP_ACP, 0, temp, -1, NULL, 0, NULL, NULL);
	if (needBytes > 0)
	{
		res = new char[needBytes + 1];
		ZeroMemory(res, needBytes + 1);
		WideCharToMultiByte(CP_ACP, 0, temp, -1, res, needBytes, NULL, NULL);
	}
	return res;
}

CString CHAR2CString(CHAR* ch)
{
	CString res(ch);
	return res;
	/*
	int sBufferSize = strlen(ch);
	DWORD dBufferSize = MultiByteToWideChar(CP_ACP, 0, ch, sBufferSize, NULL, 0);
	WCHAR* temp = new WCHAR[dBufferSize];
	wmemset(temp, 0, dBufferSize);
	int nRet = MultiByteToWideChar(CP_ACP, 0, ch, sBufferSize, temp, dBufferSize);
	if (nRet > 0)
	{
		res.SetString(temp);
		return res;
	}
	else
	{
		res.Empty();
		return res;
	}
	*/
}