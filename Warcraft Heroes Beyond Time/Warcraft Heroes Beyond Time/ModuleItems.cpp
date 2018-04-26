#include "ModuleItems.h"
#include "WCItem.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "DMGBallItem.h"
#include <time.h>

float ModuleItems::dmgBallDamage = 0.0f;
float ModuleItems::seconds_fear_ball = 0.0f;
float ModuleItems::seconds_frozen_ball = 0.0f;
float ModuleItems::slow_percent_shit = 0.0f;
float ModuleItems::time_slowed_shit = 0.0f;

bool ModuleItems::Awake(pugi::xml_node& itemsNode)
{
	//Read stats from config.xml
	dmgBallDamage = itemsNode.child("dmgBall").attribute("damage").as_float();
	seconds_fear_ball = itemsNode.child("fearBall").attribute("seconds_fear").as_float();
	seconds_frozen_ball = itemsNode.child("frozenBall").attribute("seconds_frozen").as_float();
	time_slowed_shit = itemsNode.child("slowShit").attribute("time_slowed").as_float();
	slow_percent_shit = itemsNode.child("slowShit").attribute("slow_percent").as_float();

	return true;
}

bool ModuleItems::Start()
{
	//Load Items' pull
	loadItemsPull();

	itemsTexture = App->textures->Load("sprites/all_items.png");

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

	DMGBallItem* dmgBall = new DMGBallItem();
	availableItems.push_back(dmgBall);


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
