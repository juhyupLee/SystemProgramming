//#define _WIN32_WINNT0x0400

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain()
{
	HANDLE hTimer = NULL;
	long long liDueTime=-100000000;

	hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
	if (!hTimer)
	{
		_tprintf(_T("CreateWaitableTimer failed (%d)\n", GetLastError()));

		return 1;
	}

	_tprintf(_T("Waiting for 10 seconds...\n"));
	
	SetWaitableTimer(hTimer, (LARGE_INTEGER*)&liDueTime, 1000,
		NULL, NULL, FALSE);

	while (1)
	{
		WaitForSingleObject(hTimer, INFINITE);
		_tprintf(_T("Timer was signaled\n"));
		MessageBeep(MB_ICONEXCLAMATION);
	}

	return 0;
}