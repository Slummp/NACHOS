#ifdef CHANGED

#include "thread.h"
#include "userthread.h"
#include "machine.h" 
#include "system.h"
#include "syscall.h"

typedef struct ThreadArgs {
	int func;
	int arg;
    int stackPointer;
} ThreadArgs;

int do_ThreadCreate(int f, int arg) {
	DEBUG('t', "Creating thread...\n");
	
    Thread *newThread = new Thread("new Thread");
	ThreadArgs *schmurtz = new ThreadArgs();
	schmurtz->func = f;
	schmurtz->arg = arg;
    
    // On fait passer le pointer de pile à StartUserThread grace à la structure
    // pour ne pas utiliser currentThread qui peut être modifié entre le moment
    // où on passe de do_ThreadCreate à StartUserThread

    schmurtz->stackPointer = currentThread->space->AllocateUserStack(&newThread->bitIndex);
    
    newThread->Start(StartUserThread, (void *)schmurtz);
	DEBUG('t', "Thread Created\n");

    return 0;
}
void do_ThreadExit() {
    
    /* Si c > 0 : Il y a encore au moins un thread en execution */
    DEBUG('s', "do_ThreadExit %d\n", currentThread->bitIndex);
    int c = currentThread->space->DeallocateUserStack(currentThread->bitIndex);
    DEBUG('s', "c: %d\n", c);
    if (c > 0)
        currentThread->Finish();
    else
        interrupt->Halt();

}

static void StartUserThread(void *p) {
    DEBUG('t', "StartUserThread...\n");
    int i;
	
    for (i = 0; i < NumTotalRegs; i++)
	   machine->WriteRegister (i, 0);
    
    int f = ((int *) p)[0];
    int args = ((int *) p)[1];
    int stackPointer = ((int *) p)[2];

    
    DEBUG('s', "PCReg - f : %d\n", f);
    machine->WriteRegister (PCReg, f);

    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);
    
    
    DEBUG('s', "Reg 4 - args: %d\n", args);
    machine->WriteRegister (4, args);
    
    
    DEBUG('s', "StackReg - AllocateUserStack: %d\n", stackPointer);
    machine->WriteRegister (StackReg, stackPointer);

    
   
    delete (struct ThreadArgs *) p;
    machine->Run();
}
#endif //CHANGED
