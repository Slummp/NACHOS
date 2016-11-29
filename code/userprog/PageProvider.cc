#ifdef CHANGED
#include "system.h"
#include "PageProvider.h"

PageProvider::PageProvider(int nitems) 
{
	bitmap = new BitMap(nitems);
}

PageProvider::~PageProvider() 
{
	delete bitmap;
}

int PageProvider::getEmptyPage() 
{
	int page = bitmap->Find();
	
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