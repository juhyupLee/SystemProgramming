#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define WORK_MAX 10000
#define THREAD_MAX 50
typedef void (*WORK) (void);

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
	//Work을 등록하기 위한 배열
	WORK workList[WORK_MAX];

	//Thread정보와 각 Thread 별 Event 오브젝트
	WorkerThread workerThreadList[THREAD_MAX];
	HANDLE workerEventList[THREAD_MAX];

	// Work에 대한 정보
	DWORD idxOfCurrentWork; // 대기 1순위 Work Index
	DWORD idxOfLastAddedWork;// 마지막 추가 Work Index +1
	
	//Number of Thread
	DWORD threadIdx; // pool에 존재하는 Thread의 개수

}gThreadPool;

//Mutext 관련 함수들
// gThreadPool에 대한 접근을 보호하기 위해 정의

static HANDLE mutex = NULL;

void InitMutex(void)
{
	mutex = CreateMutex(NULL,FALSE,NULL);
}
void DeInitMutex(void)
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
	{
		_tprintf(_T("Error Occur!\n"));
	}
}
//Thread Pool에 Work을 등록시키기 위한 함수
DWORD AddWorkToPool(WORK work)
{
	AcquireMutex();
	if (gThreadPool.idxOfLastAddedWork >= WORK_MAX)
	{
		_tprintf(_T("AddWorkToPool Fail!\n"));
		return NULL;
	}
	// Work 등록
	gThreadPool.workList[gThreadPool.idxOfLastAddedWork++] = work;

	// Work 등록 후, 대기중인 쓰레드듣ㄹ을 모두 깨워 일을 시작하도록 함
	// 모두 개울 필요가 없으므로 정교함이 떨어지는 부분이다
	for (DWORD i = 0; i < gThreadPool.threadIdx; ++i)
	{
		SetEvent(gThreadPool.workerEventList[i]);
	}
	ReleaseMutex();
	return 1;
}
// Thread에서 Work을 가져올 때 호출되는 함수
// gThreadPool에 대한 접근을 보호하기 위해 정의

WORK GetWorkFromPool()
{
	WORK work = NULL;

	AcquireMutex();
	//현재 처리해야할 Work이 존재하지 않는다면
	if (!(gThreadPool.idxOfCurrentWork < gThreadPool.idxOfLastAddedWork))
	{
		ReleaseMutex();
		return NULL;
	}
	work = gThreadPool.workList[gThreadPool.idxOfCurrentWork++];
	ReleaseMutex();

	return work;
}
// Thread Pool 생성
// 전달되는 인자값에 따라 Thread 생성

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

		//Event 생성
		gThreadPool.workerEventList[gThreadPool.threadIdx] = CreateEvent(NULL, FALSE, FALSE,NULL);
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkerThreadFunction, (LPVOID)gThreadPool.threadIdx, 0, &idThread);

		gThreadPool.workerThreadList[gThreadPool.threadIdx].hThread = hThread;
		gThreadPool.workerThreadList[gThreadPool.threadIdx].idThread = idThread;
		gThreadPool.threadIdx++;
	}

	return numOfThread; // 생성된쓰레드의 수를 반환한다.
}

//전달되어야 할 대상
void WorkerThreadFunction(LPVOID pParam)
{
	WORK workFunction;
	HANDLE event = gThreadPool.workerEventList[(DWORD)pParam];

	while (1)
	{
		workFunction = GetWorkFromPool();
		if (workFunction == NULL)
		{
			// Work가 할당될 때까지 Blocked 상태에 있게 된다.
			WaitForSingleObject(event, INFINITE);
			continue;
		}
		workFunction();

	}
}
void TestFunction()
{
	// i는 static 이므로 둘이상의 쓰레드에 의해 동시접근 가능하다
	// 따라서 동기화가 필요하지만 동기화를 생략한다.

	static int i = 0;
	i++;

	_tprintf(_T("Good Test --%d: Processing thread: %d--\n\n"), i, GetCurrentThreadId());

}

int _tmain(int argc, TCHAR* argv[])
{
	MakeThreadToPool(3);

	for (int i = 0; i < 100; i++)
	{
		AddWorkToPool(TestFunction);
	}
	Sleep(50000);
	return 0;
}