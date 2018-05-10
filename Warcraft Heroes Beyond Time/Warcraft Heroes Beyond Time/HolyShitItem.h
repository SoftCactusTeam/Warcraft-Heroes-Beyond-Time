#ifndef _HOLY_SHIT_ITEM_
#define _HOLY_SHIT_ITEM_

#include "Item.h"

#define HOLY_SHIT_ICON {67,117,32,28}


class HolyShitItem :
	public Item
{
public:
	HolyShitItem();
	~HolyShitItem();

	bool Start();
	bool Act(ModuleItems::ItemEvent event, float dt = App->dt);
	bool Draw();
	bool printIconOnScreen(iPoint pos);
};


#endif