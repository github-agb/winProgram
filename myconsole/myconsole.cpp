#include<Windows.h>
#include<stdio.h>
#include<Tlhelp32.h>
#include <iostream>
#include<stdlib.h>
using namespace std;

#define _MAP_ TEXT("gameDll")

HANDLE hFileMapping;
LPTSTR lpBuffer;
BOOL init()
{
	
	hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 0x1000, _MAP_);
	if (hFileMapping==NULL)
	{
		printf("create filemapping failed error : %d", GetLastError());
		return FALSE;
	}
	lpBuffer = (LPTSTR)MapViewOfFile(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, BUFSIZ);
	if (lpBuffer==NULL)
	{
		printf("create filemappingview failed error : %d", GetLastError());
		return FALSE;
	}
	return TRUE;
}

DWORD GetPid(const TCHAR* pDest)
{
	HANDLE hProcessHandle;
	PROCESSENTRY32 pe32 = {0};

	hProcessHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hProcessHandle == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	pe32.dwSize = sizeof(PROCESSENTRY32);
	//const TCHAR* pDest = TEXT("game.exe");
	while (Process32Next(hProcessHandle,&pe32))
	{
		//printf("%s\n", pe32.szExeFile);
		if (wcscmp(pe32.szExeFile,pDest)==0)
		{	
			CloseHandle(hProcessHandle);
			return pe32.th32ProcessID;
			wcout << pe32.szExeFile << ":" << pe32.th32ProcessID << endl;
		}
		
	}
	return 0;

}

BOOL LoadDll(DWORD pID,const TCHAR* pName)
{
	HANDLE hDestProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);

	DWORD pLEN = sizeof(WCHAR)*wcslen(pName)+1;
	LPVOID lpStart =  VirtualAllocEx(hDestProcess, NULL, pLEN, MEM_COMMIT, PAGE_READWRITE);
	BOOL bRET = WriteProcessMemory(hDestProcess, lpStart, pName, pLEN, NULL);
	if (!bRET)
	{
		cout << "writeprocessmemory failed error : %d" << GetLastError() << endl;
		CloseHandle(hDestProcess);
		return FALSE;
	}
	HMODULE hModule = GetModuleHandle(TEXT("Kernel32.dll"));
	if (!hModule)
	{
		cout << "get kernel32 failed error :" << GetLastError() << endl;
		CloseHandle(hDestProcess);
		return FALSE;
	}
	DWORD f = (DWORD)GetProcAddress(hModule, "LoadLibraryW");
	if (!f)
	{
		cout << "get loadLibraryA failed error :" << GetLastError() << endl;
		CloseHandle(hDestProcess);
		CloseHandle(hModule);
		return FALSE;
	}
	CreateRemoteThread(hDestProcess,NULL,0, (LPTHREAD_START_ROUTINE)f,lpStart,NULL,NULL);
	CloseHandle(hDestProcess);
	CloseHandle(hModule);
	return TRUE;
}

int main()
{
	init();

	const TCHAR* pName = TEXT("game.exe");
	DWORD pid = GetPid(pName);
	wcout << pid << endl;
	TCHAR DLLNAME[] = TEXT("D:\\vs-workspace\\WGDll\\Debug\\WGDll.dll");
	TCHAR* DNAME = DLLNAME;
	BOOL fl = LoadDll(pid, DNAME);
	if (fl)
	{
		cout << "haha" << endl;
	}

	TCHAR gameCmd[] = { L'A',L'B',L'R' };
	TCHAR tempp;
	int randnum = 0;
	for (;;)
	{
		randnum = rand()%3;
		tempp = gameCmd[randnum];
		wcout << tempp << endl;
		CopyMemory(lpBuffer, &tempp,4);
		wmemcpy_s(lpBuffer, 4, &tempp, 1);
		Sleep(2000);
	}
	getchar();
	return 0;
}