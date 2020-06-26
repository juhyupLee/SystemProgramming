#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define STRING_LEN 100

int _tmain()
{
	TCHAR fileName[] = _T("scale.txt");
	HANDLE hFile;

	hFile = CreateFile(fileName, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("File Open Fault!\n"));
		return -1;
	}

	DWORD high4ByteFileSize = 0;
	DWORD low4ByteFileSize = GetFileSize(&hFile, &high4ByteFileSize);

	LARGE_INTEGER fileSize;
	GetFileSizeEx(hFile, &fileSize);
	_tprintf(_T("Total File size :%u byte\n"), fileSize.QuadPart);

	CloseHandle(hFile);
	return 0;

}