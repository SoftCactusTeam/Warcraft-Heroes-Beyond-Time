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

		bool isEmpty()
		{
			return maxhp == 0;
		}

	} thrallstats;

	struct EnemyStats
	{
		uint maxhp = 0u;
		float hp = 0.0f;
		uint speed = 0u;
		float damage = 0.0f;
		uint dropping_chance = 0u;
		uint arrows_speed = 0u;
		float arrows_life = 0.0f; //In seconds

		uint strong_attack_chance = 0u;
		float time_between_attacks = 0.0f;
		float time_stunned_after_attack = 0.0f;

		uint vision_range = 0u;
		uint attack_range = 0u;
		uint minimal_distance_player = 0u;

		uint preAttac = 0u;
		uint tilesToLittleMove = 0u;
		uint DistanceToScape = 0u;

		uint attacCone_probability = 0u;
		uint attacFast_probability = 0u;

		uint timeStunedAfterHit = 0u;
		uint velocityDashHit = 0u;
		uint timingDashHit = 0u;

	} archerT1stats, archerT2stats, archerT3stats, guldanstats;


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

	PlayerEntity* AddPlayer(fPoint coor, PLAYER_TYPE type, PlayerStats& stats = PlayerStats());
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

	/*to check*/

public:
	bool checkEntityNearOther(Entity* entity, int dist);

};

#endif