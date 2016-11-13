#include "syscall.h"
#ifdef CHANGED
void p() {
	volatile int i;
	for (i=0; i<5;i++)
		PutChar('a');
	ThreadExit();

}
int main()
{

	ThreadCreate((void*)p, 0);
	ThreadCreate((void*)p, 0);
	ThreadCreate((void*)p, 0);
	ThreadCreate((void*)p, 0);
	ThreadExit();
}

#endif //CHANGED