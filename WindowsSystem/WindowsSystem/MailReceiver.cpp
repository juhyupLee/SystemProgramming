#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define SLOT_NAME _T("\\\\.\\mailslot\\mailbox")

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hMailSlot;
	TCHAR messageBox[50];
	DWORD bytesRead; //number of bytes read

	// Mail Slot 생성//
	hMailSlot = CreateMailslot(SLOT_NAME, 50, MAILSLOT_WAIT_FOREVER, NULL);

	if (hMailSlot == INVALID_HANDLE_VALUE)
	{
		_fputts(_T("Unable to Create mailslot\n"), stdout);
		return 1;
	}

	//Message 수신//
	_fputts(_T("******Message***********\n"), stdout);

	while (1)
	{
		//ReadFile 블로킹 함수인가?
		if (!ReadFile(hMailSlot, messageBox, sizeof(TCHAR) * 50, &bytesRead, NULL))
		{
			_fputts(_T("Unable to Read!"), stdout);
			CloseHandle(hMailSlot);
			return 1;
		}
		if (!_tcsncmp(messageBox, _T("exit"), 4))
		{
			_fputts(_T("Good Bye!"), stdout);
			break;
		}
		messageBox[bytesRead / sizeof(TCHAR)] = 0;
		_fputts(messageBox, stdout);

	}
	CloseHandle(hMailSlot);

	return 0;
}