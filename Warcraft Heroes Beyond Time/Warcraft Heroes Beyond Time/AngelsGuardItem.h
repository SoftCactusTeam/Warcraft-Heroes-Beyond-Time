#ifndef _ANGELS_GUARDIAN_ITEM_
#define _ANGELS_GUARDIAN_ITEM_

#include "Item.h"
#include <memory>

#define ANGEL_ICON {1,214,32,32} //The rect from the texture of your gui icon

class Collider;

class AngelsGuardItem : public Item
{
public:
	AngelsGuardItem() {}
	virtual ~AngelsGuardItem() {}

	bool Start();
	bool Act(ModuleItems::ItemEvent event, float dt = App->dt);
	bool Draw();
	bool printIconOnScreen(iPoint pos);

private:

};

#endif
