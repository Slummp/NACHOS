#include "syscall.h"
#ifdef CHANGED
int main()
{

	ForkExec("../test/SeveralThreads");
	ForkExec("../test/putchar");
	Exit(0);
}
#endif //CHANGED