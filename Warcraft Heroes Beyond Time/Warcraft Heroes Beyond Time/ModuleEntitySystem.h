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

	void Save(pugi::xml_node&);
	void Load(const pugi::xml_node&);

	bool ClearEntitiesList();
	bool UnloadTexturesVector();

	void AddPlayer(fPoint coor, PLAYER_TYPE type);
	void AddEnemy(fPoint coor, ENEMY_TYPE type);
	void AddBoss(fPoint coor, BOSS_TYPE type);
	void AddConsumable(fPoint coor, CONSUMABLE_TYPE type);
	void AddChest(fPoint coor, CHEST_TYPE type);
	void AddStaticObject(fPoint coor, STATIC_OBJECT_TYPE type);

private:
	std::list<Entity*>		entities;
	std::list<Entity*>		toSpawn; 
	std::vector<SDL_Texture*> spritesheetsEntities;
};

#endif