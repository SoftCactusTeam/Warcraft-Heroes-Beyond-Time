#pragma once
#include "Item.h"

class WCItem :
	public Item
{
public:
	WCItem();
	WCItem(char* name, ItemType type, uint rarity);
	~WCItem();
};

