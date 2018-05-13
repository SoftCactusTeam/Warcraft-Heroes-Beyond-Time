#ifndef _WC_ITEM_
#define _WC_ITEM_

#include "Item.h"
#include "p2Point.h"
#include "ModuleColliders.h"

#define WC_ICON {34,84,27,31} //The rect from the texture of your gui icon
#define SHIT_ITEM {102,19,31,29}

class Shit
{
public:
	Shit(Collider* temp, iPoint pos);
	~Shit();
	iPoint pos;
private:
	Collider* paper_collider;
	
};

class WCItem : public Item
{
public:
	WCItem() {}
	virtual ~WCItem();

	bool Start();
	bool Act(ModuleItems::ItemEvent event, float dt = App->dt);
	bool Draw();
	bool printYourStuff(iPoint pos);

	float time = 0;
	int cont = 0;

	std::list<Shit*> shit_list;

private:
	std::string softDescription;
	std::string hardDescription;
};

#endif