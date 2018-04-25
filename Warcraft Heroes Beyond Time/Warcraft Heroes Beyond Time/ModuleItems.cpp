#include "ModuleItems.h"
#include "WCItem.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "DMGBallItem.h"
#include "FEARBallItem.h"
#include <time.h>

bool ModuleItems::Awake(pugi::xml_node& itemsNode)
{
	//Read stats from config.xml

	//Load Items' pull
	loadItemsPull();

	return true;
}

bool ModuleItems::Start()
{
	itemsTexture = App->textures->Load("sprites/all_items.png");

	FEARBallItem* Fearball_Item = new FEARBallItem();
	equipedItems.push_back(Fearball_Item);
	Fearball_Item->Start();

	DMGBallItem* DMGball_Item = new DMGBallItem();
	equipedItems.push_back(DMGball_Item);
	DMGball_Item->Start();

	WCItem* test_item = new WCItem();
	equipedItems.push_back(test_item);
	test_item->Start();

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

	App->textures->UnLoad(itemsTexture);

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

bool ModuleItems::getThreeRandomItems(Item** items)
{
	if (availableItems.size() < 1)
		return false;

	int id_1 = rand() % availableItems.size();
	int id_2, id_3 = 0;

	items[0] = availableItems[id_1];

	if (availableItems.size() < 2)
		return false;

	do
	{
		id_2 = rand() % availableItems.size();

	} while (id_2 == id_1);

	items[1] = availableItems[id_2];

	if (availableItems.size() < 3)
		return false;

	do
	{
		id_3 = rand() % availableItems.size();

	} while (id_3 == id_1 || id_3 == id_2);

	items[2] = availableItems[id_3];

	return true;
}
