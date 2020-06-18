#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{

	HANDLE hProcess;
	TCHAR cmdString[1024];
	DuplicateHandle(GetCurrentProcess(), GetCurrentProcess(), GetCurrentProcess(), &hProcess
		, 0, TRUE, DUPLICATE_SAME_ACCESS);

	//복사를 하려고하는(대상)의 프로세스의 핸들중
	// 현재프로세스의 핸들을 
	// 현재 프로
	_stprintf_s(cmdString, _T("%s %u"), _T("ChildProcess.exe"), (unsigned)hProcess);


//부모프로세스에 존재하는
	// 부모프로세스의 핸들을
	// 부모프로세스 핸들테이블에 등록한다
	/// 등록된 핸들의 값은 변수 val에 저장한다.

	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi = { 0, };
	si.cb = sizeof(si);

	BOOL isSuccess = CreateProcess(NULL, cmdString, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

	if (isSuccess== FALSE)
	{
		_tprintf(_T("CreateProcess failed\n  "));
		return -1;
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	_tprintf(_T("[Parent Process]\n"));
	_tprintf(_T("oooooooooooooooooooops\n"));

	return 0;
}