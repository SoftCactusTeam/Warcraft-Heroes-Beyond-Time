#include "ModuleItems.h"
#include "WCItem.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "DMGBallItem.h"
#include "OneShotItem.h"
#include "AngelsGuardItem.h"

#include "FreezeBallItem.h"
#include "FEARBallItem.h"
#include "ArrowSlowItem.h"
#include "RingItem.h"
#include "LifeStealItem.h"
#include "EnergyItem.h"
#include "HolyShitItem.h"
#include "ShieldBallItem.h"

#include <time.h>

float ModuleItems::dmgBallDamage = 0.0f;
float ModuleItems::fearBallSeconds = 0.0f;
float ModuleItems::fearBallChance = 0.0f;
float ModuleItems::frozenBallSeconds = 0.0f;
float ModuleItems::frozenBallChance = 0.0f;
float ModuleItems::slowShitPercent = 0.0f;
float ModuleItems::slowShitSeconds = 0.0f;
float ModuleItems::dmgShitDamage = 0.0f;
float ModuleItems::stealhp = 0.0f;
float ModuleItems::energywhenHitted = 0.0f;
float ModuleItems::HolyShitExtraDamage = 0.0f;
float ModuleItems::HolyShitExtraHP = 0.0f;


bool ModuleItems::Awake(pugi::xml_node& itemsNode)
{
	//Read stats from config.xml
	dmgBallDamage = itemsNode.child("dmgBall").attribute("damage").as_float();
	fearBallSeconds = itemsNode.child("fearBall").attribute("seconds_fear").as_float();
	fearBallChance = itemsNode.child("fearBall").attribute("chance").as_float();
	frozenBallSeconds = itemsNode.child("frozenBall").attribute("seconds_frozen").as_float();
	frozenBallChance = itemsNode.child("frozenBall").attribute("chance").as_float();
	slowShitSeconds = itemsNode.child("slowShit").attribute("time_slowed").as_float();
	slowShitPercent = itemsNode.child("slowShit").attribute("slow_percent").as_float();
	dmgShitDamage = itemsNode.child("dmgShit").attribute("damage_by_dt").as_float();
	stealhp = itemsNode.child("StealLife").attribute("amount").as_float();
	energywhenHitted = itemsNode.child("Lexro").attribute("EnergyWhenHitted").as_uint();
	HolyShitExtraDamage = itemsNode.child("HolyShit").attribute("extraDamage").as_float();
	HolyShitExtraHP = itemsNode.child("HolyShit").attribute("extraHP").as_float();


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

void ModuleItems::newEvent(ModuleItems::ItemEvent event)
{
	std::list<Item*>::iterator it;
	for (it = equipedItems.begin(); it != equipedItems.end(); ++it)
	{
		(*it)->Act(event);
	}
}

bool ModuleItems::PostUpdate()
{
	bool ret = true;

	std::list<Item*>::iterator it;
	for (it = equipedItems.begin(); it != equipedItems.end() && ret; ++it)
	{
		ret = (*it)->Draw();
	}

	DestroyItems();

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

	FreezeBallItem* FreezeBall_Item = new FreezeBallItem();
	availableItems.push_back(FreezeBall_Item);

	FEARBallItem* Fearball_Item = new FEARBallItem();
	availableItems.push_back(Fearball_Item);


	ArrowSlowItem* projectileslowitem = new ArrowSlowItem();
	availableItems.push_back(projectileslowitem);

	RingItem* enemiesSlowItem = new RingItem();
	availableItems.push_back(enemiesSlowItem);

	LifeStealItem* lifeStealItem = new LifeStealItem();
	availableItems.push_back(lifeStealItem);

	EnergyItem* energyitem = new EnergyItem();
	availableItems.push_back(energyitem);

	HolyShitItem* holyshit = new HolyShitItem();
	availableItems.push_back(holyshit);

	ShieldBallItem* shield_ball = new ShieldBallItem();
	availableItems.push_back(shield_ball);

	OneShotItem* One_Shot_Item = new OneShotItem();
	availableItems.push_back(One_Shot_Item);

	AngelsGuardItem* Angels_Guard_Item = new AngelsGuardItem();
	availableItems.push_back(Angels_Guard_Item);
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

bool ModuleItems::unequipItem(Item* item)
{
	itemsToUnEquip.push_back(item);
	return true;
}

void ModuleItems::DestroyItems()
{
	std::list<Item*>::iterator it;
	
	for(it = itemsToUnEquip.begin(); it != itemsToUnEquip.end(); ++it)
	{
		equipedItems.remove(*it);
		delete *it;
	}
	itemsToUnEquip.clear();
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
