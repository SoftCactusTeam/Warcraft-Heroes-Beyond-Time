#include "Application.h"
#include "ModuleMapGenerator.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "Log.h"
#include "Scene.h"
#include "PlayerEntity.h"


#include <time.h>

#define VOID { 196,0,48,48 }
#define FLOOR { 0,49,48,48 }
#define FLOOR2 { 49,49,48,48 }
#define FLOOR3 { 98,49,48,48 }
#define FLOOR4 { 147,49,48,48 }
#define FLOOR5 { 196,49,48,48 }
#define FLOOR6 { 0,98,48,48 }
#define WALL { 0,0,48,48 }
#define WALL2 { 49,0,48,48 }
#define WALL3 { 98,0,48,48 }
#define WALL4 { 147,0,48,48 }

MapGenerator::MapGenerator() {}

MapGenerator::~MapGenerator() {}
bool MapGenerator::CleanUp()
{
	bool ret = true;

	LOG("Unloading Map Texture...");
	ret = App->textures->UnLoad(mapTexture);

	for (uint i = 0u; i < nodes.size(); ++i)
	{
		RELEASE(nodes[i]);
	}

	nodes.clear();

	visited.clear();

	totalSize = 0u;

	return nodes.size() <= 0 && visited.size() <= 0;
}

iPoint MapGenerator::GetRandomValidPoint()
{
	int randNum = 0;

	do
		randNum = rand() % (nodes.size() - 0 + 1);
	while (!nodes[randNum]->floor || nodes[randNum]->pos == nodes[Get(sizeX / 2, sizeY / 2)]->pos);

	return nodes[randNum]->pos;
}

bool MapGenerator::DrawPrePlayerMap()
{
	bool ret = true;

	for (uint i = 0u; i < totalSize && ret; ++i)
	{
		if (nodes[i]->floor || (!nodes[i]->floor && nodes[i]->pos.y * tileSize <= App->scene->player->pos.y))
		{
			iPoint posToBlit = nodes[i]->pos;
			ret = App->render->Blit(mapTexture, posToBlit.x * tileSize, posToBlit.y * tileSize, &nodes[i]->whatToBlit);
		}
	}

	return ret;
}

bool MapGenerator::DrawPostPlayerMap()
{
	bool ret = true;

	for (uint i = 0u; i < totalSize && ret; ++i)
	{
		if (!nodes[i]->floor && nodes[i]->pos.y * tileSize >= App->scene->player->pos.y)
		{
			iPoint posToBlit = nodes[i]->pos;
			ret = App->render->Blit(mapTexture, posToBlit.x * tileSize, posToBlit.y * tileSize, &nodes[i]->whatToBlit);
		}
	}

	return ret;
}

inline int MapGenerator::Get(int x, int y) const
{
	return (sizeX * y + x);
}

bool MapGenerator::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x > 1 && pos.x < (int)sizeX - 2 &&
		pos.y > 1 && pos.y < (int)sizeY - 2);
}

bool MapGenerator::GenerateMap(MapData data)
{
	bool ret = true;

	mapTexture = App->textures->Load(data.tilesetPath);

	this->sizeX = data.sizeX;
	this->sizeY = data.sizeY;
	totalSize = sizeX * sizeY;

	LOG("Generating map Grid...");

	for (uint i = 0u; i < sizeY; ++i)
	{
		for (uint j = 0u; j < sizeX; ++j)
		{
			nodes.push_back(new MapNode({ (int)j,(int)i }, VOID));
		}
	}
	ret = nodes.size() == totalSize;

	if (ret)
		ret = ExecuteAlgorithm( nodes[Get(sizeX/2,sizeY/2)], data.iterations, data.seed);

	if (ret)
		ret = GenerateWalls();

	return ret;
}

bool MapGenerator::ExecuteAlgorithm(MapNode* startNode, uint iterations, int seed)
{
	LOG("Executing map Algorithm...");

	if (seed != 0)
		srand(seed);
	else
		srand(time(NULL));

	startNode->whatToBlit = randomTile(true);
	startNode->floor = true;
	visited.push_back(startNode);

	MapNode* auxNode = startNode;

	for (uint i = 0u; i < iterations;)
	{

		uint randNum = rand() % (10 + 1);

		if ((randNum == 0 || randNum == 1 || randNum == 3) && CheckBoundaries({ auxNode->pos.x + 1, auxNode->pos.y }))
		{
			auxNode = nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)];
			if (!auxNode->floor)
			{
				auxNode->whatToBlit = randomTile(true);
				auxNode->cost = 0;
				visited.push_back(auxNode);
				auxNode->floor = true;
				i++;
			}
		}

		else if ((randNum == 4 || randNum == 5 || randNum == 6) && CheckBoundaries({ auxNode->pos.x - 1, auxNode->pos.y }))
		{
			auxNode = nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)];
			if (!auxNode->floor)
			{
				auxNode->whatToBlit = randomTile(true);
				auxNode->cost = 0;
				visited.push_back(auxNode);
				auxNode->floor = true;
				i++;
			}
		}

		else if ((randNum == 7 || randNum == 8) && CheckBoundaries({ auxNode->pos.x, auxNode->pos.y + 1 }))
		{
			auxNode = nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)];
			if (!auxNode->floor)
			{
				auxNode->whatToBlit = randomTile(true);
				auxNode->cost = 0;
				visited.push_back(auxNode);
				auxNode->floor = true;
				i++;
			}
		}

		else if ((randNum == 9 || randNum == 10) && CheckBoundaries({ auxNode->pos.x, auxNode->pos.y - 1 }))
		{
			auxNode = nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)];
			if (!auxNode->floor)
			{
				auxNode->whatToBlit = randomTile(true);
				auxNode->cost = 0;
				visited.push_back(auxNode);
				auxNode->floor = true;
				i++;
			}
		}
	}

	auxNode = nullptr;

	return visited.size() == iterations + 1;
}


SDL_Rect MapGenerator::randomTile(bool isFloor)
{
	SDL_Rect toReturn = VOID;

	if (isFloor)
	{
		int randNum = rand() % (6 - 1 + 1) + 1;

		if (randNum == 1)
			toReturn = FLOOR;
		else if (randNum == 2)
			toReturn = FLOOR2;
		else if (randNum == 3)
			toReturn = FLOOR3;
		else if (randNum == 4)
			toReturn = FLOOR4;
		else if (randNum == 5)
			toReturn = FLOOR5;
		else if (randNum == 6)
			toReturn = FLOOR6;
	}
	else
	{
		int randNum = rand() % (4 - 1 + 1) + 1;

		if (randNum == 1)
			toReturn = WALL;
		else if (randNum == 2)
			toReturn = WALL2;
		else if (randNum == 3)
			toReturn = WALL3;
		else if (randNum == 4)
			toReturn = WALL4;
	}

	return toReturn;
}

bool MapGenerator::GenerateWalls()
{
	LOG("Generating Walls...");

	for (uint i = 0u; i < visited.size(); ++i)
	{
		if (visited[i]->floor)
		{
			MapNode* auxNode = visited[i];

				if (SDL_RectEquals(&nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->whatToBlit, &SDL_Rect(VOID)))
				{
					nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->whatToBlit = randomTile(false);
					//Check type of node adn Update
					//int type = CheckTypeOfNode(nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]);
					//UpdateNode(nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)], type);
				}

				if (SDL_RectEquals(&nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]->whatToBlit, &SDL_Rect(VOID)))
				{
					//Check type of node adn Update
					//int type = CheckTypeOfNode(nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]);
					//UpdateNode(nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)], type);
				}

				if (SDL_RectEquals(&nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]->whatToBlit, &SDL_Rect(VOID)))
				{
					//Check type of node adn Update
					//int type = CheckTypeOfNode(nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]);
					//UpdateNode(nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)], type);
				}

				if (SDL_RectEquals(&nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)]->whatToBlit, &SDL_Rect(VOID)))
				{
					//Check type of node adn Update
					//int type = CheckTypeOfNode(nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)]);
					//UpdateNode(nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)], type);
				}
		}
	}

	return true;
}

void MapGenerator::getSize(uint& w, uint& h)
{
	w = sizeX;
	h = sizeY;
}

int MapGenerator::getTileSize()
{
	return tileSize;
}

std::vector<MapNode*> MapGenerator::GetMapNodesAndInfo(uint& sizeX, uint& sizeY, uint& tileSize)
{
	sizeX = this->sizeX;
	sizeY = this->sizeY;
	tileSize = this->tileSize;
	return nodes;
}
