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
		PLAYER_DIED
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

	bool getThreeRandomItems(Item** items);

	//Returns false in failure or if the item was already equiped
	bool equipItem(Item* item);


	void loadItemsPull();

	SDL_Texture* getItemsTexture() const
	{
		return itemsTexture;
	}

public:

	static float dmgBallDamage;
	static float seconds_fear_ball;
	static float seconds_frozen_ball;
	static float time_slowed_shit;
	static float slow_percent_shit;

private:
	
	std::vector<Item*> availableItems;
	std::list<Item*> equipedItems;
	SDL_Texture* itemsTexture = nullptr;

	
};


#endif