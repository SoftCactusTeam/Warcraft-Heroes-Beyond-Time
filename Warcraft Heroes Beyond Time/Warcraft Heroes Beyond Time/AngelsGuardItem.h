#ifndef _ANGELS_GUARDIAN_ITEM_
#define _ANGELS_GUARDIAN_ITEM_

#include "Item.h"
#include <string>

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
	bool printYourStuff(iPoint pos);

private:
	std::string softDescription = "\"One more chance...\"";
	std::string Title = "Angel's sword";
};

#endif
