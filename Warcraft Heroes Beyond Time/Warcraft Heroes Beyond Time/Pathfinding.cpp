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

bool pathNode::operator < (const pathNode& compare)
{
	return this->cost < compare.cost;
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
	std::priority_queue<pathNode*> frontQueue;
	std::vector<pathNode*> visitedQueue;
	uint costSoFar = 0;

	frontQueue.push(App->path->map[App->path->ExistWalkableAtPos(thisPos)]);
	visitedQueue.push_back(App->path->map[App->path->ExistWalkableAtPos(thisPos)]);

	while (frontQueue.empty() == false)
	{
		pathNode* current = frontQueue.top();
		frontQueue.pop();
		if (current->nodePos == tileToMove)
			break;
		// Calcular veins
		for (int i = 0; i < 4; i++)
			if (current->neighbours[i] != nullptr)
			{
				// Comprobar si ja esta a visited
				bool isVisited = false;
				for (int i2 = 0; i2 < visitedQueue.size() && isVisited == false; i2++)
					if (visitedQueue[i2] == current->neighbours[i])
						isVisited = true;
				// Si no es visitada fer la resta
				if (isVisited == false)
				{
					uint distanceToObjective = (uint)(current->neighbours[i]->nodePos.x - tileToMove.x) + (uint)(current->neighbours[i]->nodePos.y - tileToMove.y);
					current->neighbours[i]->cost += current->cost + distanceToObjective;
					frontQueue.push(current->neighbours[i]);
					visitedQueue.push_back(current->neighbours[i]);
					current->neighbours[i]->parent = current;
				}
				else
				{
					uint distanceToObjective = (uint)(current->neighbours[i]->nodePos.x - tileToMove.x) + (uint)(current->neighbours[i]->nodePos.y - tileToMove.y);
					if ((current->neighbours[i]->cost += current->cost + distanceToObjective) < (current->neighbours[i]->cost))
					{
						frontQueue.push(current->neighbours[i]);
						current->neighbours[i]->parent = current;
					}
				}
			}
	}
}