#include "syscall.h"

int main()
{
	PutString("Test42Test42Test42\n\0");
	
	Halt();
}
