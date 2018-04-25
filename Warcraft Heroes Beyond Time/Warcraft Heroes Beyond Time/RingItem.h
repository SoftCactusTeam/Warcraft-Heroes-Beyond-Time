#ifndef _SOROVOLO_GAUNT_
#define _SOROVOLO_GAUNT_

#include "Item.h"
#include "p2Point.h"
#include "ModuleColliders.h"
#include "Item.h"

#define RINT_ICON {34,84,27,31} //The rect from the texture of your gui icon
#define RING_ITEM {102,19,31,29}


class Slow_Shit
{
public:
	Slow_Shit(Collider* temp, iPoint pos);
	~Slow_Shit();
	iPoint pos;
private:
	Collider * paper_collider;

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
	bool printIconOnScreen(iPoint pos);

	float time = 0;
	int cont = 0;

	std::list<Slow_Shit*> low_shit_list;
};

#endif