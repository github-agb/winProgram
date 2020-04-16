#include "network.h"


DWORD connect2server(SOCKET* ClientSocket, addrinfo* addr)
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
	CListCtrl* PUser_List_Ctrl = (CListCtrl*)(PChatroomDlg->GetDlgItem(IDC_LIST_USERS));
	PUser_List_Ctrl->InsertColumn(0, L"user_list", 0, 100);
	CString show;
	SOCKET soc = INVALID_SOCKET;
	soc = *((SOCKET*)clientSocket);
	char recvbuf[DEFAULT_BUFFER_LEN];
	int recvlen = 0;;
	do
	{
		recvlen = recv(soc, recvbuf, DEFAULT_BUFFER_LEN, 0);
		if (recvlen > 0)
		{
			MESSAGE ms = *(PMESSAGE)recvbuf;
			if (ms.type == MS_TYPE_CHAT_CONTENT)
			{
				PChatroomDlg->GetDlgItem(IDC_EDIT_SHOWCHATCONTENT)->GetWindowTextW(show);
				show = show + CHAR2CString(ms.user.name) + CHAR2CString("\r\n\t") + CHAR2CString(ms.message_data) + CHAR2CString("\r\n");

				PChatroomDlg->GetDlgItem(IDC_EDIT_SHOWCHATCONTENT)->SetWindowTextW(show);
				continue;
			}

			//更新客户端显示在线用户列表，放在另一个线程
			if (ms.type == MS_TYPE_UPDATE_USERLIST)
			{
				//PUser_List_Ctrl->InsertColumn(0, L"user_list",0,100);
				CString cont = CHAR2CString(ms.message_data);
				PUser_List_Ctrl->InsertItem(0, cont);
				continue;
			}

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
	} while (recvlen > 0 && thread_signal == 1);
	return 0;
}


//更新用户列表线程，这里只负责发送更新请求，接收都放在 ThreadPro_RecvMsg 线程
DWORD ThreadPro_updateuserlist(LPVOID clientSocket)
{
	MESSAGE ms;
	ms.user = currentuser;
	ms.type = MS_TYPE_UPDATE_USERLIST;
	SOCKET soc = INVALID_SOCKET;
	soc = *((SOCKET*)clientSocket);
	int res;
	int update_counts = 0;
	while (thread_signal ==1)
	{
		sprintf_s(ms.message_data,DEFAULT_BUFFER_LEN, "%d", update_counts);
		EnterCriticalSection(&cs);
		res = send(soc, (char*)&ms, sizeof(ms), 0);
		LeaveCriticalSection(&cs);
		if (res == SOCKET_ERROR)
		{
			closesocket(soc);
			return 0;
		}
		update_counts++;
		Sleep(2000);
	}
	return 0;
}


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