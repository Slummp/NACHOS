#include "syscall.h"

int main()
{
	PutChar('a');
	while (1);
	PutChar('\n');
	return 0;
}
