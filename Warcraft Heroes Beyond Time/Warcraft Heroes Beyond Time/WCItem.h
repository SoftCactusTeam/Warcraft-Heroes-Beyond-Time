#ifndef _WC_ITEM_
#define _WC_ITEM_

#include "Item.h"
#include "p2Point.h"
#include "ModuleColliders.h"

#define WC_ICON {102,19,31,29} //The rect from the texture of your gui icon

class WCItem : public Item
{
public:
	WCItem() {}
	virtual ~WCItem() {}

	bool Start();
	bool Act(ModuleItems::ItemEvent event, float dt = App->dt);
	bool Draw();
	bool printIconOnScreen(iPoint pos);

	int time = 0;
	int cont = 0;

	std::list<iPoint> wcpaper;
	std::list<Collider*> paper_collider;
};

#endif