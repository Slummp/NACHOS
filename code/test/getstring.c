#include "syscall.h"

int main()
{
	char s[255];
	GetString(s, 255);

    PutString(s);
    PutChar('\n');
	
	Halt();	
}
