#include "syscall.h"

int main()
{
	char a = GetChar();

    PutChar(a);
    PutChar('\n');
    
	Halt();	
}
