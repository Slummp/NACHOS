#ifdef CHANGED
#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H


#include "bitmap.h"

class PageProvider
{
	public:
		PageProvider(int nitems);
		~PageProvider();

		int getEmptyPage();
		void releasePage(int numPage);
		unsigned int numAvailPage();
	private:
		int availPage;
		BitMap *bitmap;
};

#endif //PAGEPROVIDER_H
#endif //CHANGED
