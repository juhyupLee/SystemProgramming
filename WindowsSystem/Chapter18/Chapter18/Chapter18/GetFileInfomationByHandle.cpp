#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define STRING_LEN 100

void ShowAttributes(DWORD attrib);
void ShowFileTime(FILETIME t);

int _tmain()
{
	TCHAR fileName[] = _T("RealScaleViewer.exe");
	BY_HANDLE_FILE_INFORMATION fileInfo;
	
	HANDLE hFile = CreateFile(
		fileName, GENERIC_READ, 0, 0,
		CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("File Open Fault!\n"));
		return -1;
	}

	GetFileInformationByHandle(hFile, &fileInfo);
	
	ShowAttributes(fileInfo.dwFileAttributes);

	_tprintf(_T("File Size :%u\n"), fileInfo.nFileSizeLow);

	_tprintf(_T("File created time :"));
	ShowFileTime(fileInfo.ftCreationTime);

	_tprintf(_T("File accessed time :"));
	ShowFileTime(fileInfo.ftLastAccessTime);

	_tprintf(_T("File written time :"));
	ShowFileTime(fileInfo.ftLastWriteTime);

	CloseHandle(hFile);
	return 0;

}
void ShowFileTime(FILETIME t)
{
	TCHAR fileTimeInfo[STRING_LEN];
	FILETIME ft = t;
	SYSTEMTIME stUTC, stLocal;

	FileTimeToSystemTime(&ft, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	_stprintf_s(
		fileTimeInfo, _T("%02d/%02d/%d %02d:%02d"), stLocal.wMonth, stLocal.wDay, stLocal.wYear,
		stLocal.wHour, stLocal.wMinute);

	_tprintf(_T("[%s]"), fileTimeInfo);
}
void ShowAttributes(DWORD attrib)
{
	if (attrib & FILE_ATTRIBUTE_NORMAL)
	{
		_tprintf(_T("Normal"));
	}
	else
	{
		if (attrib & FILE_ATTRIBUTE_READONLY)
		{
			_tprintf(_T("Read only\n"));
		}
		if (attrib & FILE_ATTRIBUTE_HIDDEN)
		{
			_tprintf(_T("Hidden \n"));
		}
	}
}