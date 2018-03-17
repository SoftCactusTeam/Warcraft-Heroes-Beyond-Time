#ifndef __Pathfindgin_H__
#define __Pathfinding_H__

#include "Entity.h"
#include "Module.h"

iPoint SillyMovementToPlayer(iPoint pos);

class Pathfinding : public Module
{
public:
	Pathfinding();


private:
	Entity* actor;
};

#endif