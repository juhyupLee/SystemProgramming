#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>
#include <Windows.h>
#include <TlHelp32.h>
#define STR_LEN 256
#define CMD_TOKEN_NUM 10

TCHAR ERROR_CMD[]
= _T("'%s'은(는) 실행할 수 있는 프로그램이 아닙니다. \n");


int CmdProcessing(void);
TCHAR* StrLower(TCHAR*);
void PrintProcessList();
int SearchProcessID(const TCHAR* process);


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

		TCHAR optionStr[1000] = _T("");
		for (int i = 2; i < tokenNum; ++i)
		{
			_stprintf(optionStr, _T("%s %s"), optionStr, cmdTokenList[i]);
		}

		_stprintf(processName, _T("%s %s %s"),processName,cmdTokenList[1], optionStr);
	
		BOOL state = CreateProcess(NULL, processName, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

	}
	else if (!_tcscmp(cmdTokenList[0], _T("lp")))
	{
		PrintProcessList();
	}
	else if (!_tcscmp(cmdTokenList[0], _T("kp")))
	{
		if (tokenNum ==1)
		{
			_tprintf(_T("프로세스를 입력하지 않았습니다\n"));
			return 0;
		}

		DWORD processID = SearchProcessID(cmdTokenList[1]);
		if (processID == -1)
		{
			_tprintf(_T("해당 프로세스는 없습니다\n"));
			return 0;
		}

		HANDLE psHandle; 
		
		psHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);

		if (psHandle == NULL)
		{
			_tprintf(_T("해당 핸들을 찾지 못하였습니다"));
			return 0;
		}

		TerminateProcess(psHandle, 0);

	}
	else if (!_tcscmp(cmdTokenList[0], _T("sort")))
	{
		if (tokenNum >1)
		{
			// Token 리다이렉션 기능을 사용자가 썻는지 체크
			// 결과물을 리다이렉션한 파일에 출력한다.

			if (!_tcscmp(cmdTokenList[1], _T(">")))
			{
				STARTUPINFO si = { 0 };
				si.cb = sizeof(si);
				PROCESS_INFORMATION pi;
				SECURITY_ATTRIBUTES fileSec = { 0 };
				fileSec.bInheritHandle = TRUE;
				
				TCHAR fileName[1024] = _T("");
				_tcscpy(fileName, cmdTokenList[2]);


				HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_READ,
					&fileSec, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

				si.hStdOutput = hFile;
				si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
				si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
				si.dwFlags |= STARTF_USESTDHANDLES;

				TCHAR processName[1024] = _T("CommandProject");

				 CreateProcess(NULL,cmdTokenList[0],NULL,NULL,
					TRUE, 0, NULL, NULL, &si, &pi);

				
			}
			else if(!_tcscmp(cmdTokenList[1], _T("<")))
			return 0;
		}
		else
		{
			// 리다이렉션은안하고 그냥 sort 기능만
			TCHAR buf[100][1024];
			TCHAR printBuf[102] = _T("");

			int bufCount = 0;
			for (int i = 0; i < 100; ++i)
			{
				if (_fgetts(buf[i], 1024, stdin) == NULL)
				{
					bufCount = i;
					break;
				}
			}

			for (int i = bufCount; i>=1; --i)
			{
				for (int j = 0; j+1 < i; ++j)
				{
					if (_tcscmp(buf[j], buf[j + 1]) > 0)
					{
						TCHAR tempBuf[1024];
						_tcscpy(tempBuf, buf[j]);
						_tcscpy(buf[j], buf[j + 1]);
						_tcscpy(buf[j+1], tempBuf);

					}
				}
			}

			for (int i = 0; i < bufCount; ++i)
			{
				_fputts(buf[i], stdout);
			}
	
			
			
		}
	}
	else
	{
		_tprintf(ERROR_CMD, cmdTokenList[0]);

	}

	return 0;

}

void PrintProcessList()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("CreateToolHelp32Snapshot Error!\n"));
		return;
	}
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32))
	{
		_tprintf(_T("Process32First error!\n"));
		CloseHandle(hProcessSnap);
		return ;
	}
	do
	{
		_tprintf(_T("%25s %5d\n"), pe32.szExeFile, pe32.th32ProcessID);
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

}

int SearchProcessID(const TCHAR* process)
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("CreateToolHelp32Snapshot Error!\n"));
		return -1;
	}
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32))
	{
		_tprintf(_T("Process32First error!\n"));
		CloseHandle(hProcessSnap);
		return -1;
	}
	do
	{
		if (!_tcscmp(process, pe32.szExeFile))
		{
			CloseHandle(hProcessSnap);
			return pe32.th32ProcessID;
			
		}
	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);

	// Search Fail 
	return -1;
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