#ifdef CHANGED

#include "machine.h"
#include "system.h"
#include "userprocess.h"
typedef struct UserProcessArgs {
    AddrSpace* space;
} ThreadArgs;

void do_ForkExec(const char *s) {
	OpenFile* file = fileSystem->Open(s);
	AddrSpace *space;

	if (file == NULL)
	{
		printf ("Unable to open file %s\n", s);
		return;
	}
	space = new AddrSpace (file);

	DEBUG('s', "ForkExec %s\n", s);

	delete file;		// close file
	
	Thread *newThread = new Thread("new Thread userProcess");
	

	newThread->Start(StartUserProcess, space);

}

void StartUserProcess(void* space) {
	currentThread->space = (AddrSpace*) space;
	currentThread->space->InitRegisters ();	// set the initial register values
	currentThread->space->RestoreState ();	        // load page table register

	machine->Run ();		// jump to the user progam
	ASSERT (FALSE);		        // machine->Run never returns;
	// the address space exits
	// by doing the syscall "exit"

}
#endif //CHANGED