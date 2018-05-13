#ifndef _SHEEP_HEAD_
#define _SHEEP_HEAD_

#include "Item.h"
#include "p2Point.h"
#include "ModuleColliders.h"
#include <string>

#define SHEEP_ICON {1,51,31,31} //The rect from the texture of your gui icon
#define ARROW_SLOW_ITEM {99,87,31,29}


class Arrow_Slow_Shit
{
public:
	Arrow_Slow_Shit(Collider* temp, iPoint pos);
	~Arrow_Slow_Shit();
	iPoint pos;
private:
	Collider* arrow_slow_collider;
};

class ArrowSlowItem :
	public Item
{
public:
	ArrowSlowItem();
	~ArrowSlowItem();


	bool Start();
	bool Act(ModuleItems::ItemEvent event, float dt = App->dt);
	bool Draw();
	bool printYourStuff(iPoint pos);

	float time = 0;
	int cont = 0;

	std::list<Arrow_Slow_Shit*> arrow_slow_shit_list;
private:
	std::string softDescription = "\"Air desity levels rising\"";
	std::string Title = "Sheep's Head";
};

#endif