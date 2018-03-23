#ifndef __Pathfindgin_H__
#define __Pathfinding_H__

#include "Entity.h"
#include "Module.h"
#include <vector>

fPoint SillyMovementToPlayer(fPoint pos);

struct pathNode
{
	pathNode(int cost, iPoint nodePos);
	int cost = -1;
	iPoint nodePos;
	pathNode* neighbours[4] = { nullptr, nullptr, nullptr, nullptr };
};

class Pathfinding : public Module
{
public:
	Pathfinding();
	void LoadMap();
	void ClearMap(); 

	void AddNodeToMap(int cost, iPoint nodePos = { -1,-1 });
	void PrintColliders();

	void LoadNeighbours();
	int ExistWalkableAtPos(iPoint pos);

private:
	std::vector<pathNode*> map;
	uint tileSize = 0;
	uint mapWidth = 0;
	uint mapHeight = 0;
};

struct PathVector
{
	PathVector();
	iPoint nextTileToMove(iPoint actualPos);
	void CalculatePathAstar(iPoint thisPos, iPoint tileToMove);
private:
	std::vector<pathNode*> path;
};

#endif