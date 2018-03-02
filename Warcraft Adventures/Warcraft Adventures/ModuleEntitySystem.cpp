#include "ModuleEntitySystem.h"
#include "App.h"
#include "ModuleTextures.h"

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
	// CARREGAR TEXTURES

		//ThrallSprite = Application->textures->Load("");
		//ValeeraSprite = Application->textures->Load("");
		//SylvanasSprite = Application->textures->Load("");

		//GulDanSprite = Application->textures->Load("");
		//LichKingSprite = Application->textures->Load("");
		//IllidanSprite = Application->textures->Load("");

		//FootManSprite = Application->textures->Load("");
		//ArcherSprite = Application->textures->Load("");
		//MageSprite = Application->textures->Load("");
		//DeathKingSprite = Application->textures->Load("");
		//GoblinSprite = Application->textures->Load("");
		//ZombieSprite = Application->textures->Load("");

		//ChestSprite = Application->textures->Load("");

		//ConsumableAtacSprite = Application->textures->Load("");
		//ConsumableLiveSprite = Application->textures->Load("");
		//ConsumableMovementSpeedSprite = Application->textures->Load("");

		//StaticSceneObjectTree = Application->textures->Load("");
		//StaticSceneObjectRock = Application->textures->Load("");

	active = true;
}

bool EntitySystem::Start()
{
	ClearEntitiesList();

	for (std::list<Entity*>::iterator iterator = entities.begin(); iterator != entities.end(); iterator++) {
		(*iterator)->Start();
	}
	return true;
}

bool EntitySystem::PreUpdate()
{
	for (std::list<Entity*>::iterator iterator = entities.begin(); iterator != entities.end(); iterator++) {
		(*iterator)->Draw();
	}
	return true;
}

bool EntitySystem::Update(float dt)
{
	for (std::list<Entity*>::iterator iterator = entities.begin(); iterator != entities.end(); iterator++) {
		// AQUI S'HA DE FER EL UPDATE
	}
	return true;
}

bool EntitySystem::PostUpdate()
{
	for (std::list<Entity*>::iterator iterator = entities.begin(); iterator != entities.end(); iterator++) {
		if ((*iterator)->destroy == true) {
			delete (*iterator);
			entities.remove((*iterator));
		}
	}
	return true;
}

bool EntitySystem::CleanUp()
{
	for (std::list<Entity*>::iterator iterator = entities.begin(); iterator != entities.end(); iterator++) {
		(*iterator)->Finish();
	}
	ClearEntitiesList();
	return true;
}

void EntitySystem::ClearEntitiesList()
{
	for (std::list<Entity*>::iterator iterator = entities.begin(); iterator != entities.end(); iterator++) {
		delete ((*iterator));
		entities.pop_back();
	}
	entities.clear();
}

void EntitySystem::AddEnemie(iPoint coor, ENEMY_TYPE type)
{
	Entity* newEntity;
	switch (type) {
	case ENEMY_TYPE::FOOTMAN:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_ENEMIES, FootManSprite, type);
		break;
	case ENEMY_TYPE::ARCHER:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_ENEMIES, ArcherSprite, type);
		break;
	case ENEMY_TYPE::MAGE:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_ENEMIES, MageSprite, type);
		break;
	case ENEMY_TYPE::DEATH_KNIGHT:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_ENEMIES, DeathKingSprite, type);
		break;
	case ENEMY_TYPE::GOBLIN:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_ENEMIES, GoblinSprite, type);
		break;
	case ENEMY_TYPE::ZOMBIE:
		newEntity = new Enemy_Entity(coor, ENTITY_TYPE::DINAMIC_ENEMIES, ZombieSprite, type);
		break;
	}
	entities.push_front(newEntity);
}

void EntitySystem::AddBoss(iPoint coor, BOSS_TYPE type)
{
	Entity* newEntity;
	switch (type) {
	case BOSS_TYPE::GULDAN:
		newEntity = new Boss_Entity(coor, ENTITY_TYPE::DINAMIC_BOSS, GulDanSprite, type);
		break;
	case BOSS_TYPE::LICH_KING:
		newEntity = new Boss_Entity(coor, ENTITY_TYPE::DINAMIC_BOSS, LichKingSprite, type);
		break;
	case BOSS_TYPE::ILLIDAN:
		newEntity = new Boss_Entity(coor, ENTITY_TYPE::DINAMIC_BOSS, IllidanSprite, type);
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
	Entity* newEntity;
	switch (type) {
	case CHEST_TYPE::LOW_CHEST:
		newEntity = new Chest_Entiy(coor, ENTITY_TYPE::STATIC_CHEST, ChestSprite, type);
		break;
	case CHEST_TYPE::MID_CHEST:
		newEntity = new Chest_Entiy(coor, ENTITY_TYPE::STATIC_CHEST, ChestSprite, type);
		break;
	case CHEST_TYPE::HIGH_CHEST:
		newEntity = new Chest_Entiy(coor, ENTITY_TYPE::STATIC_CHEST, ChestSprite, type);
		break;
	}
	entities.push_front(newEntity);
}

void EntitySystem::AddStaticObject(iPoint coor, STATIC_OBJECT_TYPE type) 
{
	Entity* newEntity;
	switch (type) {
	case STATIC_OBJECT_TYPE::TREE:
		newEntity = new StaticObject_Entity(coor, ENTITY_TYPE::STATIC_ESCENE_ITEM, StaticSceneObjectTree, type);
		break;
	case STATIC_OBJECT_TYPE::ROCK:
		newEntity = new StaticObject_Entity(coor, ENTITY_TYPE::STATIC_ESCENE_ITEM, StaticSceneObjectRock, type);
		break;

	}
	entities.push_front(newEntity);
}