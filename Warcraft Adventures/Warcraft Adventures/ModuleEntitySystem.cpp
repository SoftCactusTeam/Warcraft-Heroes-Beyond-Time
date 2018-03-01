#include "ModuleEntitySystem.h"
#include "Player_Entity.h"

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

}

void EntitySystem::AddBoss(iPoint coor, BOSS_TYPE type)
{

}

void EntitySystem::AddPlayer(iPoint coor, PLAYER_TYPE type)
{
	SDL_Texture* texture = nullptr;
	Entity* pj = new PlayerPJ(coor, ENTITY_TYPE::DINAMIC_PLAYER, texture, type);
	switch (type) {
	case THRALL:
		pj = new PlayerPJ(coor, ENTITY_TYPE::DINAMIC_PLAYER, texture, type);
		break;
	case VALEERA:
		pj = new PlayerPJ(coor, ENTITY_TYPE::DINAMIC_PLAYER, texture, type);
		break;
	case SYLVANAS:
		pj = new PlayerPJ(coor, ENTITY_TYPE::DINAMIC_PLAYER, texture, type);
		break;
	}
	entities.push_front(pj);
}

void EntitySystem::AddConsumable(iPoint coor, CONSUMABLE_TYPE type)
{

}

void EntitySystem::AddChest(iPoint coor, CHEST_TYPE type) 
{

}

void EntitySystem::AddStaticObject(iPoint coor, STATIC_OBJECT_TYPE type) 
{

}