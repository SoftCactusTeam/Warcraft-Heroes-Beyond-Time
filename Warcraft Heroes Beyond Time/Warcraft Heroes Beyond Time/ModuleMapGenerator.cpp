#include "Application.h"
#include "ModuleMapGenerator.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

#include <time.h>

#define FLOOR { 260,506,32,32 }
#define WATER { 292,928,32,32 }
#define ESQUERRA { 35,441,32,32 }
#define DRETA { 516,473,32,32 }
#define ABAIX { 356,921,32,32 }
#define ADALT { 356,409,32,32 }
#define ADALT2 { 292,184,0,0 }
#define DRETAIESQUERRA
#define ADALTIABAIX
#define ADALTDRETA
#define ADALTESQUERRA
#define ADALTESQUERRADRETA
#define ABAIXDRETA
#define ABAIXESQUERRA
#define ABAIXESQUERRADRETA
#define TANCAT

MapGenerator::MapGenerator() {}

MapGenerator::~MapGenerator() {}

inline int MapGenerator::Get(int x, int y) const
{
	return (sizeX * y + x);
}

bool MapGenerator::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x < (int)sizeX &&
		pos.y >= 0 && pos.y < (int)sizeY);
}

bool MapGenerator::GenerateGrid(int sizeX, int sizeY)
{
	mapTexture = App->textures->Load("map.jpg");

	this->sizeX = sizeX;
	this->sizeY = sizeY;
	totalSize = sizeX * sizeY;

	for (uint i = 0u; i < sizeY; ++i)
	{
		for (uint j = 0u; j < sizeX; ++j)
		{
			nodes.push_back(new MapNode({ (int)j,(int)i }, WATER));
		}
	}

	ExecuteAlgorithm( nodes[Get(25,25)],600);

	//GenerateWalls();

	return nodes.size() == totalSize;
}

bool MapGenerator::ExecuteAlgorithm(MapNode* startNode, uint iterations)
{
	srand(time(NULL));

	startNode->whatToBlit = FLOOR;

	MapNode* auxNode = startNode;

	for (uint i = 0u; i < iterations;)
	{

		uint randNum = rand() % (10 + 1);

		if ((randNum == 0 || randNum == 1 || randNum == 3) && CheckBoundaries({ auxNode->pos.x + 1, auxNode->pos.y }))
		{
			auxNode = nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)];
			if (auxNode->whatToBlit.x != 260)
			{
				auxNode->whatToBlit = FLOOR;
				i++;
			}
		}
		else if ((randNum == 4 || randNum == 5 || randNum == 6) && CheckBoundaries({ auxNode->pos.x - 1, auxNode->pos.y }))
		{
			auxNode = nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)];
			if (auxNode->whatToBlit.x != 260)
			{
				auxNode->whatToBlit = FLOOR;
				i++;
			}
		}
		else if ((randNum == 7 || randNum == 8) && CheckBoundaries({ auxNode->pos.x, auxNode->pos.y + 1 }))
		{
			auxNode = nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)];
			if (auxNode->whatToBlit.x != 260)
			{
				auxNode->whatToBlit = FLOOR;
				i++;
			}
		}
		else if ((randNum == 9 || randNum == 10) && CheckBoundaries({ auxNode->pos.x, auxNode->pos.y - 1 }))
		{
			auxNode = nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)];
			if (auxNode->whatToBlit.x != 260)
			{
				auxNode->whatToBlit = FLOOR;
				i++;
			}
		}
	}

	auxNode = nullptr;

	return true;
}

bool MapGenerator::GenerateWalls()
{
	for (uint i = 0u; i < nodes.size(); ++i)
	{
		if (nodes[i]->whatToBlit.x == 260)
		{
			MapNode* auxNode = nodes[Get(nodes[i]->pos.x, nodes[i]->pos.y)];
	
			if (nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->whatToBlit.x == 292)
			{
				if(CheckBoundaries(nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]->pos))
					nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->whatToBlit = ADALT;
				if (CheckBoundaries(nodes[Get(auxNode->pos.x, auxNode->pos.y + 2)]->pos))
					nodes[Get(auxNode->pos.x, auxNode->pos.y - 2)]->whatToBlit = ADALT2;
			}

			if (nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]->whatToBlit.x == 292)
			{
				if (CheckBoundaries(nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]->pos))
					nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]->whatToBlit = ABAIX;
			}

			if (nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]->whatToBlit.x == 292)
			{
				if (CheckBoundaries(nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]->pos))
					nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]->whatToBlit = DRETA;
			}

			if (nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)]->whatToBlit.x == 292)
			{
				if (CheckBoundaries(nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)]->pos))
					nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)]->whatToBlit = ESQUERRA;
			}
		}
	}

	return true;
}

bool MapGenerator::Update(float dt)
{
	bool ret = true;

	for (uint i = 0u; i < totalSize && ret; ++i)
	{
		iPoint posToBlit = nodes[i]->pos;
		ret = App->render->Blit(mapTexture, posToBlit.x * 32, posToBlit.y * 32, &nodes[i]->whatToBlit);
	}

	return ret;
}

bool MapGenerator::CleanMap()
{
	bool ret = true;

	ret = App->textures->UnLoad(mapTexture);

	for (uint i = 0u; i < nodes.size(); ++i)
	{
		RELEASE(nodes[i]);
	}

	nodes.clear();

	return nodes.size() <= 0;
}

bool MapGenerator::CleanUp()
{
	bool ret = true;

	ret = App->textures->UnLoad(mapTexture);

	for (uint i = 0u; i < nodes.size(); ++i)
	{
		RELEASE(nodes[i]);
	}

	nodes.clear();

	return nodes.size() <= 0;
}