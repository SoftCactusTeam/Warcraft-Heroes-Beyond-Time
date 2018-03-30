#include "Pathfinding.h"
#include "Application.h"
#include "ModuleEntitySystem.h"
#include "PlayerEntity.h"
#include "Scene.h"

fPoint SillyMovementToPlayer(fPoint pos)
{
	fPoint res = { 1,1 };
	if (App->scene->player->pos.x - pos.x < 0)
		res.x = -1;
	if (App->scene->player->pos.y - pos.y < 0)
		res.y = -1;
	return res;
}

pathNode::pathNode(int cost, iPoint nodePos)
{
	this->cost = cost;
	this->nodePos = nodePos;
}

Pathfinding::Pathfinding() { name = "pathfinding"; }


void Pathfinding::LoadPathMap()
{
	this->mapWidth = 50;
	this->mapHeight = 50;
}

void Pathfinding::AddNodeToMap(int cost, iPoint point)
{
	pathNode* aux = new pathNode(cost, point);
	map.push_back(aux);
}

void Pathfinding::ClearMap()
{
	for (int i = 0; i < map.size(); i++)
		delete map[i];
	map.clear();
}

//PathVector::PathVector()
//{
//
//}
//
//iPoint PathVector::pathToMove(iPoint actualPos)
//{
//	if (actualPos == nextObjective)
//	{
//		path.pop_back();
//		nextObjective = path[0]->nodePos;
//	}
//}
