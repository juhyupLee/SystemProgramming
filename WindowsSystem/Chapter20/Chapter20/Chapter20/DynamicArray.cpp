#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define MAX_PAGE 10

int* nextPageAddr;
DWORD pageCnt = 0;
DWORD pageSize;
int PageFaultExceptionFilter(DWORD);

int _tmain(int argc, TCHAR* argv[])
{
	LPVOID baseAddr;
	int* lpPtr;
	SYSTEM_INFO sSysInfo;

	GetSystemInfo(&sSysInfo);
	pageSize = sSysInfo.dwPageSize; // 페이지 사이즈획득

	// MAX_PAGE의 개수만큼 페이지 RESERVE
	baseAddr = VirtualAlloc(
		NULL, //임의주소 할당
		MAX_PAGE * pageSize,
		MEM_RESERVE,
		PAGE_NOACCESS);

	if (baseAddr == NULL)
	{
		_tprintf(_T("VirtualAlloc reserve failed"));
	}
	
	lpPtr = (int*)baseAddr;
	nextPageAddr = (int*)baseAddr;

	for (int i = 0; i < (MAX_PAGE * pageSize) / sizeof(int); ++i)
	{
		__try
		{
			lpPtr[i] = i;
		}
		__except (PageFaultExceptionFilter(GetExceptionCode()))
		{
			ExitProcess(GetLastError());
			//예외처리 문제 발생시 종료
		}
	}

	for (int i = 0; i < (MAX_PAGE * pageSize) / sizeof(int); ++i)
	{
		_tprintf(_T("Array %d\n"), lpPtr[i]);
	}
	BOOL isSuccess = VirtualFree(
		baseAddr,
		0,
		MEM_RELEASE); // Free 상태로 변경

	if (isSuccess)
	{
		_tprintf(_T("Release succeeded!"));
	}
	else
	{
		_tprintf(_T("Release failed!"));
	}
}

int PageFaultExceptionFilter(DWORD exptCode)
{
	//예외의 원인이 page fault가 아니라면
	if (exptCode != EXCEPTION_ACCESS_VIOLATION)
	{
		_tprintf(_T("Exception code = %d\n"), exptCode);
		return EXCEPTION_EXECUTE_HANDLER;
	}
	_tprintf(_T("Exception is a page fault\n"));
	if (pageCnt >= MAX_PAGE)
	{
		_tprintf(_T("Exception: out of pages\n"));
		return EXCEPTION_EXECUTE_HANDLER;
	}

	LPVOID lpResult =
		VirtualAlloc(
			(LPVOID)nextPageAddr,
			pageSize,
			MEM_COMMIT,
			PAGE_READWRITE);

	if (lpResult == NULL)
	{
		_tprintf(_T("VirtualAlloc failed\n"));
		return EXCEPTION_EXECUTE_HANDLER;
	}
	else
	{
		_tprintf(_T("Allocationg another page.\n"));
	}
	pageCnt++;
	nextPageAddr += pageSize / sizeof(int);// 실수주의

	//page faulut 가 발생한 지점부터 실행을 계속
	return EXCEPTION_CONTINUE_EXECUTION;

}