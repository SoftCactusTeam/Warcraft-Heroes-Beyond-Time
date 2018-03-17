#include "Pathfinding.h"
#include "Application.h"
#include "ModuleEntitySystem.h"

iPoint SillyMovementToPlayer(iPoint pos)
{
	iPoint res = { 1,1 };
	if (App->entities->actualPlayer->pos.x - pos.x < 0)
		res.x = -1;
	if (App->entities->actualPlayer->pos.y - pos.y < 0)
		res.y = -1;
	return res;
}

pathNode::pathNode(int cost)
{
	this->cost = cost;
}

Pathfinding::Pathfinding()
{
	name = "pathfinding";
}

void Pathfinding::LoadMap(uint width, uint height)
{
	this->mapWidth = width;
	this->mapHeight = height;
}

void Pathfinding::AddNodeToMap(int cost)
{
	pathNode* aux = new pathNode(cost);
	map.push_back(aux);
}

void Pathfinding::ClearMap()
{
	for (int i = 0; i < map.size(); i++)
		delete map[i];
	map.clear();
}