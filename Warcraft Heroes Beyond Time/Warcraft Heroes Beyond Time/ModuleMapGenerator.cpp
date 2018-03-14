#include "Application.h"
#include "ModuleMapGenerator.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

#include <time.h>

#define FLOOR { 260,506,32,32 }
#define WATER { 292,928,32,32 }
#define ESQUERRA { 3,441,32,32 }
#define DRETA { 516,473,32,32 }
#define ABAIX { 356,921,32,32 }
#define ADALT { 356,409,32,32 }
#define ADALT2 { 292,184,0,0 }
#define DRETAIESQUERRA { 260,601,32,32 }
#define ADALTIABAIX { 453,537,32,32 }
#define ADALTDRETA { 69,506,32,32 }
#define ADALTESQUERRA { 261,890,32,32 }
#define ADALTESQUERRADRETA { 164,890,32,32 }
#define ABAIXDRETA
#define ABAIXESQUERRA
#define ABAIXESQUERRADRETA
#define TANCAT { 358,472,32,32 }

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

	GenerateWalls();

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
				visited.push_back(auxNode);
				i++;
			}
		}
		else if ((randNum == 4 || randNum == 5 || randNum == 6) && CheckBoundaries({ auxNode->pos.x - 1, auxNode->pos.y }))
		{
			auxNode = nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)];
			if (auxNode->whatToBlit.x != 260)
			{
				auxNode->whatToBlit = FLOOR;
				visited.push_back(auxNode);
				i++;
			}
		}
		else if ((randNum == 7 || randNum == 8) && CheckBoundaries({ auxNode->pos.x, auxNode->pos.y + 1 }))
		{
			auxNode = nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)];
			if (auxNode->whatToBlit.x != 260)
			{
				auxNode->whatToBlit = FLOOR;
				visited.push_back(auxNode);
				i++;
			}
		}
		else if ((randNum == 9 || randNum == 10) && CheckBoundaries({ auxNode->pos.x, auxNode->pos.y - 1 }))
		{
			auxNode = nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)];
			if (auxNode->whatToBlit.x != 260)
			{
				auxNode->whatToBlit = FLOOR;
				visited.push_back(auxNode);
				i++;
			}
		}
	}

	auxNode = nullptr;

	return true;
}

int MapGenerator::CheckTypeOfNode(MapNode* nodetocheck)
{
	bool adalt, abaix, dreta, esquerra;
	adalt = abaix = dreta = esquerra = false;

	// si existeix...
	if (CheckBoundaries({ nodetocheck->pos.x,nodetocheck->pos.y + 1 }))
	{
		// si no son terra...
		if (nodes[Get(nodetocheck->pos.x, nodetocheck->pos.y + 1)]->whatToBlit.x == 260)
		{ 
			abaix = true;

		}
	}

	// si existeix...
	if (CheckBoundaries({ nodetocheck->pos.x,nodetocheck->pos.y - 1 }))
	{
		// si no son terra...
		if (nodes[Get(nodetocheck->pos.x, nodetocheck->pos.y - 1)]->whatToBlit.x == 260)
		{ 
			adalt = true;
		}
	}

	// si existeix...
	if (CheckBoundaries({ nodetocheck->pos.x +1,nodetocheck->pos.y }))
	{
		// si no son terra...
		if (nodes[Get(nodetocheck->pos.x + 1, nodetocheck->pos.y)]->whatToBlit.x == 260)
		{
			dreta = true;
		}
	}

	// si existeix...
	if (CheckBoundaries({ nodetocheck->pos.x - 1,nodetocheck->pos.y }))
	{
		// si no son terra...
		if (nodes[Get(nodetocheck->pos.x - 1, nodetocheck->pos.y)]->whatToBlit.x == 260)
		{
			esquerra = true;
		}
	}

	if (adalt && esquerra && dreta && abaix)
		return (int)nodeType::typeFully;
	else if (adalt && esquerra && dreta && !abaix)
		return (int)nodeType::typeUpLeftRight;
	else if (!adalt && esquerra && dreta && abaix)
		return (int)nodeType::typeDownLeftRight;
	else if (adalt && esquerra && !dreta && abaix)
		return (int)nodeType::typeUpDownLeft;
	else if (adalt && !esquerra && dreta && abaix)
		return (int)nodeType::typeUpDownRight;
	else if (adalt && esquerra && !dreta && !abaix)
		return (int)nodeType::typeUpLeft;
	else if (adalt && !esquerra && dreta && !abaix)
		return (int)nodeType::typeUpRight;
	else if (!adalt && esquerra && !dreta && abaix)
		return (int)nodeType::typeDownLeft;
	else if (!adalt && !esquerra && dreta && abaix)
		return (int)nodeType::typeDownRight;
	else if (adalt && !esquerra && !dreta && abaix)
		return (int)nodeType::typeUpDown;
	else if (!adalt && esquerra && dreta && !abaix)
		return (int)nodeType::typeLeftRight;
	else if (adalt && !esquerra && !dreta && !abaix)
		return (int)nodeType::typeUp;
	else if (!adalt && esquerra && !dreta && !abaix)
		return (int)nodeType::typeLeft;
	else if (!adalt && !esquerra && dreta && !abaix)
		return (int)nodeType::typeRight;
	else if (!adalt && !esquerra && !dreta && abaix)
		return (int)nodeType::typeDown;
	else
		return (int)nodeType::noType;
}


void MapGenerator::UpdateNode(MapNode* nodetocheck, int type)
{
	if (type == (int)nodeType::typeDown)
		nodetocheck->whatToBlit = ADALT;
	else if (type == (int)nodeType::typeUp)
		nodetocheck->whatToBlit = ABAIX;
	else if (type == (int)nodeType::typeLeft)
		nodetocheck->whatToBlit = DRETA;
	else if (type == (int)nodeType::typeRight)
		nodetocheck->whatToBlit = ESQUERRA;
	else if (type == (int)nodeType::typeFully)
		nodetocheck->whatToBlit = TANCAT;
	else if (type == (int)nodeType::typeUpRight)
		nodetocheck->whatToBlit = ADALTDRETA;
	else if (type == (int)nodeType::typeUpLeftRight)
		nodetocheck->whatToBlit = ADALTESQUERRADRETA;
//	else if (type == (int)nodeType::typeDownLeftRight)
		//nodetocheck->whatToBlit = ABAIXESQUERRADRETA;
	

}

bool MapGenerator::GenerateWalls()
{
	for (uint i = 0u; i < visited.size(); ++i)
	{
		if (visited[i]->whatToBlit.x == 260)
		{
			MapNode* auxNode = visited[i];

			if (CheckBoundaries({ auxNode->pos.x, auxNode->pos.y - 1 }))
			{
				if (nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->whatToBlit.x == 292)
				{
					//Check type of node adn Update
					int type = CheckTypeOfNode(nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]);
					UpdateNode(nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)], type);
				}
			}

			if (CheckBoundaries({ auxNode->pos.x, auxNode->pos.y + 1}))
			{
				if (nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]->whatToBlit.x == 292)
				{
					//Check type of node adn Update
					int type = CheckTypeOfNode(nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]);
					UpdateNode(nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)], type);
				}
			}

			if (CheckBoundaries({ auxNode->pos.x + 1, auxNode->pos.y }))
			{
				if (nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]->whatToBlit.x == 292)
				{
					//Check type of node adn Update
					int type = CheckTypeOfNode(nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]);
					UpdateNode(nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)], type);
				}
			}

			if (CheckBoundaries({ auxNode->pos.x - 1, auxNode->pos.y }))
			{
				if (nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)]->whatToBlit.x == 292)
				{
					//Check type of node adn Update
					int type = CheckTypeOfNode(nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)]);
					UpdateNode(nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)], type);
				}
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