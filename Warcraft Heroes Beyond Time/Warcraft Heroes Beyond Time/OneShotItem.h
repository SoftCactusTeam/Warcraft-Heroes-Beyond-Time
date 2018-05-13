#ifndef _ONE_SHOT_ITEM_
#define _ONE_SHOT_ITEM_

#include "Item.h"
#include <memory>

#define ONE_SHOT_ICON {67,51,32,32} //The rect from the texture of your gui icon

class Collider;

class OneShotItem : public Item
{
public:
	OneShotItem() {}
	virtual ~OneShotItem() {}

	bool Start();
	bool Act(ModuleItems::ItemEvent event, float dt = App->dt);
	bool Draw();
	bool printYourStuff(iPoint pos);

private:
	std::string softDescription;
	std::string hardDescription;
};

#endif

