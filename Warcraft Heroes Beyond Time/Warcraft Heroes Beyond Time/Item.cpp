#include "Item.h"

Item::Item()
{}

Item::Item(char* name, ItemType type, uint rarity) : name(name), type(type), rarity(rarity)
{}

Item::~Item()
{}

bool Item::GetItem()
{
	return false;
}

bool Item::UpdateLogic()
{
	return false;
}

bool Item::ByeByeItem()
{
	return false;
}
