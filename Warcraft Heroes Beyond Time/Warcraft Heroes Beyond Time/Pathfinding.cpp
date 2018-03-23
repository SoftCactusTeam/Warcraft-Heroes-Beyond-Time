#include "Pathfinding.h"
#include "Application.h"
#include "ModuleEntitySystem.h"
#include "PlayerEntity.h"
#include "ModuleMapGenerator.h"
#include "ModuleRender.h"

#include <queue>

fPoint SillyMovementToPlayer(fPoint pos)
{
	fPoint res = { 1,1 };
	if (App->entities->player->pos.x - pos.x < 0)
		res.x = -1;
	if (App->entities->player->pos.y - pos.y < 0)
		res.y = -1;
	return res;
}

// ---------------------------------------------------------------------------------------------------
// ------------------------------------------ PATH NODE ----------------------------------------------
// ---------------------------------------------------------------------------------------------------

pathNode::pathNode(int cost, iPoint nodePos)
{
	this->cost = cost;
	this->nodePos = nodePos;
}

// ---------------------------------------------------------------------------------------------------
// ------------------------------------------- PATFINDING --------------------------------------------
// ---------------------------------------------------------------------------------------------------


Pathfinding::Pathfinding() { name = "pathfinding"; }

void Pathfinding::LoadMap()
{
	std::vector<MapNode*> tmpMapNodes = App->map->GetMapNodesAndInfo(mapWidth, mapHeight, tileSize);
	for (int i = 0; i < tmpMapNodes.size(); i++)
	{
		if (tmpMapNodes[i]->cost != -1)
			AddNodeToMap(tmpMapNodes[i]->cost, tmpMapNodes[i]->pos);
	}
	LoadNeighbours();
}

void Pathfinding::ClearMap()
{
	for (int i = 0; i < map.size(); i++)
		delete map[i];
	map.clear();
}

void Pathfinding::AddNodeToMap(int cost, iPoint point)
{
	pathNode* aux = new pathNode(cost, point);
	map.push_back(aux);
}

void Pathfinding::PrintColliders()
{
	for (int i = 0; i < map.size(); i++)
		App->render->DrawQuad({ map[i]->nodePos.x * (int)tileSize, map[i]->nodePos.y * (int)tileSize, (int)tileSize, (int)tileSize }, 255, 255, 0 , 140);
}

void Pathfinding::LoadNeighbours()
{
	for (int i = 0; i < map.size(); i++)
	{
		if (ExistWalkableAtPos(iPoint(map[i]->nodePos.x + 1, map[i]->nodePos.y)) != -1)
			map[i]->neighbours[0] = map[ExistWalkableAtPos(iPoint(map[i]->nodePos.x + 1, map[i]->nodePos.y))];
		if (ExistWalkableAtPos(iPoint(map[i]->nodePos.x - 1, map[i]->nodePos.y)) != -1)
			map[i]->neighbours[0] = map[ExistWalkableAtPos(iPoint(map[i]->nodePos.x - 1, map[i]->nodePos.y))];
		if (ExistWalkableAtPos(iPoint(map[i]->nodePos.x, map[i]->nodePos.y + 1)) != -1)
			map[i]->neighbours[0] = map[ExistWalkableAtPos(iPoint(map[i]->nodePos.x, map[i]->nodePos.y + 1))];
		if (ExistWalkableAtPos(iPoint(map[i]->nodePos.x, map[i]->nodePos.y - 1)) != -1)
			map[i]->neighbours[0] = map[ExistWalkableAtPos(iPoint(map[i]->nodePos.x, map[i]->nodePos.y - 1))];
	}
}

int Pathfinding::ExistWalkableAtPos(iPoint pos)
{
	for (int i = 0; i < map.size(); i++)
		if (map[i]->nodePos == pos)
			return i;
	return -1;
}

std::vector<pathNode*> Pathfinding::GetPathNode()
{
	return map;
}

// ---------------------------------------------------------------------------------------------------
// ------------------------------------------- PATH VECTOR -------------------------------------------
// ---------------------------------------------------------------------------------------------------

PathVector::PathVector() {}

iPoint PathVector::nextTileToMove(iPoint actualPos)
{
	if (actualPos == path[0]->nodePos)
		path.pop_back();

	return path[0]->nodePos;
}

void PathVector::CalculatePathAstar(iPoint thisPos, iPoint tileToMove)
{
	const std::vector<pathNode*> mapCopy = App->path->GetPathNode();
	std::priority_queue<pathNode*> frontQueue;
	std::queue<pathNode*> visitedQueue;
	
	frontQueue.push([mapCopy[ExistWalkableAtPos(thisPos)]);
}