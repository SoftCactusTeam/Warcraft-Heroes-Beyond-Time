#pragma once
#include "Item.h"
class WCPaperItem : public Item
{
public:
	WCPaperItem(char* name, ItemType type, uint rarity);
	~WCPaperItem();
};

