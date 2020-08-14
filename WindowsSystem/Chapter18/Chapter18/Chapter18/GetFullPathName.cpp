#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#define  STRING_LEN 100
int main()
{

	TCHAR fileName[] = _T("scalec.txt");
	TCHAR filePathName[STRING_LEN];
	LPTSTR filePtr;

	GetFullPathName(fileName, STRING_LEN, filePathName, &filePtr);

	_tprintf(_T("%s\n"), filePathName);
	_tprintf(_T("%s\n"), filePtr);


}