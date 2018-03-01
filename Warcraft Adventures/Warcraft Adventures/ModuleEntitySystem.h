#ifndef  __ModuleEntitySystem_H__
#define	 __ModuleEntitySystem_H__

#include <list>

#include "App.h"
#include "p2Point.h"
#include "Entity.h"

class PlayerPJ;
class Boss;
class Enemy;

class Consumable;
class Chest;
class StaticObject;

class EntitySystem : public Module
{
public:

	EntitySystem();

	void Init();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void AddPlayer(iPoint coor, PLAYER_TYPE type);
	void AddEnemie(iPoint coor, ENEMY_TYPE type);
	void AddBoss(iPoint coor, BOSS_TYPE type);
	void AddConsumable(iPoint coor, CONSUMABLE_TYPE type);
	void AddChest(iPoint coor, CHEST_TYPE type);
	void AddStaticObject(iPoint coor, STATIC_OBJECT_TYPE type);

	void ClearEntitiesList();

public:
	std::string	name;
	bool		active = true;;
	bool		pauseAllEntities = false;

private:
	std::list<Entity*> entities;

};

#endif