#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

unsigned int WINAPI OutputThreadFunction(LPVOID lpParam);

TCHAR string[100];
HANDLE hEvent;

int _tmain()
{
	HANDLE hThread;
	DWORD dwThreadID;

	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (hEvent == NULL)
	{
		_tprintf(_T("Event Object Creation Error\n"));
		return -1;
	}

	hThread = (HANDLE)_beginthreadex(
		NULL, 0, OutputThreadFunction, NULL,
		0, (unsigned int*)&dwThreadID);
	if (hThread == 0)
	{
		_tprintf(_T("Thread Creation error\n"));
		return -1;
	}

	_fputts(_T("Insert String: "), stdout);
	_fgetts(string, 30, stdin);

	SetEvent(hEvent);
	//event 의  state를  signaled 상태로 변경

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hEvent);
	CloseHandle(hThread);

	return 0;
}

unsigned int WINAPI OutputThreadFunction(LPVOID lpParam)
{
	WaitForSingleObject(hEvent, INFINITE);

	_fputts(_T("Output string:"), stdout);
	_fputts(string, stdout);

	return 0;
}