#ifndef STASH_H
/* defines */
#define STASH_H

/* forward declare */
class Stash;

/* includes */
#include <list>
#include "Item.h"
#include "Globals.h"

/* namespaces */
using namespace std;

/* Stash */
class Stash {
	public:
		/* variables */
		unsigned char top_symbol;
		list<Item> items;

		/* constructors */
		Stash(unsigned char top_symbol = ILLEGAL_ITEM);

		/* methods */
		void addItem(const Item &item);
		void removeItem(const Item &item);
};
#endif