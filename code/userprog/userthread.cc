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
	
	newThread->Start(StartUserThread, (void *)schmurtz);
    return 0;
}
void do_ThreadExit() {
    
    /* Si c > 0 : Il y a encore au moins un thread en execution */
    
    int c = currentThread->space->DeallocateUserStack();
    if (c > 0)
        currentThread->Finish();
    else
        interrupt->Halt();

}

static void StartUserThread(void *p) {
    int i;
	
    for (i = 0; i < NumTotalRegs; i++)
	   machine->WriteRegister (i, 0);
    
    int f = ((int *) p)[0];
    int args = ((int *) p)[1];

    
    DEBUG('s', "PCReg - f : %d\n", f);
    machine->WriteRegister (PCReg, f);

    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);
    
    
    DEBUG('s', "Reg 4 - args: %d\n", args);
    machine->WriteRegister (4, args);
    
    int allocStack = currentThread->space->AllocateUserStack();
    DEBUG('s', "StackReg - AllocateUserStack: %d\n", allocStack);
    machine->WriteRegister (StackReg, allocStack);

    
   
    free((struct ThreadArgs *)p);
    machine->Run();
}
#endif //CHANGED
