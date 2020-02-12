// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include<Windows.h>
#include<iostream>
#include<stdio.h>
using namespace std;

#define _MAP_ TEXT("gameDll")

#define ATTACK 0x0641740
#define REST 0x0641800
#define BLOOD 0x06417a0

HANDLE hMapFile;
LPTSTR lpBuffer;
TCHAR dwType;

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
    HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, _MAP_);
    if (!hMapFile)
    {
        printf("OpenMappingFile Error : %d", GetLastError());
        return 0;
    }
    lpBuffer = (LPTSTR)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUFSIZ);
    for (;;)
    {
        Sleep(2000);
        if (lpBuffer != NULL)
        {
           // CopyMemory(&dwType, lpBuffer, 4);
            wmemcpy_s(&dwType, 4, lpBuffer, 1);
            wcout << lpBuffer << endl;
        }
        if (dwType == L'A')
        {
            //MessageBox(NULL, TEXT("AAAAA"), TEXT("AAAAA"), MB_OK);
            __asm
            {
                mov eax, ATTACK
                call eax
            }
            //dwType = 0;
            //CopyMemory(lpBuffer, &dwType, 4);
        }
        if (dwType == L'B')
        {
            //MessageBox(NULL, TEXT("BBBBBB"), TEXT("BBBBBBB"), MB_OK);
            __asm
            {
                mov eax, BLOOD
                call eax
            }
            //dwType = 0;
            //CopyMemory(lpBuffer, &dwType, 4);
        }
        if (dwType == L'R')
        {
            //MessageBox(NULL, TEXT("RRRRRRR"), TEXT("RRRRRRR"), MB_OK);
            __asm
            {
                mov eax, REST
                call eax
            }
            //dwType = 0;
            //CopyMemory(lpBuffer, &dwType, 4);
        }
        if (dwType == L'Q')
        {
            //MessageBox(NULL, TEXT("QQQQQQQ"), TEXT("QQQQQQ"), MB_OK);
            UnmapViewOfFile(lpBuffer);
        }
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        MessageBox(NULL, TEXT("hehe"), TEXT("HAHA"), MB_OKCANCEL);
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProc, NULL, 0, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

