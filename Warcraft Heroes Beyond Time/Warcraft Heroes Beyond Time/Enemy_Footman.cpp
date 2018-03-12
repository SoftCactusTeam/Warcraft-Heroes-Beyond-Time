#include "Enemy_Footman.h"

Enemy_Footman::Enemy_Footman(iPoint coor, ENEMY_TYPE character, SDL_Texture* texture) : EnemyEntity(coor, character, texture) {}

bool Enemy_Footman::Start()
{
	baseAnm.PushBack({ 0,188,55,47 });	// TEMPORAL !!
	anim = &baseAnm;					// TEMPORAL !!
	return true;
}

bool Enemy_Footman::Update()
{

	return true;
}

bool Enemy_Footman::Finish()
{

	return true;
}