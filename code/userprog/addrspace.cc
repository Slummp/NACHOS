// addrspace.cc 
//      Routines to manage address spaces (executing user programs).
//
//      In order to run a user program, you must:
//
//      1. link with the -N -T 0 option 
//      2. run coff2noff to convert the object file to Nachos format
//              (Nachos object code format is essentially just a simpler
//              version of the UNIX executable object code format)
//      3. load the NOFF file into the Nachos file system
//              (if you haven't implemented the file system yet, you
//              don't need to do this last step)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "addrspace.h"
#include "noff.h"
#include "syscall.h"
#include "new"

#ifdef CHANGED

static void ReadAtVirtual(OpenFile *executable, int virtualAddr, int numBytes, int position, TranslationEntry *pageTable, unsigned numPages);

#endif //CHANGED
//----------------------------------------------------------------------
// SwapHeader
//      Do little endian to big endian conversion on the bytes in the 
//      object file header, in case the file was generated on a little
//      endian machine, and we're now running on a big endian machine.
//----------------------------------------------------------------------

static void
SwapHeader (NoffHeader * noffH)
{
    noffH->noffMagic = WordToHost (noffH->noffMagic);
    noffH->code.size = WordToHost (noffH->code.size);
    noffH->code.virtualAddr = WordToHost (noffH->code.virtualAddr);
    noffH->code.inFileAddr = WordToHost (noffH->code.inFileAddr);
    noffH->initData.size = WordToHost (noffH->initData.size);
    noffH->initData.virtualAddr = WordToHost (noffH->initData.virtualAddr);
    noffH->initData.inFileAddr = WordToHost (noffH->initData.inFileAddr);
    noffH->uninitData.size = WordToHost (noffH->uninitData.size);
    noffH->uninitData.virtualAddr =
	WordToHost (noffH->uninitData.virtualAddr);
    noffH->uninitData.inFileAddr = WordToHost (noffH->uninitData.inFileAddr);
}

//----------------------------------------------------------------------
// AddrSpace::AddrSpace
//      Create an address space to run a user program.
//      Load the program from a file "executable", and set everything
//      up so that we can start executing user instructions.
//
//      Assumes that the object code file is in NOFF format.
//
//      First, set up the translation from program memory to physical 
//      memory.  For now, this is really simple (1:1), since we are
//      only uniprogramming, and we have a single unsegmented page table
//
//      "executable" is the file containing the object code to load into memory
//----------------------------------------------------------------------

AddrSpace::AddrSpace (OpenFile * executable)
{
    NoffHeader noffH;
    unsigned int i, size;
    
    //Increasing process counter 
    nbProcess++;
    DEBUG('s', "Inc nb process %d\n", nbProcess);

    executable->ReadAt (&noffH, sizeof (noffH), 0);
    if ((noffH.noffMagic != NOFFMAGIC) &&
	(WordToHost (noffH.noffMagic) == NOFFMAGIC))
	SwapHeader (&noffH);
    /* Check that this is really a MIPS program */
    ASSERT (noffH.noffMagic == NOFFMAGIC);

    // how big is address space?
    size = noffH.code.size + noffH.initData.size + noffH.uninitData.size + UserStacksAreaSize;	// we need to increase the size
    // to leave room for the stack
    numPages = divRoundUp (size, PageSize);
    size = numPages * PageSize;

    // check we're not tryingP
    // to run anything too big --
    // at least until we have
    // virtual memory
    if (numPages > pageProvider->numAvailPage()) //NumPagePhys
	    throw std::bad_alloc();

    DEBUG ('s', "Initializing address space, num pages %d, total size 0x%x\n",
	    numPages, size);
    // first, set up the translation 
    pageTable = new TranslationEntry[numPages];
    int physPage = 0;
    for (i = 0; i < numPages; i++)
      {
      DEBUG('s', "Assigning page %d / %d\n", i+1, numPages);
      physPage = pageProvider->getEmptyPage();
      pageTable[i].physicalPage = physPage; //i + 1;
      ASSERT(physPage != -1);
      //DEBUG('s', "Assigning page %d\n", pageTable[i].physicalPage);
      //pageTable[i].virtualPage = i + 1;
	  pageTable[i].valid = TRUE;
	  pageTable[i].use = FALSE;
	  pageTable[i].dirty = FALSE;
	  pageTable[i].readOnly = FALSE;	// if the code segment was entirely on 
	  // a separate page, we could set its 
	  // pages to be read-only
      }

    // then, copy in the code and data segments into memory
    if (noffH.code.size > 0)
      {
	  DEBUG ('a', "Initializing code segment, at 0x%x, size 0x%x\n",
		 noffH.code.virtualAddr, noffH.code.size);
      ReadAtVirtual(executable, noffH.code.virtualAddr, noffH.code.size, noffH.code.inFileAddr, pageTable, numPages);
      }
    if (noffH.initData.size > 0)
      {
	  DEBUG ('a', "Initializing data segment, at 0x%x, size 0x%x\n",
		 noffH.initData.virtualAddr, noffH.initData.size);
      ReadAtVirtual(executable, noffH.initData.virtualAddr, noffH.initData.size, noffH.initData.inFileAddr, pageTable, numPages);
      }

    DEBUG ('a', "Area for stacks at 0x%x, size 0x%x\n",
	   size - UserStacksAreaSize, UserStacksAreaSize);

    pageTable[0].valid = FALSE;			// Catch NULL dereference
    
    #ifdef CHANGED
        lockBitmap = new Lock("bitmap allocStack");
        nbrStacks = (UserStacksAreaSize)/ 256;
        bitmap = new BitMap(nbrStacks); 
        // [0][0]...
        bitmap->Mark(0); //On reserve la pile du thread principal
    #endif //CHANGED
}

//----------------------------------------------------------------------
// AddrSpace::~AddrSpace
//      Dealloate an press space.  Nothing for now!
//----------------------------------------------------------------------

AddrSpace::~AddrSpace ()
{
  // LB: Missing [] for delete
  // delete pageTable;

  for (unsigned int i = 0; i < numPages; i++)
  {
       pageProvider->releasePage(pageTable[i].physicalPage);
  }

  delete [] pageTable;
  
  #ifdef CHANGED
  
    delete lockBitmap;
  
  #endif //CHANGED

  // End of modification
}

//----------------------------------------------------------------------
// AddrSpace::InitRegisters
//      Set the initial values for the user-level register set.
//
//      We write these directly into the "machine" registers, so
//      that we can immediately jump to user code.  Note that these
//      will be saved/restored into the currentThread->userRegisters
//      when this thread is context switched out.
//----------------------------------------------------------------------

void
AddrSpace::InitRegisters ()
{
    int i;

    for (i = 0; i < NumTotalRegs; i++)
	machine->WriteRegister (i, 0);

    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg, USER_START_ADDRESS);

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);

    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
    machine->WriteRegister (StackReg, numPages * PageSize - 16);
    DEBUG ('a', "Initializing stack register to 0x%x\n",
	   numPages * PageSize - 16);
}

//----------------------------------------------------------------------
// AddrSpace::SaveState
//      On a context switch, save any machine state, specific
//      to this address space, that needs saving.
//
//      For now, nothing!
//----------------------------------------------------------------------

void
AddrSpace::SaveState ()
{
}

//----------------------------------------------------------------------
// AddrSpace::RestoreState
//      On a context switch, restore the machine state so that
//      this address space can run.
//
//      For now, tell the machine where to find the page table.
//----------------------------------------------------------------------

void
AddrSpace::RestoreState ()
{
    machine->pageTable = pageTable;
    machine->pageTableSize = numPages;
}

#ifdef CHANGED

    int AddrSpace::AllocateUserStack(int *bitIndex) {
        
        lockBitmap->Acquire();
        // Crit
        
        //On cherche une pile à allouer
        *bitIndex = bitmap->Find();
        DEBUG('s', "Attribution de %d\n", *bitIndex);
        int stackPointer = (PageSize * numPages - 256 * *bitIndex);
        
        // /Crit
        lockBitmap->Release();

        return stackPointer;
    }

    int AddrSpace::DeallocateUserStack(int bitIndex) {
        
        DEBUG('s', "Liberation de %d\n", bitIndex);
        lockBitmap->Acquire();
        // Crit
        
        bitmap->Clear(bitIndex); //On libère la pile ciblée par bitIndex
        int c = nbrStacks - bitmap->NumClear();
        //bitmap->Print();
        // /Crit
        lockBitmap->Release();


        return c;
    }
    static void ReadAtVirtual(OpenFile *executable, int virtualAddr, int numBytes, int position, 
                            TranslationEntry *pageTable, unsigned numPages)
    {
        char buf[numBytes];
        unsigned short sizeChar = sizeof(char);

        // Sauvegarde de l'état du processus initial
        TranslationEntry *oldPageTable = machine->pageTable;
        int oldNumPage = machine->pageTableSize;

        // Mise en place de l'état du processus créé
        machine->pageTable = pageTable;
        machine->pageTableSize = numPages;

        executable->ReadAt(buf, numBytes, position);

        for(int i = 0 ; i < numBytes ; i++)
        {
            machine->WriteMem(virtualAddr + i, sizeChar, (int) buf[i]);
        }

        // Restauration de l'état du processus initial
        machine->pageTable = oldPageTable;
        machine->pageTableSize = oldNumPage;
    }
#endif //CHANGED
