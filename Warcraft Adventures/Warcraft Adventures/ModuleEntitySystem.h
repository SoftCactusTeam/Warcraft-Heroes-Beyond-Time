#ifndef  __ModuleEntitySystem_H__
#define	 __ModuleEntitySystem_H__

#include <list>

#include "App.h"
#include "p2Point.h"
#include "Entity.h"

class EntitySystem
{
public:

	EntitySystem()
	{}

	void Init();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void ClearEntitiesList();
	void AddEnemie(iPoint coor, ENEMY_TYPE type);
	void AddBoss(iPoint coor, BOSS_TYPE type);
	void AddPlayer(iPoint coor, PLAYER_TYPE type);
	void AddItem(iPoint coor, CONSUMABLE_TYPE type);
	void AddChest(iPoint coor, CHEST_TYPE type);
	void AddStaticObject(iPoint coor, STATIC_OBJECT_TYPE type);

public:
	std::string	name;
	bool		active = true;;
	bool		pauseAllEntities = false;

private:
	std::list<Entity> entities;

};

#endif