#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

void ShowAttributes(DWORD attrib);

int _tmain()
{
	TCHAR fileName[] = _T("data.txt");
	_tprintf(_T("Original file attrubytes \n"));
	DWORD attrib = GetFileAttributes(fileName);

	ShowAttributes(attrib);

	attrib |= (FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN);
	SetFileAttributes(fileName,attrib);

	_tprintf(_T("Changed file attributes\n"));
	attrib = GetFileAttributes(_T("RealScaleViewer.exe"));
	ShowAttributes(attrib);

	return 0;
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