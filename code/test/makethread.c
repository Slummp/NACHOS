#include "syscall.h"
void p() {
	PutChar('a');
	ThreadExit();

}
int main()
{

	ThreadCreate((void*)p, 0);
	while (1) {}
}
