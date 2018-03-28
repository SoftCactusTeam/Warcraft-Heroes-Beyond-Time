#include "Application.h"
#include "ModuleMapGenerator.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "Log.h"

#include <time.h>

#define FLOOR { 0,48,48,48 }
#define WATER { 48,0,48,48 }
#define WALL { 0,0,48,48 }

MapGenerator::MapGenerator() {}

MapGenerator::~MapGenerator() {}

bool MapGenerator::Update(float dt)
{
	bool ret = true;

	//ret = DrawPostPlayerMap();
	
	return ret;
}

bool MapGenerator::PostUpdate()
{
	bool ret = true;
	
	

	return ret;
}

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
	while (!SDL_RectEquals(&nodes[randNum]->whatToBlit, &SDL_Rect(FLOOR)) || nodes[randNum]->pos == nodes[Get(sizeX / 2, sizeY / 2)]->pos);
		
	return nodes[randNum]->pos;
}

bool MapGenerator::DrawPrePlayerMap()
{
	bool ret = true;

	for (uint i = 0u; i < totalSize && ret; ++i)
	{
		if (SDL_RectEquals(&nodes[i]->whatToBlit, &SDL_Rect(FLOOR)))
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
		if (!SDL_RectEquals(&nodes[i]->whatToBlit, &SDL_Rect(FLOOR)))
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
			nodes.push_back(new MapNode({ (int)j,(int)i }, WATER));
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

	startNode->whatToBlit = FLOOR;
	visited.push_back(startNode);

	MapNode* auxNode = startNode;

	for (uint i = 0u; i < iterations;)
	{

		uint randNum = rand() % (10 + 1);

		if ((randNum == 0 || randNum == 1 || randNum == 3) && CheckBoundaries({ auxNode->pos.x + 1, auxNode->pos.y }))
		{
			auxNode = nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)];
			if (!SDL_RectEquals(&auxNode->whatToBlit, &SDL_Rect(FLOOR)))
			{
				auxNode->whatToBlit = FLOOR;
				auxNode->cost = 0;
				visited.push_back(auxNode);
				i++;
			}
		}

		else if ((randNum == 4 || randNum == 5 || randNum == 6) && CheckBoundaries({ auxNode->pos.x - 1, auxNode->pos.y }))
		{
			auxNode = nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)];
			if (!SDL_RectEquals(&auxNode->whatToBlit, &SDL_Rect(FLOOR)))
			{
				auxNode->whatToBlit = FLOOR;
				auxNode->cost = 0;
				visited.push_back(auxNode);
				i++;
			}
		}

		else if ((randNum == 7 || randNum == 8) && CheckBoundaries({ auxNode->pos.x, auxNode->pos.y + 1 }))
		{
			auxNode = nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)];
			if (!SDL_RectEquals(&auxNode->whatToBlit, &SDL_Rect(FLOOR)))
			{
				auxNode->whatToBlit = FLOOR;
				auxNode->cost = 0;
				visited.push_back(auxNode);
				i++;
			}
		}

		else if ((randNum == 9 || randNum == 10) && CheckBoundaries({ auxNode->pos.x, auxNode->pos.y - 1 }))
		{
			auxNode = nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)];
			if (!SDL_RectEquals(&auxNode->whatToBlit, &SDL_Rect(FLOOR)))
			{
				auxNode->whatToBlit = FLOOR;
				auxNode->cost = 0;
				visited.push_back(auxNode);
				i++;
			}
		}
	}

	auxNode = nullptr;

	return visited.size() == iterations + 1;
}

bool MapGenerator::GenerateWalls()
{
	LOG("Generating Walls...");

	for (uint i = 0u; i < visited.size(); ++i)
	{
		if (SDL_RectEquals(&visited[i]->whatToBlit, &SDL_Rect(FLOOR)))
		{
			MapNode* auxNode = visited[i];

				if (SDL_RectEquals(&nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->whatToBlit, &SDL_Rect(WATER)))
				{
					nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->whatToBlit = WALL;
					//Check type of node adn Update
					//int type = CheckTypeOfNode(nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]);
					//UpdateNode(nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)], type);
				}

				if (SDL_RectEquals(&nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]->whatToBlit, &SDL_Rect(WATER)))
				{
					//Check type of node adn Update
					//int type = CheckTypeOfNode(nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]);
					//UpdateNode(nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)], type);
				}

				if (SDL_RectEquals(&nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]->whatToBlit, &SDL_Rect(WATER)))
				{
					//Check type of node adn Update
					//int type = CheckTypeOfNode(nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]);
					//UpdateNode(nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)], type);
				}

				if (SDL_RectEquals(&nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)]->whatToBlit, &SDL_Rect(WATER)))
				{
					//Check type of node adn Update
					//int type = CheckTypeOfNode(nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)]);
					//UpdateNode(nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)], type);
				}
		}
	}

	return true;
}

void MapGenerator::UpdateNode(MapNode* nodetocheck, int type)
{
	if (type == type == (int)nodeType::typeTiledWall)
		nodetocheck->whatToBlit = WALL;
	else if (type == type == (int)nodeType::typeWall)
		nodetocheck->whatToBlit = WALL;
}

int MapGenerator::CheckTypeOfNode(MapNode* nodetocheck)
{
	bool adalt, abaix, dreta, esquerra;
	adalt = abaix = dreta = esquerra = false;

	// si existeix...
	if (CheckBoundaries({ nodetocheck->pos.x,nodetocheck->pos.y + 1 }))
	{
		// si no son terra...
		if (SDL_RectEquals(&nodes[Get(nodetocheck->pos.x, nodetocheck->pos.y + 1)]->whatToBlit, &SDL_Rect(FLOOR)))
		{ 
			abaix = true;
		}
	}

	// si existeix...
	if (CheckBoundaries({ nodetocheck->pos.x, nodetocheck->pos.y - 1 }))
	{
		// si no son terra...
		if (SDL_RectEquals(&nodes[Get(nodetocheck->pos.x, nodetocheck->pos.y - 1)]->whatToBlit, &SDL_Rect(FLOOR)))
		{ 
			adalt = true;
		}
	}

	// si existeix...
	if (CheckBoundaries({ nodetocheck->pos.x +1, nodetocheck->pos.y }))
	{
		// si no son terra...
		if (SDL_RectEquals(&nodes[Get(nodetocheck->pos.x + 1, nodetocheck->pos.y)]->whatToBlit, &SDL_Rect(FLOOR)))
		{
			dreta = true;
		}
	}

	// si existeix...
	if (CheckBoundaries({ nodetocheck->pos.x - 1, nodetocheck->pos.y }))
	{
		// si no son terra...
		if (SDL_RectEquals(&nodes[Get(nodetocheck->pos.x - 1, nodetocheck->pos.y)]->whatToBlit, &SDL_Rect(FLOOR)))
		{
			esquerra = true;
		}
	}

	if (adalt && esquerra && dreta && abaix)
		return (int)nodeType::typeFully;
	else
		return (int)nodeType::noType;
}

void MapGenerator::getSize(uint& w, uint& h)
{
	w = sizeX;
	h = sizeY;
}