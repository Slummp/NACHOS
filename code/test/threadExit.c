#ifdef CHANGED
#include "syscall.h"

void p(void *args) {
	(void) args;
	PutChar('a');
	ThreadExit();
}
int main(void)
{

	ThreadCreate(&p, 0);
	PutChar('o');
	//while (1) {}
	ThreadExit();
}

#endif //CHANGED