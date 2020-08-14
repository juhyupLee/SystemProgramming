#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

TCHAR strData[] =
_T("i am a boy\n")
_T("you are a girl\n")
_T("i am student\n");
void WINAPI FileIOCompletionRoutine(DWORD errorCode, DWORD numOfBytesTrannsfered, LPOVERLAPPED overlapped);
int _tmain()
{

	TCHAR fileName[] = _T("data.txt");
	HANDLE hFile = CreateFile(
		fileName, GENERIC_WRITE, FILE_SHARE_WRITE, 0,
		CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, 0);


	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("File Creation fault!\n"));
		return -1;
	}
	
	OVERLAPPED overlappedInst;
	memset(&overlappedInst, 0, sizeof(overlappedInst));
	overlappedInst.hEvent = (HANDLE)1234;
	WriteFileEx(hFile, strData, sizeof(strData), &overlappedInst, FileIOCompletionRoutine);
	SleepEx(INFINITE, TRUE);

	CloseHandle(hFile);

	return 1;

}

void WINAPI FileIOCompletionRoutine(DWORD errorCode, DWORD numOfBytesTrannsfered, LPOVERLAPPED overlapped)
{
	//tprintf(_T("******File write result******\n"));
	//_tprintf(_T("Error Code :%u\n"), errorCode);
	_tprintf(_T("dfdf %u"), numOfBytesTrannsfered);
	//_tprintf(_T("The other info : %u\n"), (DWORD)overlapped->hEvent);

}