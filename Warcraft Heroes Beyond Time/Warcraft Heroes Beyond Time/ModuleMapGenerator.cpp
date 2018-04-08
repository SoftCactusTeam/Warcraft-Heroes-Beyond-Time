#include "Application.h"
#include "ModuleMapGenerator.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePrinter.h"
#include "Log.h"
#include "Scene.h"
#include "PlayerEntity.h"
#include "ModuleColliders.h"
#include "Brofiler\Brofiler.h"
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
	BROFILER_CATEGORY("ClearMAP", Profiler::Color::Chocolate);
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
	BROFILER_CATEGORY("GetRandomValidPoint", Profiler::Color::Chocolate);
	int randNum = 0;

	do
		randNum = rand() % (nodes.size() - 0 + 1);
	while (!nodes[randNum]->layerBelow || nodes[randNum]->pos == nodes[Get(sizeX / 2, sizeY / 2)]->pos);

	return nodes[randNum]->pos;
}

bool MapGenerator::PostUpdate()
{
	return DrawMap();
}

bool MapGenerator::DrawMap() const
{
	bool ret = true;

	BROFILER_CATEGORY("Map Draw", Profiler::Color::Azure);

	/*int x = -(tileSize - 2), y = 0;
	
	for (int i = 0u; i < totalSize && ret; ++i)
	{
		if (i % 50 == 0 && i != 0)
		{
			x = 0;
			y += tileSize - 2;
		}
		else
			x += tileSize - 2;

		if (x >= (-1 * App->render->camera.x) - tileSize &&
			y >= (-1 * App->render->camera.y) - tileSize &&
			x < -App->render->camera.x + App->render->camera.w + tileSize &&
			y < -App->render->camera.y + App->render->camera.h + tileSize)
		{
			ret = App->printer->PrintSprite({ x , y }, mapTexture, nodes[i]->whatToBlit, nodes[i]->layerBelow);
		}
	}*/

	
	for (int index_y = 0; index_y < sizeY && ret; ++index_y)
	{
		for (int index_x = 0; index_x < sizeX && ret; ++index_x)
		{
			int x = index_x * (tileSize - 2);
			int y = index_y * (tileSize - 2);

			if (x >= (-1 * App->render->camera.x) - tileSize &&
				y >= (-1 * App->render->camera.y) - tileSize &&
				x < -App->render->camera.x + App->render->camera.w + tileSize &&
				y < -App->render->camera.y + App->render->camera.h + tileSize)
			
				ret = App->printer->PrintSprite({ x, y }, mapTexture, nodes[index_x + index_y * sizeX]->whatToBlit, nodes[index_x + index_y * sizeX]->layerBelow);

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
	BROFILER_CATEGORY("Map Generation", Profiler::Color::Azure);

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

	for (uint i = 0u; i < totalSize; ++i)
	{
		if (SDL_RectEquals(&nodes[i]->whatToBlit, &SDL_Rect(VOID)))
			nodes[i]->layerBelow = 1;
	}

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
	startNode->layerBelow = -1;
	startNode->cost = -1;
	visited.push_back(startNode);

	MapNode* auxNode = startNode;

	for (uint i = 0u; i < iterations;)
	{

		uint randNum = rand() % (10 + 1);

		if ((randNum == 0 || randNum == 1 || randNum == 3) && CheckBoundaries({ auxNode->pos.x + 1, auxNode->pos.y }))
		{
			auxNode = nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)];
			if (auxNode->layerBelow != -1)
			{
				auxNode->whatToBlit = randomTile(true);
				auxNode->cost = -1;
				visited.push_back(auxNode);
				auxNode->layerBelow = -1;
				i++;
			}
		}

		else if ((randNum == 4 || randNum == 5 || randNum == 6) && CheckBoundaries({ auxNode->pos.x - 1, auxNode->pos.y }))
		{
			auxNode = nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)];
			if (auxNode->layerBelow != -1)
			{
				auxNode->whatToBlit = randomTile(true);
				auxNode->cost = -1;
				visited.push_back(auxNode);
				auxNode->layerBelow = -1;
				i++;
			}
		}

		else if ((randNum == 7 || randNum == 8) && CheckBoundaries({ auxNode->pos.x, auxNode->pos.y + 1 }))
		{
			auxNode = nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)];
			if (auxNode->layerBelow != -1)
			{
				auxNode->whatToBlit = randomTile(true);
				auxNode->cost = -1;
				visited.push_back(auxNode);
				auxNode->layerBelow = -1;
				i++;
			}
		}

		else if ((randNum == 9 || randNum == 10) && CheckBoundaries({ auxNode->pos.x, auxNode->pos.y - 1 }))
		{
			auxNode = nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)];
			if (auxNode->layerBelow != -1)
			{
				auxNode->whatToBlit = randomTile(true);
				auxNode->cost = -1;
				visited.push_back(auxNode);
				auxNode->layerBelow = -1;
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
		int randNum = rand() % (100 - 1 + 1) + 1;

		if (randNum <= 2)
			toReturn = FLOOR6;
		else if (randNum <= 4)
			toReturn = FLOOR5;		
		else if (randNum <= 7)
			toReturn = FLOOR4;
		else if (randNum <= 10)
			toReturn = FLOOR3;
		else if (randNum <= 13)
			toReturn = FLOOR2;
		else if (randNum <= 100)
			toReturn = FLOOR;
		
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
		if (visited[i]->layerBelow)
		{
			MapNode* auxNode = visited[i];

				if (SDL_RectEquals(&nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->whatToBlit, &SDL_Rect(VOID)))
				{
					nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->whatToBlit = randomTile(false);
									
					if (!nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->colliderInside)
					{
						App->colliders->AddCollider(SDL_Rect({ auxNode->pos.x * (int)(tileSize-2), auxNode->pos.y * (int)(tileSize - 2) - (int)(tileSize - 2), 48,48 }), COLLIDER_TYPE::COLLIDER_UNWALKABLE);
						nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->colliderInside = true;
					}
				}

				if (SDL_RectEquals(&nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]->whatToBlit, &SDL_Rect(VOID)))
				{
					if (!nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]->colliderInside)
					{
						App->colliders->AddCollider(SDL_Rect({ auxNode->pos.x * (int)(tileSize - 2), auxNode->pos.y * (int)(tileSize - 2) + (int)(tileSize - 2), 48,48 }), COLLIDER_TYPE::COLLIDER_UNWALKABLE);
						nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]->colliderInside = true;
					}
				}

				if (SDL_RectEquals(&nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]->whatToBlit, &SDL_Rect(VOID)))
				{
					if (!nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]->colliderInside)
					{
						App->colliders->AddCollider(SDL_Rect({ auxNode->pos.x * (int)(tileSize - 2) + (int)(tileSize - 2), auxNode->pos.y * (int)(tileSize - 2), 48,48 }), COLLIDER_TYPE::COLLIDER_UNWALKABLE);
						nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]->colliderInside = true;
					}
				}

				if (SDL_RectEquals(&nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)]->whatToBlit, &SDL_Rect(VOID)))
				{
					if (!nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)]->colliderInside)
					{
						App->colliders->AddCollider(SDL_Rect({ auxNode->pos.x * (int)(tileSize - 2) - (int)(tileSize - 2), auxNode->pos.y * (int)(tileSize - 2), 48,48 }), COLLIDER_TYPE::COLLIDER_UNWALKABLE);
						nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)]->colliderInside = true;
					}
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
