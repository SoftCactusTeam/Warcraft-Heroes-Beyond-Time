#include "WCItem.h"

bool WCItem::Start()
{
	return true;
}

bool WCItem::Act(ModuleItems::ItemEvent event, float dt)
{
	//Here you manage your item depending of the event you receive.
	return true;
}

bool WCItem::Draw()
{
	//Use the ModulePrinter to print all the stuff.
	return true;
}


