#ifndef _LEXRO_ITEM_
#define _LEXRO_ITEM_


#include "Item.h"

#define ENERGY_ITEM {1,183,31,25}

class EnergyItem :	public Item
{
public:
	EnergyItem();
	~EnergyItem();

	bool Start();
	bool Act(ModuleItems::ItemEvent event, float dt = App->dt);
	bool Draw();
	bool printYourStuff(iPoint pos);

private:
	std::string softDescription = "\"You will enjoy the pain\"";
	std::string Title = "Surprise bag";
};

#endif 
