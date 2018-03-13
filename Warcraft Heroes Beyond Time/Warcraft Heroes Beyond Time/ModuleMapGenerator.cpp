#include "Application.h"
#include "ModuleMapGenerator.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

#include <time.h>

#define FLOOR { 36,144,32,32 }
#define WATER { 108,0,32,32 };

MapGenerator::MapGenerator() {}

MapGenerator::~MapGenerator() {}

inline int MapGenerator::Get(int x, int y) const
{
	return (sizeX * y + x);
}

bool MapGenerator::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x < (int)sizeX * 32 &&
		pos.y >= 0 && pos.y < (int)sizeY * 32);
}

bool MapGenerator::GenerateGrid(int sizeX, int sizeY)
{
	mapTexture = App->textures->Load("map.png");

	this->sizeX = sizeX;
	this->sizeY = sizeY;
	totalSize = sizeX * sizeY;

	for (uint i = 0u; i < sizeY; ++i)
	{
		for (uint j = 0u; j < sizeX; ++j)
		{
			nodes.push_back(new MapNode({ (int)j,(int)i }, FLOOR));
		}
	}

	ExecuteAlgorithm( nodes[Get(25,25)],300);

	return nodes.size() == totalSize;
}

bool MapGenerator::ExecuteAlgorithm(MapNode* startNode, uint iterations)
{
	srand(time(NULL));

	startNode->whatToBlit = WATER;
	visited.push_back(startNode);

	MapNode* auxNode = startNode;

	for (uint i = 0u; i < iterations; ++i)
	{

		uint randNum = rand() % (3 + 1);

		if (randNum == 0 && CheckBoundaries({ auxNode->pos.x + 1, auxNode->pos.y }))
		{
			auxNode = nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)];
			auxNode->whatToBlit = WATER;
			//visited.push_back(auxNode);
		}
		else if (randNum == 1 && CheckBoundaries({ auxNode->pos.x - 1, auxNode->pos.y }))
		{
			auxNode = nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)];
			auxNode->whatToBlit = WATER;
			//visited.push_back(auxNode);
		}
		else if (randNum == 2 && CheckBoundaries({ auxNode->pos.x, auxNode->pos.y + 1 }))
		{
			auxNode = nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)];
			auxNode->whatToBlit = WATER;
			//visited.push_back(auxNode);
		}
		else if (randNum == 3 && CheckBoundaries({ auxNode->pos.x, auxNode->pos.y - 1 }))
		{
			auxNode = nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)];
			auxNode->whatToBlit = WATER;
			//visited.push_back(auxNode);
		}
		else
			--i;
	}

	auxNode = nullptr;

	return visited.size() == iterations;
}

bool MapGenerator::BlitMap()
{
	bool ret = true;

	for (uint i = 0u; i < totalSize && ret; ++i)
	{
		iPoint posToBlit = nodes[i]->pos;
		ret = App->render->Blit(mapTexture, posToBlit.x * 32, posToBlit.y * 32, &nodes[i]->whatToBlit);
	}

	return ret;
}