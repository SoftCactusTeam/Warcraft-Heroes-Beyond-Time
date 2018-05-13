#ifndef _SOROVOLO_GAUNT_
#define _SOROVOLO_GAUNT_

#include "Item.h"
#include "p2Point.h"
#include "ModuleColliders.h"
#include "Item.h"

#define RING_ICON {34,183,19,29} //The rect from the texture of your gui icon
#define RING_ITEM {102,52,31,29}


class Slow_Shit
{
public:
	Slow_Shit(Collider* temp, iPoint pos);
	~Slow_Shit();
	iPoint pos;
private:
	Collider* slow_shit_collider;
};

class RingItem :
	public Item
{
public:
	RingItem();
	~RingItem();

	bool Start();
	bool Act(ModuleItems::ItemEvent event, float dt = App->dt);
	bool Draw();
	bool printYourStuff(iPoint pos);

	float time = 0;
	int cont = 0;

	std::list<Slow_Shit*> slow_shit_list;

private:
	std::string softDescription = "\"You are too slow for me\"";
	std::string Title = "Im an item";
};

#endif