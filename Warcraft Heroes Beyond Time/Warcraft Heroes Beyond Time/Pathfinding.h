#ifndef __Pathfindgin_H__
#define __Pathfinding_H__

#include "Entity.h"
#include "Module.h"
#include <vector>

iPoint SillyMovementToPlayer(iPoint pos);

struct pathNode
{
	pathNode(int cost, iPoint nodePos);
	int cost = -1;
	iPoint nodePos;
};

class Pathfinding : public Module
{
public:
	Pathfinding();
	void LoadMap(uint width, uint height);
	void AddNodeToMap(int cost, iPoint nodePos = { -1,-1 });
	void ClearMap(); 

private:
	std::vector<pathNode*> map;
	uint mapWidth = 0;
	uint mapHeight = 0;
};

struct PathVector
{
	PathVector();
	iPoint nextObjective = { -1,-1 };
	iPoint pathToMove(iPoint actualPos);
private:
	std::vector<pathNode*> path;
};

#endif