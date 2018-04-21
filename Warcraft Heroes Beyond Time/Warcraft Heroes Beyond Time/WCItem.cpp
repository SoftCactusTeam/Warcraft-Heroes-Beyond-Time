#include "WCItem.h"
#include "ModuleRender.h"

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

bool WCItem::printIconOnScreen(iPoint pos)
{
	//The GUI uses this method, fill it in all the items.
	return App->render->Blit(App->items->getItemsTexture(), pos.x, pos.y, WC_ICON, 1, 0);
}


