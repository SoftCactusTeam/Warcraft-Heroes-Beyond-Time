#include "Application.h"
#include "ModuleMapGenerator.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

#include "Timer.h"

#define FLOOR { 36,144,32,32 }
#define WATER { 108,0,32,32 };

MapGenerator::MapGenerator() {}

MapGenerator::~MapGenerator() {}

inline int MapGenerator::Get(int x, int y) const
{
	return (sizeX * y + x);
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

	ExecuteAlgorithm( nodes[Get(10,10)],10);

	return nodes.size() == totalSize;
}

bool MapGenerator::ExecuteAlgorithm(MapNode* startNode, uint iterations)
{
	srand(NULL);

	visited.push_back(startNode);

	MapNode* auxNode = nullptr;

	for (uint i = 0u; i < iterations; ++i)
	{
		if (startNode == nullptr)
		{
			auxNode = nodes[Get(startNode->pos.x + 1, startNode->pos.y)];
			auxNode->whatToBlit = WATER;
			//visited.push_back(auxNode);
		}
		else if (startNode == nullptr)
		{
			auxNode = nodes[Get(startNode->pos.x - 1, startNode->pos.y)];
			auxNode->whatToBlit = WATER;
			//visited.push_back(auxNode);
		}
		else if (startNode == nullptr)
		{
			auxNode = nodes[Get(startNode->pos.x, startNode->pos.y + 1)];
			auxNode->whatToBlit = WATER;
			//visited.push_back(auxNode);
		}
		else if (startNode == nullptr)
		{
			auxNode = nodes[Get(startNode->pos.x, startNode->pos.y - 1)];
			auxNode->whatToBlit = WATER;
			//visited.push_back(auxNode);
		}
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