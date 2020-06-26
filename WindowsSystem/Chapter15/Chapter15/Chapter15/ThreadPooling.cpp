#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <strsafe.h>
#define WORK_MAX 10000
#define THREAD_MAX 50

typedef void (*WORK)(void);

DWORD AddWorkToPool(WORK work);
WORK GetWorkFromPool(void);
DWORD MakeThreadToPool(DWORD numOfThread);
void WorkerThreadFunction(LPVOID pParam);

typedef struct __WorkerThread
{
	HANDLE hThread;
	DWORD idThread;

}WorkerThread;

struct __ThreadPool
{
	WORK workList[WORK_MAX];

	//Thread 정보와 각 Thread별 event 오브젝트
	WorkerThread workerThreadList[THREAD_MAX];
	HANDLE workerEventList[THREAD_MAX];

	DWORD idxOfCurrentWork;
	DWORD idxOfLastAddedWork; 

	DWORD threadIdx; // Pool에 존재하는 Thread의개수

}gThreadPool;

//뮤텍스 관련 함수들
// gThreadPool에 대한 접근을 보호하기 위해 정의

static HANDLE mutex = NULL;

void InitMutex(void)
{
	mutex = CreateMutex(NULL, FALSE, NULL);
}
void DeInitmutex(void)
{
	BOOL ret = CloseHandle(mutex);
}
void AcquireMutex(void)
{
	DWORD ret = WaitForSingleObject(mutex, INFINITE);
	if (ret == WAIT_FAILED)
	{
		_tprintf(_T("Error Occur!\n"));
	}
}
void ReleaseMutex(void)
{
	BOOL ret = ReleaseMutex(mutex);
	if (ret == 0)
		_tprintf(_T("Error Occur!\n"));
}
DWORD AddWorkToPool(WORK work)
{
	AcquireMutex();
	//여러번 AddWorkToPool 할때 다른 쓰레드들을 깨우니
	if (gThreadPool.idxOfLastAddedWork >= WORK_MAX)
	{
		_tprintf(_T("AddWorkToPool fail\n"));
		return NULL;
	}

	gThreadPool.workList[gThreadPool.idxOfLastAddedWork++] = work;
	
	//Work 등록후 , 대기중인쓰레드들은 모두 깨워 일을 시작하도록함
	// 모두 깨울 필요가 없으므로 정교함이 떨어지는 부분이다.
	
	for (DWORD i = 0; i < gThreadPool.threadIdx; i++)
	{
		SetEvent(gThreadPool.workerEventList[i]);
	}
	ReleaseMutex();
	return 1;
}

WORK GetWorkFromPool()
{
	WORK work = NULL;

	//AcquireMutex();
	if (!(gThreadPool.idxOfCurrentWork < gThreadPool.idxOfLastAddedWork))
	{
		ReleaseMutex();
		return NULL;
	}
	work = gThreadPool.workList[gThreadPool.idxOfCurrentWork++];

	//ReleaseMutex();
	return work;
}

DWORD MakeThreadToPool(DWORD numOfThread)
{
	InitMutex();
	DWORD capacity = WORK_MAX - (gThreadPool.threadIdx);

	if (capacity < numOfThread)
	{
		numOfThread = capacity;
	}
	for (DWORD i = 0; i < numOfThread; i++)
	{
		DWORD idThread;
		HANDLE hThread;

		gThreadPool.workerEventList[gThreadPool.threadIdx]
			= CreateEvent(NULL, FALSE, FALSE, NULL);

		hThread = CreateThread(
			NULL, 0,
			(LPTHREAD_START_ROUTINE)WorkerThreadFunction,
			(LPVOID)gThreadPool.threadIdx,
			0, &idThread);
		gThreadPool.workerThreadList[gThreadPool.threadIdx].hThread
			= hThread;
		gThreadPool.workerThreadList[gThreadPool.threadIdx].idThread
			= idThread;
	}

	return numOfThread; // 생성된 쓰레드의 수를 반환한다.

}
void WorkerThreadFunction(LPVOID pParam)
{
	WORK workFunction;
	HANDLE event = gThreadPool.workerEventList[(DWORD)pParam];
	while (1)
	{
		workFunction = GetWorkFromPool();
		if (workFunction == NULL)
		{
			WaitForSingleObject(event, INFINITE);
			continue;
		}
		else
		{
			workFunction();
		}
	}
}

void TestFunction()
{
	static int i = 0;
	i++;
	_tprintf(
		_T("Good Test -- %d: Processing thread : %d--\n\n"),
		i, GetCurrentThreadId());
}
int _tmain()
{
	MakeThreadToPool(3);

	for (int i = 0; i < 100; i++)
	{
		AddWorkToPool(TestFunction);
	}
	Sleep(50000); 
	return 0;

}

//Good Test-- 1: Processing thread : 10308--
//
//Good Test-- 4 : Processing thread : 10308--
//
//Good Test-- 3 : Processing thread : 11288--
//
//Good Test-- 2 : Processing thread : 13392--
//
//Good Test-- 5 : Processing thread : 10308--
//
//Good Test-- 6 : Processing thread : 11288--
//
//Good Test-- 7 : Processing thread : 13392--
//
//Good Test-- 8 : Processing thread : 10308--
//
//Good Test-- 9 : Processing thread : 11288--
//
//Good Test-- 10 : Processing thread : 13392--
//
//Good Test-- 11 : Processing thread : 10308--
//
//Good Test-- 12 : Processing thread : 11288--
//
//Good Test-- 13 : Processing thread : 13392--
//
//Good Test-- 14 : Processing thread : 10308--
//
//Good Test-- 15 : Processing thread : 11288--
//
//Good Test-- 18 : Processing thread : 11288--
//
//Good Test-- 17 : Processing thread : 10308--
//
//Good Test-- 20 : Processing thread : 10308--
//
//Good Test-- 21 : Processing thread : 10308--
//
//Good Test-- 22 : Processing thread : 10308--
//
//Good Test-- 19 : Processing thread : 11288--
//
//Good Test-- 24 : Processing thread : 11288--
//
//Good Test-- 23 : Processing thread : 10308--
//
//Good Test-- 26 : Processing thread : 10308--
//
//Good Test-- 25 : Processing thread : 11288--
//
//Good Test-- 27 : Processing thread : 11288--
//
//Good Test-- 28 : Processing thread : 11288--
//
//Good Test-- 30 : Processing thread : 11288--
//
//Good Test-- 16 : Processing thread : 13392--
//
//Good Test-- 31 : Processing thread : 13392--
//
//Good Test-- 32 : Processing thread : 13392--
//
//Good Test-- 34 : Processing thread : 13392--
//
//Good Test-- 33 : Processing thread : 11288--
//
//Good Test-- 35 : Processing thread : 11288--
//
//Good Test-- 29 : Processing thread : 10308--
//
//Good Test-- 38 : Processing thread : 10308--
//
//Good Test-- 37 : Processing thread : 11288--
//
//Good Test-- 40 : Processing thread : 11288--
//
//Good Test-- 39 : Processing thread : 10308--
//
//Good Test-- 42 : Processing thread : 10308--
//
//Good Test-- 41 : Processing thread : 11288--
//
//Good Test-- 44 : Processing thread : 11288--
//
//Good Test-- 45 : Processing thread : 11288--
//
//Good Test-- 46 : Processing thread : 11288--
//
//Good Test-- 47 : Processing thread : 11288--
//
//Good Test-- 48 : Processing thread : 11288--
//
//Good Test-- 49 : Processing thread : 11288--
//
//Good Test-- 50 : Processing thread : 11288--
//
//Good Test-- 51 : Processing thread : 11288--
//
//Good Test-- 52 : Processing thread : 11288--
//
//Good Test-- 53 : Processing thread : 11288--
//
//Good Test-- 36 : Processing thread : 13392--
//
//Good Test-- 55 : Processing thread : 13392--
//
//Good Test-- 56 : Processing thread : 13392--
//
//Good Test-- 43 : Processing thread : 10308--
//
//Good Test-- 57 : Processing thread : 10308--
//
//Good Test-- 54 : Processing thread : 11288--
//
//Good Test-- 60 : Processing thread : 11288--
//
//Good Test-- 59 : Processing thread : 10308--
//
//Good Test-- 62 : Processing thread : 10308--
//
//Good Test-- 61 : Processing thread : 11288--
//
//Good Test-- 64 : Processing thread : 11288--
//
//Good Test-- 63 : Processing thread : 10308--
//
//Good Test-- 66 : Processing thread : 10308--
//
//Good Test-- 65 : Processing thread : 11288--
//
//Good Test-- 68 : Processing thread : 11288--
//
//Good Test-- 67 : Processing thread : 10308--
//
//Good Test-- 70 : Processing thread : 10308--
//
//Good Test-- 69 : Processing thread : 11288--
//
//Good Test-- 72 : Processing thread : 11288--
//
//Good Test-- 71 : Processing thread : 10308--
//
//Good Test-- 73 : Processing thread : 10308--
//
//Good Test-- 58 : Processing thread : 13392--
//
//Good Test-- 76 : Processing thread : 13392--
//
//Good Test-- 75 : Processing thread : 10308--
//
//Good Test-- 78 : Processing thread : 10308--
//
//Good Test-- 77 : Processing thread : 13392--
//
//Good Test-- 80 : Processing thread : 13392--
//
//Good Test-- 81 : Processing thread : 13392--
//
//Good Test-- 74 : Processing thread : 11288--
//
//Good Test-- 83 : Processing thread : 11288--
//
//Good Test-- 82 : Processing thread : 13392--
//
//Good Test-- 79 : Processing thread : 10308--
//
//Good Test-- 86 : Processing thread : 10308--
//
//Good Test-- 85 : Processing thread : 13392--
//
//Good Test-- 88 : Processing thread : 13392--
//
//Good Test-- 87 : Processing thread : 10308--
//
//Good Test-- 89 : Processing thread : 10308--
//
//Good Test-- 84 : Processing thread : 11288--
//
//Good Test-- 92 : Processing thread : 11288--
//
//Good Test-- 91 : Processing thread : 10308--
//
//Good Test-- 93 : Processing thread : 10308--
//
//Good Test-- 90 : Processing thread : 13392--
//
//Good Test-- 96 : Processing thread : 13392--
//
//Good Test-- 95 : Processing thread : 10308--
//
//Good Test-- 98 : Processing thread : 10308--
//
//Good Test-- 97 : Processing thread : 13392--
//
//Good Test-- 100 : Processing thread : 13392--
//
//Good Test-- 99 : Processing thread : 10308--
//
//Good Test-- 94 : Processing thread : 11288--

