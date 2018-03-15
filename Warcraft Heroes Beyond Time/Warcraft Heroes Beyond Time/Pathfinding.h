#ifndef __Pathfindgin_H__
#define __Pathfinding_H__

#include "Entity.h"

iPoint SillyMovementToPlayer(iPoint pos);

class DstarPath
{
public:
	DstarPath(Entity* actor);


private:
	Entity* actor;
};

#endif