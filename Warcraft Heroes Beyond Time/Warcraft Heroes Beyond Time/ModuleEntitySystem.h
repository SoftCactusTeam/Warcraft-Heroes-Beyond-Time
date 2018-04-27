#ifndef  __MODULEENTITYSYSTEM_H__
#define	 __MODULEENTITYSYSTEM_H__

#include <list>
#include <vector>

#include "SDL_image\include\SDL_image.h"
#include "Module.h"
#include "p2Point.h"
#include "EntitiesEnums.h"

class Entity;
class PlayerEntity;
class ChestEntity;
class StaticEntity;
class BossEntity;
enum class BossType;

class EntitySystem : public Module
{
public:

	struct PlayerStats
	{
		uint skillMultiplier = 0;

		uint maxhp = 0u;
		float hp = 0.0f;
		uint armor = 0u;
		uint speed = 0u;
		float damage = 0.0f;
		uint energy = 0u;
		uint energyPercentbyHit = 0u;
		uint critChance = 0u;

	} thrallstats;

	struct EnemyStats
	{
		uint maxhp = 0u;
		float hp = 0.0f;
		uint speed = 0u;
		uint damage = 0u;
		uint range = 0u;
		uint dropping_chance = 0u;
		uint difficulty = 0u;

		uint def_hability_chance = 0u;
		uint strong_attack_chance = 0u;
		uint vision_area = 0u;
		float CD_between_attacks = 0.0f;

	} footmanstats, archerstats, wizardstats, darkknightstats, guldanstats;


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
	BossEntity* AddBoss(fPoint coor, BossType type);
	void AddConsumable(fPoint coor, CONSUMABLE_TYPE type);
	ChestEntity* AddChest(fPoint coor, CHEST_TYPE type);
	StaticEntity* AddStaticEntity(fPoint coor, STATIC_ENTITY_TYPE type);

	void ClearEnemies();
	int GetRandomNumber(int rang);

	void LoadCDs(float& dashCD, float& invpostdamaged, float& timepostdead)
	{
		dashCD = this->dashCD;
		invpostdamaged = this->invpostdamaged;
		timepostdead = this->timepostdead;
	}

	std::vector<SDL_Texture*> spritesheetsEntities;
	int enemiescount = 0;

	std::list<Entity*>		entities;
private:
	//std::list<Entity*>		entities;
	std::list<Entity*>		toSpawn;

	float dashCD = 0.0f;
	float invpostdamaged = 0.0f;
	float timepostdead = 0.0f;

};

#endif