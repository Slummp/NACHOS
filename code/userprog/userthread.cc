#ifdef CHANGED

#include "thread.h"
#include "userthread.h"
#include "machine.h" 
#include "system.h"
#include "syscall.h"

typedef struct ThreadArgs {
	int func;
	int arg;
} ThreadArgs;

int do_ThreadCreate(int f, int arg) {
		
	Thread *newThread = new Thread("new Thread");
	ThreadArgs *schmurtz = new ThreadArgs();
	schmurtz->func = f;
	schmurtz->arg = arg;
	
	newThread->Start(StartUserThread, schmurtz);
}
void do_ThreadExit() {
	currentThread->Finish();
}

void StartUserThread(void *p) {
    int i;
	
    for (i = 0; i < NumTotalRegs; i++)
	   machine->WriteRegister (i, 0);
    
    int f = ((int *) p)[0];
    int args = ((int *) p)[1];

    
    DEBUG('s', "PCReg - f : %d\n", f);
    machine->WriteRegister (PCReg, f);
    
    DEBUG('s', "NextPCReg - args: %d\n", machine->ReadRegister(PCReg) + 4);
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);
    
    
    DEBUG('s', "Reg 4 - args: %d\n", args);
    machine->WriteRegister (4, args);
    
    DEBUG('s', "StackReg - AllocateUserStack: %d\n", currentThread->space->AllocateUserStack(1));
    machine->WriteRegister (StackReg, currentThread->space->AllocateUserStack(1));

    
   
    free((int *)p);
    machine->Run();
}
#endif //CHANGED
