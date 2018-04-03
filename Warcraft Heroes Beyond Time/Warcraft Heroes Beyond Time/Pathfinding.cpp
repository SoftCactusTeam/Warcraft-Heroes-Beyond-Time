#include "Pathfinding.h"
#include "Application.h"
#include "ModuleEntitySystem.h"
#include "PlayerEntity.h"
#include "ModuleMapGenerator.h"
#include "ModuleRender.h"
#include "Scene.h"

#include "Brofiler\Brofiler.h"

fPoint SillyMovementToPlayer(fPoint pos)
{
	fPoint res = { 1,1 };
	if (App->scene->player->pos.x - pos.x < 0)
		res.x = -1;
	if (App->scene->player->pos.y - pos.y < 0)
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
// ------------------------------------------- PATHFINDING --------------------------------------------
// ---------------------------------------------------------------------------------------------------


Pathfinding::Pathfinding() { name = "pathfinding"; }

void Pathfinding::LoadPathMap()
{
	std::vector<MapNode*> tmpMapNodes = App->map->GetMapNodesAndInfo(mapWidth, mapHeight, tileSize);
	for (int i = 0; i < tmpMapNodes.size(); i++)
	{
		if (tmpMapNodes[i]->cost == -1)
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

void Pathfinding::PrintWalkableTiles()
{

	for (int i = 0; i < map.size(); i++)
	{
		App->render->DrawQuad({ map[i]->nodePos.x * (int)tileSize, map[i]->nodePos.y * (int)tileSize, (int)tileSize, (int)tileSize }, 255, 255, 0, 140);
	}
}

void Pathfinding::LoadNeighbours()
{
	for (int i = 0; i < map.size(); i++)
	{
		if (ExistWalkableAtPos(iPoint(map[i]->nodePos.x + 1, map[i]->nodePos.y)) != -1)
			map[i]->neighbours[0] = map[ExistWalkableAtPos(iPoint(map[i]->nodePos.x + 1, map[i]->nodePos.y))];
		if (ExistWalkableAtPos(iPoint(map[i]->nodePos.x - 1, map[i]->nodePos.y)) != -1)
			map[i]->neighbours[1] = map[ExistWalkableAtPos(iPoint(map[i]->nodePos.x - 1, map[i]->nodePos.y))];
		if (ExistWalkableAtPos(iPoint(map[i]->nodePos.x, map[i]->nodePos.y + 1)) != -1)
			map[i]->neighbours[2] = map[ExistWalkableAtPos(iPoint(map[i]->nodePos.x, map[i]->nodePos.y + 1))];
		if (ExistWalkableAtPos(iPoint(map[i]->nodePos.x, map[i]->nodePos.y - 1)) != -1)
			map[i]->neighbours[3] = map[ExistWalkableAtPos(iPoint(map[i]->nodePos.x, map[i]->nodePos.y - 1))];
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

bool PathVector::CalculatePathAstar(iPoint thisPos, iPoint tileToMove)
{
	BROFILER_CATEGORY("AStar Algorithm", Profiler::Color::Azure);

	/// CLEAN VISITED
	pathVec.clear();

	/// SET SIZE & POSITIONS
	thisPos = iPoint(thisPos.x / App->map->getTileSize() , thisPos.y / App->map->getTileSize());
	tileToMove = iPoint(tileToMove.x / App->map->getTileSize() , tileToMove.y / App->map->getTileSize());

	/// SI L'OBJECTIU ESTA FORA DEL PATH WALKABLE FORA
	if (App->path->ExistWalkableAtPos(tileToMove) == -1)
		return false;

	/// INIT QUEUES & FIRST PUSH
	std::priority_queue<pathNode*, std::vector<pathNode*>, pathNodeComparison> frontQueue;
	std::vector<pathNode*> visitedQueue;
	if (App->path->ExistWalkableAtPos(thisPos) == -1)
		return false;
	frontQueue.push(App->path->map[App->path->ExistWalkableAtPos(thisPos)]);
	visitedQueue.push_back(App->path->map[App->path->ExistWalkableAtPos(thisPos)]);
	
	/// BUCLE
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
				/// Comprobar si ja esta a visited
				bool isVisited = false;
				for (int i2 = 0; i2 < visitedQueue.size() && isVisited == false; i2++)
					if (visitedQueue[i2] == current->neighbours[i]) {
						isVisited = true;
						break;
					}
				/// Si no es visitada fer la resta
				if (isVisited == false)
				{
					int a = (current->neighbours[i]->nodePos.x) - tileToMove.x;
					if (a < 0)
						a *= -1;
					int b = (current->neighbours[i]->nodePos.y) - tileToMove.y;
					if (b < 0)
						b *= -1;
					int distanceToObjective = a + b;
					current->neighbours[i]->cost = 1 + distanceToObjective;
					current->neighbours[i]->parent = current;
					frontQueue.push(current->neighbours[i]);
					visitedQueue.push_back(current->neighbours[i]);
				}
				//else
				//{
				//	int a = current->neighbours[i]->nodePos.x - tileToMove.x;
				//	if (a < 0)
				//		a *= -1;
				//	int b = current->neighbours[i]->nodePos.y - tileToMove.y;
				//	if (b < 0)
				//		b *= -1;
				//	int distanceToObjective = a + b;
				//	if ((current->neighbours[i]->cost + current->cost + distanceToObjective) < (current->neighbours[i]->cost))
				//	{
				//		frontQueue.push(current->neighbours[i]);
				//		current->neighbours[i]->parent = current;
				//	}
				//}
			}
	}
	pathVec = visitedQueue;
}

bool PathVector::CalculateWay(iPoint thisPos, iPoint tileToMove)
{
	/// SET SIZE & POSITIONS
	thisPos = iPoint(thisPos.x / App->map->getTileSize(), thisPos.y / App->map->getTileSize());
	tileToMove = iPoint(tileToMove.x / App->map->getTileSize(), tileToMove.y / App->map->getTileSize());

	/// CLEAR VECTOR
	walkPath.clear();

	/// CALCULATE VECTOR
	for (int i = 0; i < pathVec.size(); i++)
		if (pathVec[i]->nodePos == tileToMove)
		{
			walkPath.push_back(pathVec[i]);
			pathNode* aux = pathVec[i];
			i = pathVec.size();
			while (aux->nodePos != thisPos)
			{
				aux = aux->parent;
				if (aux->parent != nullptr)
					walkPath.push_back(aux);
				else
					break;
			}
		}
	return true;
}

void PathVector::PrintAstar()
{
	int tileSize = App->map->getTileSize();
	for (int i = 0; i < pathVec.size(); i++)
		App->render->DrawQuad({ pathVec[i]->nodePos.x * (int)tileSize, pathVec[i]->nodePos.y * (int)tileSize, (int)tileSize, (int)tileSize }, 0, 150 , 255, 140);
	for (int i = 0; i < walkPath.size(); i++)
		App->render->DrawQuad({ walkPath[i]->nodePos.x * (int)tileSize, walkPath[i]->nodePos.y * (int)tileSize, (int)tileSize, (int)tileSize }, 0, 200, 255, 140);
}

void PathVector::Clear()
{
	pathVec.clear();
	walkPath.clear();
}

bool PathVector::isEmpty()
{
	if (pathVec.size() <= 0)
		return true;
	else if (walkPath.size() <= 0)
		return true;
	return false;
}

iPoint PathVector::nextTileToMove(iPoint actualPos)
{
	/// SET SIZE & POSITIONS
	actualPos = iPoint(actualPos.x / App->map->getTileSize(), actualPos.y / App->map->getTileSize());

	iPoint ret(0, 0);
	if (walkPath.empty() == true)
		return ret;

	ret.x = walkPath.back()->nodePos.x - actualPos.x;
	ret.y = walkPath.back()->nodePos.y - actualPos.y;
	if (ret.x == 0 && ret.y == 0)
		walkPath.pop_back();

	return ret;
}