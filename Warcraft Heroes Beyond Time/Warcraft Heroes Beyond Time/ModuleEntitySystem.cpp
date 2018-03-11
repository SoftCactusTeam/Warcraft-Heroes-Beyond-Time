#include "App.h"
#include "Log.h"
#include "ModuleEntitySystem.h"
#include "ModuleTextures.h"

#include "Entity.h"
#include "PlayerEntity.h"
#include "Thrall.h"
#include "BossEntity.h"
#include "EnemyEntity.h"
#include "ConsumableEntity.h"
#include "ChestEntity.h"
#include "StaticObjectEntity.h"

EntitySystem::EntitySystem() : Module()
{
	name = "entitySystem";
}

bool EntitySystem::Start()
{
	LOG("Loading textures");
	spritesheetsEntities.push_back(Application->textures->Load("thrall_spritesheet.png"));
	
	return true;
}

bool EntitySystem::PreUpdate()
{	
	if (toSpawn.size() >= 0)
	{
		for(std::list<Entity*>::iterator it = toSpawn.begin(); it != toSpawn.end(); ++it)
		{
			(*it)->Start();
			entities.push_back(*it);
		}
		toSpawn.clear();
	}

	return toSpawn.size() <= 0;
}

bool EntitySystem::Update(float dt)
{
	bool ret = true;

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end() && ret; ++it)
	{
		ret = (*it)->Update(dt);
	}

	if (ret)
	{
		for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		{
			ret = (*it)->Draw(dt);
		}
	}
	return ret;
}

bool EntitySystem::PostUpdate()
{
	bool ret = true;

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		if ((*it)->destroy)
			entities.remove((*it));
	}

	return ret;
}

bool EntitySystem::CleanUp()
{
	bool ret = true;

	LOG("Entity System: CleanUp...");

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end() && ret; ++it)
		ret = (*it)->Finish();
	
	if (ret)
		ret = ClearEntitiesList();

	if (ret)
		ret = UnloadTexturesVector();

	return ret;
}

bool EntitySystem::ClearEntitiesList()
{
	bool ret = true;

	LOG("Entity System: Releasing entities...");

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end() && ret; ++it)
	{
		delete *it;
	}

	entities.clear();

	return entities.size() <= 0;
}

bool EntitySystem::UnloadTexturesVector()
{
	bool ret = true;

	LOG("Entity System: Unloading textures...");

	for (int i = 0; i < spritesheetsEntities.size() && ret; ++i)
	{
		ret = Application->textures->UnLoad(spritesheetsEntities[i]);
	}

	if (ret)
		spritesheetsEntities.clear();

	return spritesheetsEntities.size() <= 0;
}

void EntitySystem::AddEnemy(iPoint coor, ENEMY_TYPE type)
{
	EnemyEntity* newEntity = nullptr;
	switch (type) {
	case ENEMY_TYPE::FOOTMAN:
		newEntity = new EnemyEntity(coor, ENEMY_TYPE::FOOTMAN, nullptr);
		break;
	case ENEMY_TYPE::ARCHER:
		newEntity = new EnemyEntity(coor, ENEMY_TYPE::ARCHER, nullptr);
		break;
	case ENEMY_TYPE::MAGE:
		newEntity = new EnemyEntity(coor, ENEMY_TYPE::MAGE, nullptr);
		break;
	case ENEMY_TYPE::DEATH_KNIGHT:
		newEntity = new EnemyEntity(coor, ENEMY_TYPE::DEATH_KNIGHT, nullptr);
		break;
	case ENEMY_TYPE::GOBLIN:
		newEntity = new EnemyEntity(coor, ENEMY_TYPE::GOBLIN, nullptr);
		break;
	case ENEMY_TYPE::SKELETON:
		newEntity = new EnemyEntity(coor, ENEMY_TYPE::SKELETON, nullptr);
		break;
	}
	toSpawn.push_back((Entity*)newEntity);
}

void EntitySystem::AddBoss(iPoint coor, BOSS_TYPE type)
{
	BossEntity* newEntity = nullptr;
	switch (type) {
	case BOSS_TYPE::GULDAN:
		newEntity = new BossEntity(coor, BOSS_TYPE::GULDAN, nullptr);
		break;
	case BOSS_TYPE::LICH_KING:
		newEntity = new BossEntity(coor, BOSS_TYPE::LICH_KING, nullptr);
		break;
	case BOSS_TYPE::ILLIDAN:
		newEntity = new BossEntity(coor, BOSS_TYPE::ILLIDAN, nullptr);
		break;
	}
	toSpawn.push_back((Entity*)newEntity);
}

void EntitySystem::AddPlayer(iPoint coor, PLAYER_TYPE type)
{
	PlayerEntity* newEntity = nullptr;
	switch (type)
	{
	case PLAYER_TYPE::THRALL:
		newEntity = new Thrall(coor, PLAYER_TYPE::THRALL, spritesheetsEntities[THRALLSHEET]);
		break;
	case PLAYER_TYPE::VALEERA:
		newEntity = new PlayerEntity(coor, PLAYER_TYPE::VALEERA, nullptr);
		break;
	case PLAYER_TYPE::SYLVANAS:
		newEntity = new PlayerEntity(coor, PLAYER_TYPE::SYLVANAS, nullptr);
		break;
	}
	toSpawn.push_back(newEntity);
}

void EntitySystem::AddConsumable(iPoint coor, CONSUMABLE_TYPE type)
{
	ConsumableEntity* newEntity = nullptr;
	switch (type) {
	case CONSUMABLE_TYPE::ATACK_POTION:
		newEntity = new ConsumableEntity(coor, CONSUMABLE_TYPE::ATACK_POTION, nullptr);
		break;
	case CONSUMABLE_TYPE::LIVE_POTION:
		newEntity = new ConsumableEntity(coor, CONSUMABLE_TYPE::LIVE_POTION, nullptr);
		break;
	case CONSUMABLE_TYPE::MOVEMENT_SPEED_POTION:
		newEntity = new ConsumableEntity(coor, CONSUMABLE_TYPE::MOVEMENT_SPEED_POTION, nullptr);
		break;
	}
	toSpawn.push_back((Entity*)newEntity);
}

void EntitySystem::AddChest(iPoint coor, CHEST_TYPE type) 
{
	ChestEntiy* newEntity = nullptr;
	switch (type) {
	case CHEST_TYPE::LOW_CHEST:
		newEntity = new ChestEntiy(coor, CHEST_TYPE::LOW_CHEST, nullptr);
		break;
	case CHEST_TYPE::MID_CHEST:
		newEntity = new ChestEntiy(coor, CHEST_TYPE::MID_CHEST, nullptr);
		break;
	case CHEST_TYPE::HIGH_CHEST:
		newEntity = new ChestEntiy(coor, CHEST_TYPE::HIGH_CHEST, nullptr);
		break;
	}
	toSpawn.push_back((Entity*)newEntity);
}

void EntitySystem::AddStaticObject(iPoint coor, STATIC_OBJECT_TYPE type) 
{
	StaticObjectEntity* newEntity = nullptr;
	switch (type) 
	{
	case STATIC_OBJECT_TYPE::TREE:
		newEntity = new StaticObjectEntity(coor, STATIC_OBJECT_TYPE::TREE, nullptr);
		break;
	case STATIC_OBJECT_TYPE::ROCK:
		newEntity = new StaticObjectEntity(coor, STATIC_OBJECT_TYPE::ROCK, nullptr);
		break;
	}
	toSpawn.push_back((Entity*)newEntity);
}