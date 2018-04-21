#include "ModuleItems.h"
#include "WCItem.h"

bool ModuleItems::Awake(pugi::xml_node& itemsNode)
{
	//Read stats from config.xml

	//Load Items' pull
	loadItemsPull();

	return true;
}

bool ModuleItems::Start()
{
	return true;
}


bool ModuleItems::Update(float dt)
{
	bool ret = true;

	std::list<Item*>::iterator it;
	for (it = equipedItems.begin(); it != equipedItems.end() && ret; ++it)
	{
		ret = (*it)->Act(ItemEvent::UPDATE, dt);
	}

	return ret;
}

bool ModuleItems::PostUpdate()
{
	bool ret = true;

	std::list<Item*>::iterator it;
	for (it = equipedItems.begin(); it != equipedItems.end() && ret; ++it)
	{
		ret = (*it)->Draw();
	}

	return ret;
}

bool ModuleItems::CleanUp()
{
	std::vector<Item*>::iterator pullIt;
	for (pullIt = availableItems.begin(); pullIt != availableItems.end(); ++pullIt)
	{
		delete(*pullIt);
	}
	availableItems.clear();

	std::list<Item*>::iterator equipedIt;
	for (equipedIt = equipedItems.begin(); equipedIt != equipedItems.end(); ++equipedIt)
	{
		delete(*equipedIt);
	}
	equipedItems.clear();

	return true;
}

void ModuleItems::loadItemsPull()
{
	WCItem* wc = new WCItem();
	availableItems.push_back(wc);

	//etc

}

bool ModuleItems::equipItem(Item* item)
{
	bool ret = false;
	std::vector<Item*>::iterator pullIt;
	for (pullIt = availableItems.begin(); pullIt != availableItems.end(); ++pullIt)
	{
		if (*pullIt == item)
		{
			equipedItems.push_back(item);
			availableItems.erase(pullIt);
			item->Start();
			ret = true;
			break;
		}
	}
	return ret;
}





