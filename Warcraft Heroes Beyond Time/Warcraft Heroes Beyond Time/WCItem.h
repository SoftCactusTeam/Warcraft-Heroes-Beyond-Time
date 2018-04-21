#ifndef _WC_ITEM_
#define _WC_ITEM_
#include "Item.h"

class WCItem : public Item
{
public:
	WCItem() {}
	virtual ~WCItem() {}

	bool Start();
	bool Act(ModuleItems::ItemEvent event, float dt = App->dt);
	bool Draw();
};

#endif