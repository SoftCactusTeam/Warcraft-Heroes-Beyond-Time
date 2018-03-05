#include "App.h"
#include "Log.h"
#include "ModuleEntitySystem.h"
#include "ModuleTextures.h"

#include "Entity.h"
#include "Player_Entity.h"
#include "Boss_Entity.h"
#include "Enemy_Entity.h"
#include "Consumable_Entity.h"
#include "Chest_Entity.h"
#include "StaticObject_Entity.h"

EntitySystem::EntitySystem() : Module()
{
	name = "entitySystem";
}

bool EntitySystem::Start()
{
	LOG("Starting Entities Factory module");
	
	return true;
}

bool EntitySystem::PreUpdate()
{	
	return true;
}

bool EntitySystem::Update(float dt)
{
	bool ret = true;

	for (std::list<Entity*>::iterator iterator = entities.begin(); iterator != entities.end() && ret; ++iterator)
	{
		ret = (*iterator)->Update(dt);
	}

	if (ret)
	{
		for (std::list<Entity*>::iterator iterator = entities.begin(); iterator != entities.end(); ++iterator)
		{
			ret = (*iterator)->Draw(dt);
		}
	}
	return ret;
}

bool EntitySystem::PostUpdate()
{
	for (std::list<Entity*>::iterator iterator = entities.begin(); iterator != entities.end(); iterator++)
		if ((*iterator)->destroy == true)
			entities.remove((*iterator));

	return true;
}

bool EntitySystem::CleanUp()
{
	bool ret = true;

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end() && ret; ++it)
		ret = (*it)->Finish();
	
	if (ret)
		ret = ClearEntitiesList();

	return ret;
}

bool EntitySystem::ClearEntitiesList()
{
	bool ret = true;

	for (std::list<Entity*>::iterator it = entities.begin(); it != entities.end() && ret; ++it)
	{
		delete *it;
	}

	entities.clear();

	return entities.size() <= 0;
}

void EntitySystem::AddEnemy(iPoint coor, ENEMY_TYPE type)
{
	Entity* newEntity;
	switch (type) {
	case ENEMY_TYPE::FOOTMAN:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_ENEMIES, nullptr, type);
		break;
	case ENEMY_TYPE::ARCHER:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_ENEMIES, nullptr, type);
		break;
	case ENEMY_TYPE::MAGE:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_ENEMIES, nullptr, type);
		break;
	case ENEMY_TYPE::DEATH_KNIGHT:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_ENEMIES, nullptr, type);
		break;
	case ENEMY_TYPE::GOBLIN:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_ENEMIES, nullptr, type);
		break;
	case ENEMY_TYPE::ZOMBIE:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_ENEMIES, nullptr, type);
		break;
	}
	entities.push_front(newEntity);
}

void EntitySystem::AddBoss(iPoint coor, BOSS_TYPE type)
{
	Entity* newEntity;
	switch (type) {
	case BOSS_TYPE::GULDAN:
		newEntity = new Boss_Entity(coor, ENTITY_TYPE::DINAMIC_BOSS, nullptr, type);
		break;
	case BOSS_TYPE::LICH_KING:
		newEntity = new Boss_Entity(coor, ENTITY_TYPE::DINAMIC_BOSS, nullptr, type);
		break;
	case BOSS_TYPE::ILLIDAN:
		newEntity = new Boss_Entity(coor, ENTITY_TYPE::DINAMIC_BOSS, nullptr, type);
		break;
	}
	entities.push_front(newEntity);
}

void EntitySystem::AddPlayer(iPoint coor, PLAYER_TYPE type)
{
	Entity* newEntity;
	switch (type)
	{
	case PLAYER_TYPE::THRALL:
		newEntity = new PlayerPJ_Entity(coor, ENTITY_TYPE::DINAMIC_PLAYER, nullptr, type);
		break;
	case PLAYER_TYPE::VALEERA:
		newEntity = new PlayerPJ_Entity(coor, ENTITY_TYPE::DINAMIC_PLAYER, nullptr, type);
		break;
	case PLAYER_TYPE::SYLVANAS:
		newEntity = new PlayerPJ_Entity(coor, ENTITY_TYPE::DINAMIC_PLAYER, nullptr, type);
		break;
	}
	entities.push_front(newEntity);
}

void EntitySystem::AddConsumable(iPoint coor, CONSUMABLE_TYPE type)
{
	Entity* newEntity;
	switch (type) {
	case CONSUMABLE_TYPE::ATACK_POTION:
		newEntity = new Consumable_Entity(coor, ENTITY_TYPE::STATIC_CONSUMABLE, nullptr, type);
		break;
	case CONSUMABLE_TYPE::LIVE_POTION:
		newEntity = new Consumable_Entity(coor, ENTITY_TYPE::STATIC_CONSUMABLE, nullptr, type);
		break;
	case CONSUMABLE_TYPE::MOVEMENT_SPEED_POTION:
		newEntity = new Consumable_Entity(coor, ENTITY_TYPE::STATIC_CONSUMABLE, nullptr, type);
		break;
	}
	entities.push_front(newEntity);
}

void EntitySystem::AddChest(iPoint coor, CHEST_TYPE type) 
{
	Entity* newEntity;
	switch (type) {
	case CHEST_TYPE::LOW_CHEST:
		newEntity = new Chest_Entiy(coor, ENTITY_TYPE::STATIC_CHEST, nullptr, type);
		break;
	case CHEST_TYPE::MID_CHEST:
		newEntity = new Chest_Entiy(coor, ENTITY_TYPE::STATIC_CHEST, nullptr, type);
		break;
	case CHEST_TYPE::HIGH_CHEST:
		newEntity = new Chest_Entiy(coor, ENTITY_TYPE::STATIC_CHEST, nullptr, type);
		break;
	}
	entities.push_front(newEntity);
}

void EntitySystem::AddStaticObject(iPoint coor, STATIC_OBJECT_TYPE type) 
{
	Entity* newEntity;
	switch (type) 
	{
	case STATIC_OBJECT_TYPE::TREE:
		newEntity = new StaticObject_Entity(coor, ENTITY_TYPE::STATIC_ESCENE_ITEM, nullptr, type);
		break;
	case STATIC_OBJECT_TYPE::ROCK:
		newEntity = new StaticObject_Entity(coor, ENTITY_TYPE::STATIC_ESCENE_ITEM, nullptr, type);
		break;
	}
	entities.push_front(newEntity);
}