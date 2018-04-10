#ifndef  __MODULEENTITYSYSTEM_H__
#define	 __MODULEENTITYSYSTEM_H__

#include <list>
#include <vector>

#include "SDL_image\include\SDL_image.h"

#include "p2Point.h"
#include "EntitiesEnums.h"

class Entity;
class PlayerEntity;
class ChestEntity;
class StaticEntity;

class EntitySystem : public Module
{
public:

	struct PlayerStats
	{
		uint maxhp = 0;
		uint hp = 0;
		uint armor = 0;
		uint speed = 0;
		uint damage = 0;
		uint energy = 100;
		uint energyPercentbyHit = 0;
		uint critChance = 0;

	} thrallstats;

	struct EnemyStats
	{
		uint maxhp = 0;
		uint hp = 0;
		uint speed = 0;
		uint damage = 0;
		uint range = 0;
		uint dropping_chance = 0;
		uint difficulty = 0;

	} footmanstats, archerstats, wizardstats, darkknightstats;


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

	void AddCommands();

	bool ClearEntitiesList();
	bool UnloadTexturesVector();

	PlayerEntity* AddPlayer(fPoint coor, PLAYER_TYPE type);
	void AddEnemy(fPoint coor, ENEMY_TYPE type);
	void AddBoss(fPoint coor, BOSS_TYPE type);
	void AddConsumable(fPoint coor, CONSUMABLE_TYPE type);
	ChestEntity* AddChest(fPoint coor, CHEST_TYPE type);
	StaticEntity* AddStaticEntity(fPoint coor, STATIC_ENTITY_TYPE type);

	void ClearEnemies();
	int GetRandomNumber(int rang);

	std::vector<SDL_Texture*> spritesheetsEntities;
private:
	std::list<Entity*>		entities;
	std::list<Entity*>		toSpawn; 

};

#endif
