#define _WIN32_WINNT 0x0400

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain()
{
	
	HANDLE hTimer = NULL;
	long long liDueTime = -50000000;

	hTimer =
		CreateWaitableTimer(
			NULL, FALSE, NULL);
	if (!hTimer)
	{
		_tprintf(_T("CreateWaitableTimer failed (%d)\n", GetLastError()));

		return 1;
	}

	_tprintf(_T("Waiting for 10  seconds...\n"));

	SetWaitableTimer(hTimer, (LARGE_INTEGER*)&liDueTime, 0, NULL, NULL, FALSE);


	WaitForSingleObject(hTimer, INFINITE);
	_tprintf(_T("Timer was signaled\n"));
	MessageBeep(MB_ICONEXCLAMATION);

	return 0;
}