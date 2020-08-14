#define _WIN32_WINNT0x0400

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

VOID CALLBACK APCProc(ULONG_PTR);

int _tmain()
{
	HANDLE hThread = GetCurrentThread();
	QueueUserAPC(APCProc, hThread, (ULONG_PTR)1);
	QueueUserAPC(APCProc, hThread, (ULONG_PTR)2);
	QueueUserAPC(APCProc, hThread, (ULONG_PTR)2);
	QueueUserAPC(APCProc, hThread, (ULONG_PTR)3);
	QueueUserAPC(APCProc, hThread, (ULONG_PTR)4);
	QueueUserAPC(APCProc, hThread, (ULONG_PTR)5);

	Sleep(5000);
	SleepEx(INFINITE, TRUE);
	
	return 0;
}

void CALLBACK APCProc(ULONG_PTR dwParam)
{
	_tprintf(_T("Asumc  call num :%d"), (DWORD)dwParam);

}