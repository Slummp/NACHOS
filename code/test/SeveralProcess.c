#include "syscall.h"
#ifdef CHANGED
int main()
{

	ForkExec("../test/putchar");
	ForkExec("../test/putchar");
	ForkExec("../test/putchar");
	
	while (1);
}
#endif //CHANGED