#ifndef  __MODULEENTITYSYSTEM_H__
#define	 __MODULEENTITYSYSTEM_H__

#include <list>
#include <vector>

#include "SDL_image\include\SDL_image.h"

#include "p2Point.h"
#include "EntitiesEnums.h"

class Entity;

class EntitySystem : public Module
{
public:

	EntitySystem();

	void Init();
	bool Awake(pugi::xml_node&);


	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool ClearEntitiesList();
	bool UnloadTexturesVector();

	void AddPlayer(iPoint coor, PLAYER_TYPE type);
	void AddEnemy(iPoint coor, ENEMY_TYPE type);
	void AddBoss(iPoint coor, BOSS_TYPE type);
	void AddConsumable(iPoint coor, CONSUMABLE_TYPE type);
	void AddChest(iPoint coor, CHEST_TYPE type);
	void AddStaticObject(iPoint coor, STATIC_OBJECT_TYPE type);

public:
	std::string				name;

private:
	std::list<Entity*>		entities;
	std::list<Entity*>		toSpawn; 
	std::vector<SDL_Texture*> spritesheetsEntities;
};

#endif