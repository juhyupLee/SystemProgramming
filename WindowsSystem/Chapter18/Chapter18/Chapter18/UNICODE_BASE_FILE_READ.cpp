#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include  <Windows.h>

#define STRING_LEN 100

using namespace std;

int _tmain()
{
	TCHAR fileName[] = _T("data.txt");
	TCHAR fileData[STRING_LEN];

	HANDLE hFile = CreateFile(
		fileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("File Open Fault\n"));
		return -1;
	}

	DWORD numOfByteRead = 0;
	ReadFile(hFile, fileData, sizeof(fileData), &numOfByteRead, NULL);

	fileData[numOfByteRead / sizeof(TCHAR)] = 0;

}