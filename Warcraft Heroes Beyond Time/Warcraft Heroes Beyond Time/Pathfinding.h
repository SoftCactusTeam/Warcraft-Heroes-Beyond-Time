#ifndef __Pathfindgin_H__
#define __Pathfinding_H__

#include "Entity.h"
#include "Module.h"
#include <vector>

iPoint SillyMovementToPlayer(iPoint pos);

struct pathNode
{
	pathNode(int cost);
	int cost = -1;
};

class Pathfinding : public Module
{
public:
	Pathfinding();
	void LoadMap(uint width, uint height);
	void AddNodeToMap(int cost);
	void ClearMap();

private:
	std::vector<pathNode*> map;
	uint mapWidth = 0;
	uint mapHeight = 0;
};

struct PathList
{

};

#endif