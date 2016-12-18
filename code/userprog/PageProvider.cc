#ifdef CHANGED
#include "system.h"
#include "PageProvider.h"

PageProvider::PageProvider(int nitems) 
{
	bitmap = new BitMap(nitems);
	lockGEP = new Lock("PageProvider GetEmptyPage");
}

PageProvider::~PageProvider() 
{
	delete lockGEP;
	delete bitmap;
}

int PageProvider::getEmptyPage() 
{
	lockGEP->Acquire();
	int page = bitmap->Find();
	lockGEP->Release();
	
	if (page == -1) return -1;

	memset(&(machine->mainMemory[page * PageSize]), 0, PageSize);

	return page;
}

unsigned int PageProvider::numAvailPage() 
{
	return (unsigned int) bitmap->NumClear();
}

void PageProvider::releasePage(int numPage) 
{
	//memset(machine->mainMemory[numPage * PageSize], 0, PageSize);
	bitmap->Clear(numPage);
}

#endif //CHANGED