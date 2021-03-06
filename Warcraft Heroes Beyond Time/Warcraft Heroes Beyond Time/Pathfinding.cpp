#include "Pathfinding.h"
#include "Application.h"
#include "ModuleEntitySystem.h"
#include "PlayerEntity.h"
#include "ModuleMapGenerator.h"
#include "ModuleRender.h"
#include "ModulePrinter.h"
#include "Scene.h"

#include "Brofiler\Brofiler.h"

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
	BROFILER_CATEGORY("LoadPathfinding", Profiler::Color::Chocolate);
	std::vector<MapNode*> tmpMapNodes = App->map->GetMapNodesAndInfo(mapWidth, mapHeight, tileSize);
	for (int i = 0; i < tmpMapNodes.size(); i++)
	{
		if (tmpMapNodes[i]->cost == -1)
			AddNodeToMap(tmpMapNodes[i]->cost, tmpMapNodes[i]->pos);
	}
}

void Pathfinding::ClearMap()
{
	BROFILER_CATEGORY("ClearPATHFINDING", Profiler::Color::Chocolate);
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
		App->render->DrawQuad({ map[i]->nodePos.x * ((int)tileSize - 2), map[i]->nodePos.y * ((int)tileSize - 2), ((int)tileSize), ((int)tileSize) }, 255, 255, 0, 140);
}

void Pathfinding::LoadNeighbours(pathNode* node)
{
	BROFILER_CATEGORY("LoadPathfindingNeighbours", Profiler::Color::Chocolate);

	// Frontals
	if (ExistWalkableAtPos(iPoint(node->nodePos.x + 1, node->nodePos.y)) != -1)
		node->neighbours.push_back(map[ExistWalkableAtPos(iPoint(node->nodePos.x + 1, node->nodePos.y))]);
	if (ExistWalkableAtPos(iPoint(node->nodePos.x - 1, node->nodePos.y)) != -1)
		node->neighbours.push_back(map[ExistWalkableAtPos(iPoint(node->nodePos.x - 1, node->nodePos.y))]);
	if (ExistWalkableAtPos(iPoint(node->nodePos.x, node->nodePos.y + 1)) != -1)
		node->neighbours.push_back(map[ExistWalkableAtPos(iPoint(node->nodePos.x, node->nodePos.y + 1))]);
	if (ExistWalkableAtPos(iPoint(node->nodePos.x, node->nodePos.y - 1)) != -1)
		node->neighbours.push_back(map[ExistWalkableAtPos(iPoint(node->nodePos.x, node->nodePos.y - 1))]);
	// Diagonals
	//if (ExistWalkableAtPos(iPoint(node->nodePos.x + 1, node->nodePos.y + 1)) != -1)
	//	node->neighbours.push_back(map[ExistWalkableAtPos(iPoint(node->nodePos.x + 1, node->nodePos.y + 1))]);
	//if (ExistWalkableAtPos(iPoint(node->nodePos.x + 1, node->nodePos.y - 1)) != -1)
	//	node->neighbours.push_back(map[ExistWalkableAtPos(iPoint(node->nodePos.x + 1, node->nodePos.y - 1))]);
	//if (ExistWalkableAtPos(iPoint(node->nodePos.x - 1, node->nodePos.y + 1)) != -1)
	//	node->neighbours.push_back(map[ExistWalkableAtPos(iPoint(node->nodePos.x - 1, node->nodePos.y + 1))]);
	//if (ExistWalkableAtPos(iPoint(node->nodePos.x - 1, node->nodePos.y - 1)) != -1)
	//	node->neighbours.push_back(map[ExistWalkableAtPos(iPoint(node->nodePos.x - 1, node->nodePos.y - 1))]);
}

int Pathfinding::ExistWalkableAtPos(iPoint& pos)
{
	for (int i = 0; i < map.size(); i++)
		if (map[i]->nodePos == pos)
			return i;
	return -1;
}

fPoint Pathfinding::posAtConcreteTile(int tile)
{
	if (tile < map.size())
		if (ExistWalkableAtPos(map[tile]->nodePos) != -1)
			return fPoint(map[tile]->nodePos.x, map[tile]->nodePos.y);
	return fPoint(0, 0);
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

	thisPos = iPoint(thisPos.x / (App->map->getTileSize() - 2) , thisPos.y / (App->map->getTileSize() - 2));
	tileToMove = iPoint(tileToMove.x / (App->map->getTileSize() - 2), tileToMove.y / (App->map->getTileSize() - 2));

	/// SI L'OBJECTIU ESTA FORA DEL PATH WALKABLE FORA
	if (App->path->ExistWalkableAtPos(tileToMove) == -1 || App->path->ExistWalkableAtPos(thisPos) == -1)
		return false;

	/// INIT QUEUES & FIRST PUSH
	std::priority_queue<pathNode*, std::vector<pathNode*>, pathNodeComparison> frontQueue;
	std::vector<pathNode*> visitedQueue;

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
		App->path->LoadNeighbours(current);
		for (int i = 0; i < current->neighbours.size(); i++)
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
		}
		current->neighbours.clear();
	}
	pathVec = visitedQueue;
	return true;
}

bool PathVector::CalculateWay(iPoint thisPos, iPoint tileToMove)
{
	/// SET SIZE & POSITIONS
	thisPos = iPoint(thisPos.x / (App->map->getTileSize() - 2), thisPos.y / (App->map->getTileSize() -2));
	tileToMove = iPoint(tileToMove.x / (App->map->getTileSize() -2), tileToMove.y / (App->map->getTileSize() -2));

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
	int tileSize = App->map->getTileSize() - 2;
	for (int i = 0; i < pathVec.size(); i++)
	{
		App->printer->PrintQuad({ pathVec[i]->nodePos.x * (int)tileSize, pathVec[i]->nodePos.y * (int)tileSize, (int)tileSize, (int)tileSize }, Black);
		App->render->DrawQuad({ pathVec[i]->nodePos.x * (int)tileSize, pathVec[i]->nodePos.y * (int)tileSize, (int)tileSize, (int)tileSize }, 255, 255, 0, 100);
	}
	for (int i = 0; i < walkPath.size(); i++)
	{
		App->printer->PrintQuad({ walkPath[i]->nodePos.x * (int)tileSize, walkPath[i]->nodePos.y * (int)tileSize, (int)tileSize, (int)tileSize }, Blue);
		App->render->DrawQuad({ walkPath[i]->nodePos.x * (int)tileSize, walkPath[i]->nodePos.y * (int)tileSize, (int)tileSize, (int)tileSize }, 255, 200, 0, 100);
	}
}

void PathVector::Clear()
{
	pathVec.clear();
	walkPath.clear();
}

bool PathVector::isEmpty()
{
	if (pathVec.size() <= 0 || walkPath.size() <= 0)
		return true;
	return false;
}

iPoint PathVector::nextTileToMove(iPoint actualPos)
{
	/// SET SIZE & POSITIONS
	//actualPos = iPoint((actualPos.x + App->map->getTileSize() / 2) / (App->map->getTileSize() - 2), (actualPos.y + App->map->getTileSize() / 2) / (App->map->getTileSize() - 2));

	int tileSize = App->map->getTileSize() - 2;
	int limit = 3;

	iPoint ret(0, 0);
	iPoint temporal(0, 0);

	if (walkPath.empty() == true)
		return ret;

	ret.x = (walkPath.back()->nodePos.x * tileSize + (tileSize / 2)) - actualPos.x;
	ret.y = (walkPath.back()->nodePos.y * tileSize + (tileSize / 2)) - actualPos.y;

	temporal.x = ret.x;
	temporal.y = ret.y;

	if (temporal.x < 0)
		temporal.x *= -1;
	if (temporal.y < 0)
		temporal.y *= -1;

	if (temporal.x <= limit && temporal.y <= limit)
	{
		walkPath.pop_back();
		return iPoint(0, 0);
	}

	if (temporal.x > limit)
		if (ret.x > 0)
			ret.x = 1;
		else
			ret.x = -1;
	else
		ret.x = 0;

	if (temporal.y > limit)
		if (ret.y > 0)
			ret.y = 1;
		else
			ret.y = -1;
	else
		ret.y = 0;

	return ret;
}
