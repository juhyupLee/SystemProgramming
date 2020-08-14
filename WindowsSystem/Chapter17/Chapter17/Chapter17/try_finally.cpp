
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

int _tmain()
{
	TCHAR str[1];

	__try
	{
		_tcscpy_s(str,_countof(str), _T("asdfdsfsdfsdf"));
	}
	__finally
	{
		_tprintf(_T("__finally block\n"));
	}
	
	_tprintf(_T(" end line "));

	return 0;
}