#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>
#include <Windows.h>

#define STR_LEN 256
#define CMD_TOKEN_NUM 10

TCHAR ERROR_CMD[]
= _T("'%s'은(는) 실행할 수 있는 프로그램이 아닙니다. \n");


int CmdProcessing(void);
TCHAR* StrLower(TCHAR*);

int _tmain(int argc, TCHAR* argv[])
{
	//한글 입력을 가능케 하기 위해
	_tsetlocale(LC_ALL, _T("Korean"));
	DWORD isExit;

	while (1)
	{
		isExit = CmdProcessing();
		if (isExit == TRUE)
		{
			_fputts(_T("명령어 처리를 종료합니다. \n"), stdout);
			break;
		}
	}

	return 0;
}
TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" ,\t\n");


int CmdProcessing(void)
{
	_fputts(_T("Best command prompt>> "), stdout);
	_getts_s(cmdString);
	TCHAR* token = _tcstok(cmdString, seps);
	int tokenNum = 0;
	while (token != NULL)
	{
		_tcscpy(cmdTokenList[tokenNum++], StrLower(token));
		token = _tcstok(NULL, seps);
	}
	if (!_tcscmp(cmdTokenList[0], _T("exit")))
	{
		return true;
	}
	else if (!_tcscmp(cmdTokenList[0], _T("notepad.exe")))
	{
		STARTUPINFO si = { 0, };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(si);

		TCHAR processName[] = _T("notepad.exe");
		BOOL state =CreateProcess(NULL, processName, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi);
		if (!state)
		{
			_fputts(_T("Fail\n"), stdout);
		}
		else
		{
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

		}
	}
	else if (!_tcscmp(cmdTokenList[0], _T("start")))
	{
		//if (cmdTokenList[1] != NULL)
		//{
		//	TCHAR temp[]
		//}
		STARTUPINFO si = { 0, };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(si);
		TCHAR processName[1000]=_T("cmd.exe /k");
		_tcscat(processName, _T(" "));
		_tcscat(processName, cmdTokenList[1]);
		_tcscat(processName, _T(" "));
		TCHAR optionStr[1000] = _T("");
		for (int i = 2; i < tokenNum; ++i)
		{
			_tcscat(optionStr, cmdTokenList[i]);
			_tcscat(optionStr, _T(" "));
		}
		_tcscat(processName, optionStr);
	
		BOOL state = CreateProcess(NULL, processName, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	}
	else
	{
		_tprintf(ERROR_CMD, cmdTokenList[0]);

	}

	return 0;

}

TCHAR* StrLower(TCHAR* pStr)
{
	TCHAR* ret = pStr;

	while (*pStr)
	{
		if (_istupper(*pStr))
		{
			*pStr = _totlower(*pStr);
		}
		pStr++;
	}
	return ret;
}