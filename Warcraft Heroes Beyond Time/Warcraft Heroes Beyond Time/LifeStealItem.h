#ifndef _RICS_SWORD_
#define _RICS_SWORD_

#include "Item.h"

#define LIFE_STEAL_ICON {34,214,31,31} //The rect from the texture of your gui icon

class LifeStealItem :
	public Item
{
public:
	LifeStealItem();
	~LifeStealItem();


	bool Start();
	bool Act(ModuleItems::ItemEvent event, float dt = App->dt);
	bool Draw();
	bool printYourStuff(iPoint pos);

private:

	virtual const std::string myNameIs() const override;

	std::string softDescription = "\"Difficulty levels decreasing\"";
	std::string Title = "Ric's Sword";

};

#endif
