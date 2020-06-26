#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

unsigned int WINAPI OutputThreadFunction(LPVOID lpParam);
unsigned int WINAPI CountThreadFunction(LPVOID lpParma);


TCHAR string[100];
HANDLE hEvent;
HANDLE hMutex;

int _tmain()
{
	HANDLE hThread[2];
	DWORD dwThreadID[2];

	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//hMutex = CreateMutex(NULL, FALSE, NULL);

	if (hEvent == NULL)
	{
		_tprintf(_T("Event Object Creation Error\n"));
		return -1;
	}

	hThread[0] = (HANDLE)_beginthreadex(
		NULL, 0, OutputThreadFunction, NULL,
		0, (unsigned int*)&dwThreadID[0]);
	if (hThread[0] == 0)
	{
		_tprintf(_T("Thread Creation error\n"));
		return -1;
	}

	hThread[1] = (HANDLE)_beginthreadex(
		NULL, 0, CountThreadFunction, NULL,
		0, (unsigned int*)&dwThreadID[1]);

	if (hThread[1] == 0)
	{
		_tprintf(_T("Thread Creation error\n"));
		return -1;
	}
	_fputts(_T("Insert String: "), stdout);
	_fgetts(string, 100, stdin);

	SetEvent(hEvent);
	//event 의  state를  signaled 상태로 변경

	WaitForMultipleObjects(2, hThread,TRUE,INFINITE);
	CloseHandle(hEvent);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	return 0;
}

unsigned int WINAPI OutputThreadFunction(LPVOID lpParam)
{

	WaitForSingleObject(hEvent, INFINITE);
	WaitForSingleObject(hMutex, INFINITE);
	_fputts(_T("Output string:"), stdout);
	_fputts(string, stdout);
	ReleaseMutex(hMutex);
	return 0;
}

unsigned int WINAPI CountThreadFunction(LPVOID lpParam)
{
	static bool binit = true;
	if (binit)
	{
		hMutex = CreateMutex(NULL, TRUE, NULL);
		binit = false;
	}
	WaitForSingleObject(hEvent, INFINITE);
	WaitForSingleObject(hMutex, INFINITE);
	_tprintf(_T("output string length: %d\n"), _tcslen(string) - 1);
	ReleaseMutex(hMutex);
	return 0;
}