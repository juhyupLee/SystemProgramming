//#include <stdio.h>
//#include <tchar.h>
//#include <Windows.h>
//
//#define MAX_THREADS (1024*10)
//
//DWORD WINAPI ThreadProc(LPVOID lpParm)
//{
//	DWORD threadNum = (DWORD)lpParm;
//
//	while (1)
//	{
//		_tprintf(_T("thread num : %d\n"), threadNum);
//		Sleep(5000);
//	}
//	return 0;
//
//}
//
//DWORD cntOfThread = 0;
//int _tmain(int argc, TCHAR* argv[])
//{
//	DWORD dwThreadID[MAX_THREADS];
//	HANDLE hThread[MAX_THREADS];
//
//	while (1)
//	{
//		hThread[cntOfThread] = CreateThread(NULL,1024*1024*10, ThreadProc, (LPVOID)cntOfThread, 0, &dwThreadID[cntOfThread]);
//		//디폴트 보안 속성 지정
//		// 디폴트 스택 사이즈
//		// 쓰레드 함수
//		// 쓰레드 함수의 전달인자
//		// 디폴트 생성 flag 지정
//		// 쓰레드 ID  반환
//	
//		if (hThread[cntOfThread] == NULL)
//		{
//			_tprintf(_T("MAXIMUM THREAD NUMBER: %d"), cntOfThread);
//			break;
//		}
//		cntOfThread++;
//
//
//	}
//	for (DWORD i = 0; i < cntOfThread; ++i)
//	{
//		CloseHandle(hThread[i]);
//	}
//
//	WaitForMultipleObjects(3, hThread, TRUE, INFINITE);
//	_tprintf(_T("total(1~10):%d \n"), total);
//
//
//
//	return 0;
//
//}