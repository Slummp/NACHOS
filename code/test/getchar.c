#include "syscall.h"

int main()
{
	char a;
	a=GetChar();
    PutChar(a);
    PutChar('\n');
	
	Halt();	
}