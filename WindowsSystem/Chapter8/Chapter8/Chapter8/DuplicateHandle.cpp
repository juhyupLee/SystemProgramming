#include <stdio.h>
#include <tchar.h>
#include <windows.h>

int _tmain(int argc, TCHAR* argv[])
{

	HANDLE hProcess;
	TCHAR cmdString[1024];
	DuplicateHandle(GetCurrentProcess(), GetCurrentProcess(), GetCurrentProcess(), &hProcess
		, 0, TRUE, DUPLICATE_SAME_ACCESS);

	//���縦 �Ϸ����ϴ�(���)�� ���μ����� �ڵ���
	// �������μ����� �ڵ��� 
	// ���� ����
	_stprintf_s(cmdString, _T("%s %u"), _T("ChildProcess.exe"), (unsigned)hProcess);


//�θ����μ����� �����ϴ�
	// �θ����μ����� �ڵ���
	// �θ����μ��� �ڵ����̺� ����Ѵ�
	/// ��ϵ� �ڵ��� ���� ���� val�� �����Ѵ�.

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