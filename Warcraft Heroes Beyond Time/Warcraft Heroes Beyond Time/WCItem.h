#ifndef _WC_ITEM_
#define _WC_ITEM_

#include "Item.h"

#define WC_ICON {} //The rect from the texture of your gui icon

class WCItem : public Item
{
public:
	WCItem() {}
	virtual ~WCItem() {}

	bool Start();
	bool Act(ModuleItems::ItemEvent event, float dt = App->dt);
	bool Draw();
	bool printIconOnScreen(iPoint pos);
};

#endif