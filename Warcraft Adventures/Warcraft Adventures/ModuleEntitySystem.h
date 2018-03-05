#ifndef  __MODULEENTITYSYSTEM_H__
#define	 __MODULEENTITYSYSTEM_H__

#include <list>

#include "SDL_image\include\SDL_image.h"

#include "p2Point.h"

class PlayerPJ_Entity;
class Boss;
class Enemy;

class Consumable;
class Chest;
class StaticObject;

class EntitySystem : public Module
{
public:

	EntitySystem();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void AddPlayer(iPoint coor, PLAYER_TYPE type);
	void AddEnemy(iPoint coor, ENEMY_TYPE type);
	void AddBoss(iPoint coor, BOSS_TYPE type);
	void AddConsumable(iPoint coor, CONSUMABLE_TYPE type);
	void AddChest(iPoint coor, CHEST_TYPE type);
	void AddStaticObject(iPoint coor, STATIC_OBJECT_TYPE type);

	bool ClearEntitiesList();

public:
	std::string				name;
	bool					active = true;

private:
	std::list<Entity*>		entities;
	std::list<Entity*>		toSpawn;
};

#endif