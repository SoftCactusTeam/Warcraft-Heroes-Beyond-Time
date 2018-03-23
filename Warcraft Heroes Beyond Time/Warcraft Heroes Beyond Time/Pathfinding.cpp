#include "Pathfinding.h"
#include "Application.h"
#include "ModuleEntitySystem.h"
#include "PlayerEntity.h"
#include "ModuleMapGenerator.h"
#include "ModuleRender.h"

fPoint SillyMovementToPlayer(fPoint pos)
{
	fPoint res = { 1,1 };
	if (App->entities->player->pos.x - pos.x < 0)
		res.x = -1;
	if (App->entities->player->pos.y - pos.y < 0)
		res.y = -1;
	return res;
}

pathNode::pathNode(int cost, iPoint nodePos)
{
	this->cost = cost;
	this->nodePos = nodePos;
}

Pathfinding::Pathfinding() { name = "pathfinding"; }

void Pathfinding::LoadMap()
{
	std::vector<MapNode*> tmpMapNodes = App->map->GetMapNodesAndInfo(mapWidth, mapHeight, tileSize);
	for (int i = 0; i < tmpMapNodes.size(); i++)
	{
		if (tmpMapNodes[i]->cost != -1)
			AddNodeToMap(tmpMapNodes[i]->cost, tmpMapNodes[i]->pos);
	}
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