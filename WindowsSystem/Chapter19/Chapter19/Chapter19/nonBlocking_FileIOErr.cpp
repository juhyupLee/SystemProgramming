#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

TCHAR strData1[] = _T("Nobody was farther off base than~ \n");
TCHAR strData2[] = _T("Royal Liverpool was outdated and not worthy of ~\n");
TCHAR strData3[] = _T("for the first time since 1967. The Hoylake tack ~\n");


OVERLAPPED overlappedFilePtr; 
VOID WINAPI FileIOCompletionRoutine(DWORD, DWORD, LPOVERLAPPED);
int _tmain()
{

	TCHAR fileName[] = _T("data.txt");
	HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, 0);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("File Creation fault ! \n"));
		return -1;
	}

	OVERLAPPED overlappedInstOne;
	memset(&overlappedInstOne, 0, sizeof(overlappedInstOne));
	overlappedInstOne.hEvent = (HANDLE)"First I/O";
	WriteFileEx(hFile, strData1, sizeof(strData1), &overlappedInstOne, FileIOCompletionRoutine);
	overlappedFilePtr.Offset += _tcslen(strData1) * sizeof(TCHAR);


	OVERLAPPED overlappedInstTwo;
	memset(&overlappedInstTwo, 0, sizeof(overlappedInstOne));
	overlappedInstTwo.Offset = overlappedFilePtr.Offset;

	overlappedInstTwo.hEvent = (HANDLE)"Second I/O";
	WriteFileEx(hFile, strData2, sizeof(strData2), &overlappedInstTwo, FileIOCompletionRoutine);
	overlappedFilePtr.Offset += _tcslen(strData2) * sizeof(TCHAR);


	OVERLAPPED overlappedInstThree;
	memset(&overlappedInstThree, 0, sizeof(overlappedInstOne));
	overlappedInstThree.Offset = overlappedFilePtr.Offset;
	overlappedInstThree.hEvent = (HANDLE)"Third I/O";

	WriteFileEx(hFile, strData3, sizeof(strData3), &overlappedInstThree, FileIOCompletionRoutine);
	overlappedFilePtr.Offset += _tcslen(strData3) * sizeof(TCHAR);

	SleepEx(INFINITE, TRUE);
	CloseHandle(hFile);

	return 1;
}

VOID WINAPI FileIOCompletionRoutine(DWORD errorCode, DWORD numOfBytesTransfered, LPOVERLAPPED overlapped)
{
	_tprintf(_T("******File Write Result*******\n"));
	_tprintf(_T("Error Code :%u"), errorCode);
	_tprintf(_T("Transfereed bytes len :%u\n"), numOfBytesTransfered);
	_tprintf(_T("Ther other info : %s\n"), (TCHAR*)overlapped->hEvent);
}