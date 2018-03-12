#include "Application.h"
#include "ModuleEntitySystem.h"
#include "Enemy_Footman.h"

Enemy_Footman::Enemy_Footman(iPoint coor, ENEMY_TYPE character, SDL_Texture* texture) : EnemyEntity(coor, character, texture) {}

bool Enemy_Footman::Start()
{
	baseAnm.PushBack({ 0,188,55,47 });	// TEMPORAL !!
	anim = &baseAnm;					// TEMPORAL !!
	return true;
}

bool Enemy_Footman::Update(float dt)
{
	switch (CaculateAngle(App->entities->actualPlayer->pos))
	{
	case FIXED_ANGLE::UP:
		printf_s("UP\n");
		break;
	case FIXED_ANGLE::UP_RIGHT:
		printf_s("UP_RIGHT\n");
		break;
	case FIXED_ANGLE::RIGHT:
		printf_s("RIGHT\n");
		break;
	case FIXED_ANGLE::DOWN_RIGHT:
		printf_s("DOWN_RIGHT\n");
		break;
	case FIXED_ANGLE::DOWN:
		printf_s("DOWN\n");
		break;
	case FIXED_ANGLE::DOWN_LEFT:
		printf_s("DOWN_LEFT\n");
		break;
	case FIXED_ANGLE::LEFT:
		printf_s("LEFT\n");
		break;
	case FIXED_ANGLE::UP_LEFT:
		printf_s("UP_LEFT\n");
		break;
	}
	return true;
}

bool Enemy_Footman::Finish()
{

	return true;
}