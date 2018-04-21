#ifndef _ITEM_CLASS_
#define _ITEM_CLASS_

#include "ModuleItems.h"
#include "p2Point.h"
#include "Application.h"

class Item
{
public:

	Item() {}
	virtual ~Item() {}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Act(ModuleItems::ItemEvent event, float dt = App->dt)
	{
		return true;
	}

	virtual bool Draw()
	{
		return true;
	}

	virtual bool printIconOnScreen(iPoint pos) 
	{
		return true;
	}
};

#endif