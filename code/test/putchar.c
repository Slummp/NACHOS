#include "syscall.h"
void print(char c, int n)
{
	#if 1
		int i;
		for (i = 0; i < n; i++) {
			PutChar(c + i);
		}
		PutChar('\n');
	#endif
}
int main()
{
	print('a',4);
	
}

/* 
	Explications sémaphore
	
	P(c)
	if (i > 0) i--; */
