#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain(int argc, TCHAR* argv[])
{
	STARTUPINFO si1 = { 0 };
	STARTUPINFO si2 = { 0 };

	PROCESS_INFORMATION pi1;
	PROCESS_INFORMATION pi2;

	HANDLE handleArr[2];

	DWORD return_Val1;
	DWORD return_Val2;

	TCHAR command1[] = _T("PartAdder.exe 1 5");
	TCHAR command2[] = _T("PartAdder.exe 6 10");

	DWORD sum = 0;

	si1.cb = sizeof(si1);
	si2.cb = sizeof(si2);
	CreateProcess(NULL, command1, NULL, NULL, TRUE, 0, NULL, NULL, &si1, &pi1);
	CreateProcess(NULL, command2, NULL, NULL, TRUE, 0, NULL, NULL, &si2, &pi2);
	CloseHandle(pi1.hThread);
	CloseHandle(pi2.hThread);
	handleArr[0] = pi1.hProcess;
	handleArr[1] = pi2.hProcess;
	WaitForMultipleObjects(2, handleArr, TRUE, INFINITE);

	GetExitCodeProcess(pi1.hProcess, &return_Val1);
	GetExitCodeProcess(pi2.hProcess, &return_Val2);

	if (return_Val1 == -1 || return_Val2 == -1)
	{
		return -1;
	}

	sum += return_Val1;
	sum += return_Val2;

	_tprintf(_T("total : %d\n"), sum);

	CloseHandle(pi1.hProcess);
	CloseHandle(pi2.hProcess);

	return 0;

}