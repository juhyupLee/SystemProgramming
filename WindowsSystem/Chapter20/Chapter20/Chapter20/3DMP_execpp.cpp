#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <stdlib.h>

#define MEGA 1048576
PBYTE stptr = NULL;

#define MAX_PAGE 10

void ReadRecords();

int _tmain(int argc, TCHAR* argv[])
{

	ReadRecords();

}

void ReadRecords( )
{
	int c = 0;
	int RecSize = 0;
	PBYTE nowptr, endptr;
	stptr = (PBYTE)VirtualAlloc(NULL, (100 * MEGA), MEM_RESERVE, PAGE_READWRITE);
	if (stptr == NULL)
	{
		printf("메모리 예약실패");
	}
	nowptr = stptr;
	endptr = stptr;

	c = rand()%90 + 10;
	for (int i = 0; i < c; ++i)
	{
		if ((endptr - nowptr) < MEGA)
		{
			VirtualAlloc(endptr, MEGA, MEM_COMMIT, PAGE_READWRITE);
			endptr += MEGA;
		}
		RecSize = ((rand() % 100 )+ 1) * 10240;
		memset(nowptr, i, RecSize);
		nowptr += RecSize;
	}


}