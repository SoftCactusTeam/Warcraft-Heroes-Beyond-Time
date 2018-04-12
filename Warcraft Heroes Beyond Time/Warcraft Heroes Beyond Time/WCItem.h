#ifndef _WC_ITEM_
#define _WC_ITEM_
#include "Item.h"

class WCItem :public Item
{
public:
	WCItem();
	WCItem(char* name, ItemType type, uint rarity);
	~WCItem();

public:
	bool got_paper = false;
};

#endif