#include <stdio.h>
#include <Windows.h>
#include <process.h>


unsigned int WINAPI Thread_Proc(LPVOID lpParam)
{
	int i = 0;

	while (true)
	{
		i++;
	}

	return 0;

}
int main()
{
	SYSTEM_INFO si;

	GetSystemInfo(&si);

	//우선 프로세스의 우선순위를 높이고,
	SetPriorityClass(GetCurrentProcess(),ABOVE_NORMAL_PRIORITY_CLASS);

	HANDLE* pHandle = new HANDLE[si.dwNumberOfProcessors];

	for (int i = 0; i < si.dwNumberOfProcessors; ++i)
	{
		pHandle[i] = (unsigned int*)_beginthreadex(NULL, 0, Thread_Proc, NULL, 0, NULL);
		SuspendThread(pHandle[i]);
	}

	for (int i = 0; i < si.dwNumberOfProcessors; ++i)
	{
		SetThreadPriority(pHandle[i], THREAD_PRIORITY_ABOVE_NORMAL);
		ResumeThread(pHandle[i]);
	}

	while (true)
	{


	}
	for (int i = 0; i < si.dwNumberOfProcessors; ++i)
	{
		
		CloseHandle(pHandle[i]);
	}
	
	return 0;
}