#ifndef __MODULEITEMS_H__
#define __MODULEITEMS_H__

#include "Module.h"
#include <list>
#include "p2Point.h"
#include "Item.h"


class ModuleItems : public Module
{
public:
	std::list<iPoint> wcpaper;
	std::list<Item> itemsActive;

	bool CleanUp()
	{
		itemsActive.clear();
		return true;
	}
};


#endif