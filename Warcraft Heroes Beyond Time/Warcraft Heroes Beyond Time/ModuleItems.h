#ifndef __MODULEITEMS_H__
#define __MODULEITEMS_H__

#include "Module.h"
#include <list>
#include <vector>

class Item;
struct SDL_Texture;

class ModuleItems : public Module
{
public:

	enum class ItemEvent
	{
		NO_EVENT,
		UPDATE,
		PLAYER_DIED,

		PLAYER_HIT,

		PLAYER_HITTED
	};

	ModuleItems()
	{
		name = "items";
	}
	virtual ~ModuleItems() {}

	void Init()
	{
		active = false;
	}

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	virtual void Save(pugi::xml_node&);
	virtual void Load(const pugi::xml_node&);

	void DestroyItems();

	bool getThreeRandomItems(Item** items);

	//Returns false in failure or if the item was already equiped
	bool equipItem(Item* item);
	bool unequipItem(Item* item);

	SDL_Texture* getItemsTexture() const
	{
		return itemsTexture;
	}

	bool isPoolEmpty()
	{
		return availableItems.empty();
	}

	void newEvent(ItemEvent event);

private:
	void loadItemsPull();

public:

	static float dmgBallDamage;
	static float dmgShitDamage;
	static float fearBallSeconds;
	static float fearBallChance;
	static float frozenBallSeconds;
	static float frozenBallChance;
	static float slowShitSeconds;
	static float slowShitPercent;
	static float stealhp;
	static float energywhenHitted;
	static float HolyShitExtraHP;
	static float HolyShitExtraDamage;

private:

	std::vector<Item*> availableItems;
	std::list<Item*> equipedItems;
	std::list<Item*> itemsToUnEquip;
	SDL_Texture* itemsTexture = nullptr;


};


#endif
