#include "Pathfinding.h"
#include "Application.h"
#include "ModuleEntitySystem.h"

DstarPath::DstarPath(Entity* actor)
{
	this->actor = actor;
}

iPoint SillyMovementToPlayer(iPoint pos)
{
	iPoint res = { 1,1 };
	if (App->entities->actualPlayer->pos.x - pos.x < 0)
		res.x = -1;
	if (App->entities->actualPlayer->pos.y - pos.y < 0)
		res.y = -1;
	return res;
}