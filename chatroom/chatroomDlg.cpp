﻿
// chatroomDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "chatroom.h"
#include "chatroomDlg.h"
#include "afxdialogex.h"
#include "network.h"
#include "model.h"
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//自定义成员
SOCKET ClientSocket = INVALID_SOCKET;

int recvbuflen = DEFAULT_BUFFER_LEN;

struct addrinfo *addr = NULL;

char recvbuf[DEFAULT_BUFFER_LEN];

USER currentuser;

// CchatroomDlg 对话框
CchatroomDlg::CchatroomDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATROOM_DIALOG, pParent)
{
	srand((int)time(NULL));
	currentuser.id = rand();
	currentuser.gender = 0;
	_itoa_s(currentuser.id, currentuser.name, 10);
	//sprintf_s(currentuser.name, "%s", currentuser.id);
	//strcpy_s(currentuser.name, "xiaoming");
	//currentuser.name = "haha";
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	
}

void CchatroomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_input_edit);
	DDX_Control(pDX, IDC_EDIT_SHOWCHATCONTENT, m_showchatcontent_edit);
}

BEGIN_MESSAGE_MAP(CchatroomDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CchatroomDlg 消息处理程序

BOOL CchatroomDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_EDIT_INPUT)->SetFocus();
	WSADATA wsadata;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (iResult!=0)
	{
		return TRUE;
	}
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	iResult = getaddrinfo(DEFAULT_ADDR, DEFAULT_PORT, &hints, &addr);
	if (iResult!=0)
	{
		WSACleanup();
		return TRUE;
	}

	DWORD iState = connect2server(&ClientSocket, addr);
	if (iState != 0)
	{
		closesocket(ClientSocket);
		return TRUE;
	}

	//update userlist
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadPro_updateuserlist, (LPVOID)addr, 0, NULL);
	return FALSE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CchatroomDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CchatroomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/**/
BOOL CchatroomDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_RETURN)
	{
		if (GetFocus()==GetDlgItem(IDC_EDIT_INPUT))
		{
			//SOCKET ClientSocket = INVALID_SOCKET;
			
			CString inputcontent;
			GetDlgItem(IDC_EDIT_INPUT)->GetWindowText(inputcontent);
			USES_CONVERSION;
			char* sendData = T2A(inputcontent);
			
			MESSAGE ms;
			ms.user = currentuser;
			strcpy_s(ms.message_data, sendData);
			//ms.message_data = sendData;
			int res = send(ClientSocket, (char*)&ms, sizeof(ms), 0);
			if (res == SOCKET_ERROR)
			{
				closesocket(ClientSocket);
				return TRUE;
			}
			
			GetDlgItem(IDC_EDIT_INPUT)->SetWindowTextW(L"");
			do
			{
				res = recv(ClientSocket, recvbuf, sizeof(MESSAGE), 0);
				if (res>0)
				{
					MESSAGE mss = *(PMESSAGE)recvbuf;
					USES_CONVERSION;
					CString show;
					GetDlgItem(IDC_EDIT_SHOWCHATCONTENT)->GetWindowTextW(show);
					show = show + A2W(mss.user.name) + A2W("\r\n")+A2W(mss.message_data)+A2W("\r\n");
					
					((CEdit*)GetDlgItem(IDC_EDIT_SHOWCHATCONTENT))->SetWindowTextW(show);
					break;
				}
				else
				{
					if (res ==0)
					{
						break;
					}
					else
					{
						printf("recv error ...");
						break;
					}
				}
			} while (res>0);
		}
		return TRUE;
	}
	if (pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
