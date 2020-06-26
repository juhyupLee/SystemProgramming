#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

void SortIntData(int* pSortArr, int num);

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hFile =
		CreateFile(
			_T("data.dat"), GENERIC_READ | GENERIC_WRITE, 0
			, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
		);

	if (hFile == NULL)
	{
		_tprintf(_T("could not open file."));
	}

	HANDLE hMapFile =
		CreateFileMapping(
			hFile, NULL, PAGE_READWRITE,
			0,
			1024*10, NULL);
	
	if (hMapFile == NULL)
	{
		_tprintf(_T("Could not create map of file\n"));
	}

	int* pWrite = (int*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	if (pWrite == NULL)
	{
		_tprintf(_T("Could not map view of file.\n"));
	}
	pWrite[0] = 1;
	pWrite[1] = 3;
	pWrite[2] = 0;
	pWrite[3] = 2;
	pWrite[4] = 4;
	pWrite[5] = 5;
	pWrite[6] =  8;
	pWrite[7] = 6;
	pWrite[8] = 7;

	SortIntData(pWrite, 9);
	for (int i = 0; i < 9; i++)
	{
		_tprintf(_T("%d\n"), pWrite[i]);
	}
	UnmapViewOfFile(pWrite);
	CloseHandle(hMapFile);

	CloseHandle(hFile);

	_tprintf(_T("End Of Process!\n"));
	return 0;
}

void SortIntData(int* pSortArr, int num)
{
	int temp;

	for (int i = 0; i < num - 1; i++)
	{
		for (int j = 1; j < num; ++j)
		{
			if (pSortArr[j - 1] > pSortArr[j])
			{
				temp = pSortArr[j - 1];
				pSortArr[j - 1] = pSortArr[j];
				pSortArr[j] = temp;
			}
		}
	}
}