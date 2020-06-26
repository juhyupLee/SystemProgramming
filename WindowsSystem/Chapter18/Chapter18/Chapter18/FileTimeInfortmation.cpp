#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define STRING_LEN 100

int _tmain()
{
	TCHAR fileName[] = _T("scale.txt");

	TCHAR fileCreateTimeInfo[STRING_LEN];
	TCHAR fileAccessTimeInfo[STRING_LEN];
	TCHAR fileWriteTimeInfo[STRING_LEN];

	FILETIME ftCreate, ftAccess, ftWrite;

	SYSTEMTIME stCreateUTC, stCreateLocal;
	SYSTEMTIME stAccessUTC, stAccessLocal;
	SYSTEMTIME stWriteUTC, stWriteLocal;

	HANDLE hFile = CreateFile(
		fileName, GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("File open fault\n"));
		return -1;
	}

	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
	{
		_tprintf(_T("GetFileTime Function Call fault\n"));
		return FALSE;
	}
	
	FileTimeToSystemTime(&ftCreate, &stCreateUTC);
	SystemTimeToTzSpecificLocalTime(
		NULL, &stCreateUTC, &stCreateLocal);

	FileTimeToSystemTime(&ftAccess, &stAccessUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stAccessUTC, &stAccessLocal);

	FileTimeToSystemTime(&ftWrite, &stWriteUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stWriteUTC, &stWriteLocal);

	_stprintf_s(fileCreateTimeInfo, _T("%02d / %02d / %d %02d:%02d\n"), stCreateLocal.wMonth, stCreateLocal.wDay, stCreateLocal.wYear, stCreateLocal.wHour, stCreateLocal.wMinute);

	_fputts(fileCreateTimeInfo, stdout);

}