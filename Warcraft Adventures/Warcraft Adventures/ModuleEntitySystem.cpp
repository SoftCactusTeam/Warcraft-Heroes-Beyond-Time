#include "ModuleEntitySystem.h"

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

void EntitySystem::Init()
{
	active = true;
}

bool EntitySystem::Start()
{
	ClearEntitiesList();
	return true;
}

bool EntitySystem::PreUpdate()
{
	return true;
}

bool EntitySystem::Update(float dt)
{
	return true;
}

bool EntitySystem::PostUpdate()
{
	return true;
}

bool EntitySystem::CleanUp()
{
	ClearEntitiesList();
	return true;
}

void EntitySystem::ClearEntitiesList()
{
	for (std::list<Entity*>::iterator iterador = entities.begin(); iterador != entities.end(); iterador++) {
		entities.pop_back();
	}
	entities.clear();
}

void EntitySystem::AddEnemie(iPoint coor, ENEMY_TYPE type)
{
	Entity* newEntity;
	switch (type) {
	case ENEMY_TYPE::FOOTMAN:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_PLAYER, FootManSprite, type);
		break;
	case ENEMY_TYPE::ARCHER:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_PLAYER, ArcherSprite, type);
		break;
	case ENEMY_TYPE::MAGE:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_PLAYER, MageSprite, type);
		break;
	case ENEMY_TYPE::DEATH_KNIGHT:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_PLAYER, DeathKingSprite, type);
		break;
	case ENEMY_TYPE::GOBLIN:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_PLAYER, GoblinSprite, type);
		break;
	case ENEMY_TYPE::ZOMBIE:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_PLAYER, ZombieSprite, type);
		break;
	}
	entities.push_front(newEntity);
}

void EntitySystem::AddBoss(iPoint coor, BOSS_TYPE type)
{
	Entity* newEntity;
	switch (type) {
	case BOSS_TYPE::GULDAN:
		newEntity = new Boss_Entity(coor, ENTITY_TYPE::DINAMIC_PLAYER, GulDanSprite, type);
		break;
	case BOSS_TYPE::LICH_KING:
		newEntity = new Boss_Entity(coor, ENTITY_TYPE::DINAMIC_PLAYER, LichKingSprite, type);
		break;
	case BOSS_TYPE::ILLIDAN:
		newEntity = new Boss_Entity(coor, ENTITY_TYPE::DINAMIC_PLAYER, IllidanSprite, type);
		break;
	}
	entities.push_front(newEntity);
}

void EntitySystem::AddPlayer(iPoint coor, PLAYER_TYPE type)
{
	Entity* newEntity;
	switch (type) {
	case PLAYER_TYPE::THRALL:
		newEntity = new PlayerPJ_Entity(coor, ENTITY_TYPE::DINAMIC_PLAYER, ThrallSprite, type);
		break;
	case PLAYER_TYPE::VALEERA:
		newEntity = new PlayerPJ_Entity(coor, ENTITY_TYPE::DINAMIC_PLAYER, ValeeraSprite, type);
		break;
	case PLAYER_TYPE::SYLVANAS:
		newEntity = new PlayerPJ_Entity(coor, ENTITY_TYPE::DINAMIC_PLAYER, SylvanasSprite, type);
		break;
	}
	entities.push_front(newEntity);
}

void EntitySystem::AddConsumable(iPoint coor, CONSUMABLE_TYPE type)
{
	Entity* newEntity;
	switch (type) {
	case CONSUMABLE_TYPE::ATACK_POTION:
		newEntity = new Consumable_Entity(coor, ENTITY_TYPE::STATIC_CONSUMABLE, ConsumableAtacSprite, type);
		break;
	case CONSUMABLE_TYPE::LIVE_POTION:
		newEntity = new Consumable_Entity(coor, ENTITY_TYPE::STATIC_CONSUMABLE, ConsumableLiveSprite, type);
		break;
	case CONSUMABLE_TYPE::MOVEMENT_SPEED_POTION:
		newEntity = new Consumable_Entity(coor, ENTITY_TYPE::STATIC_CONSUMABLE, ConsumableMovementSpeedSprite, type);
		break;
	}
	entities.push_front(newEntity);
}

void EntitySystem::AddChest(iPoint coor, CHEST_TYPE type) 
{

}

void EntitySystem::AddStaticObject(iPoint coor, STATIC_OBJECT_TYPE type) 
{

}