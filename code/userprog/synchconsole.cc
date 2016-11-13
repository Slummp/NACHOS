#ifdef CHANGED
	#include "copyright.h"
	#include "system.h"
	#include "synchconsole.h"

	static Semaphore* readAvail;
	static Semaphore* writeDone;
	static void ReadAvailHandler(void* arg) { (void) arg; readAvail->V(); }
	static void WriteDoneHandler(void* arg) { (void) arg; writeDone->V(); }
	SynchConsole::SynchConsole(const char* in, const char* out)
	{
		readAvail = new Semaphore("read avail", 0);
		writeDone = new Semaphore("write done", 0);

		lockGetChar = new Lock("lockGetChar");
		lockPutChar = new Lock("lockPutChar");
		
		console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, 0);
	}
	SynchConsole::~SynchConsole()
	{
		delete console;
		delete writeDone;
		delete readAvail;/*
		delete lockGetChar;
		delete lockPutChar;*/
	}
	void SynchConsole::SynchPutChar(int ch)
	{
		lockPutChar->Acquire(); //Start in critical zone

		console->PutChar(ch);
		writeDone->P();

		lockPutChar->Release(); //End of critial zone
	}
	int SynchConsole::SynchGetChar()
	{

		lockGetChar->Acquire();
		
		int c;
		
		readAvail->P();
		c = console->GetChar();

		lockGetChar->Release();

		return c;
	}
	void SynchConsole::SynchPutString(const char* str)
	{
		unsigned int i = 0;
		while (str[i] != '\0')
		{
			SynchPutChar(str[i]);
			i++;
		}
	}
	void SynchConsole::SynchGetString(char* s, int n)
	{
		int i = 0;
		while(i < n)
		{
			s[i] = SynchGetChar();
			if(s[i] == EOF)
				break;

			i++;
		}
		s[i] = '\0';
	}
	
	
#endif // CHANGED
