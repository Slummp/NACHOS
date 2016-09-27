#include "syscall.h"

int main()
{
	PutString("aaa\n\0");
	Halt();
}

/* 
	Explications sémaphore
	
	P(c)
	if (i > 0) i--; */
