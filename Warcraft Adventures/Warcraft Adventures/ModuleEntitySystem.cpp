#include "ModuleEntitySystem.h"

void EntitySystem::Init()
{
	active = true;
}

bool EntitySystem::Start()
{
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
	return true;
}

void EntitySystem::ClearEntitiesList()
{

}

void EntitySystem::AddEnemie(iPoint coor, ENEMY_TYPE type)
{

}

void EntitySystem::AddBoss(iPoint coor, BOSS_TYPE type)
{

}

void EntitySystem::AddPlayer(iPoint coor, PLAYER_TYPE type)
{

}

void EntitySystem::AddItem(iPoint coor, CONSUMABLE_TYPE type)
{

}

void EntitySystem::AddChest(iPoint coor, CHEST_TYPE type) 
{

}

void EntitySystem::AddStaticObject(iPoint coor, STATIC_OBJECT_TYPE type) 
{

}