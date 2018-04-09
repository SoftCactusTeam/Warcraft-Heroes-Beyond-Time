#ifndef __Pathfindgin_H__
#define __Pathfinding_H__

#include "Entity.h"
#include "Module.h"
#include <vector>
#include <queue>

struct pathNode
{
	pathNode(int cost, iPoint nodePos);
	int cost = 0;
	iPoint nodePos;
	std::vector<pathNode*> neighbours;
	pathNode* parent = nullptr;
};

class Pathfinding : public Module
{
public:

	void Init()
	{
		active = false;
	}

	Pathfinding();
	void LoadPathMap();
	void ClearMap(); 

	void AddNodeToMap(int cost, iPoint nodePos = { -1,-1 });
	void PrintWalkableTiles();

	void LoadNeighbours(pathNode* node);
	int ExistWalkableAtPos(iPoint& pos);

	uint tileSize = 0;
	uint mapWidth = 0;
	uint mapHeight = 0;
	bool printWalkables = false;

	std::vector<pathNode*> map;
};

struct pathNodeComparison
{
	bool operator()(const pathNode* lhs, const pathNode* rhs) const
	{
		return lhs->cost > rhs->cost;
	}
};

struct PathVector
{
	PathVector();
	iPoint nextTileToMove(iPoint actualPos);
	bool CalculatePathAstar(iPoint thisPos, iPoint tileToMove);
	bool CalculateWay(iPoint thisPos, iPoint tileToMove);

	void PrintAstar();
	void Clear();
	bool isEmpty();

private:
	std::vector<pathNode*> pathVec;
	std::vector<pathNode*> walkPath;

};

#endif