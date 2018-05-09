#include "Application.h"
#include "ModuleMapGenerator.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePrinter.h"
#include "Log.h"
#include "Scene.h"
#include "PlayerEntity.h"
#include "ModuleColliders.h"
#include "FileSystem.h"
#include "Brofiler\Brofiler.h"
#include "Scene.h"
#include <time.h>

#define MINDISTANCE 10

#define VOID { 192,1056,48,48 }
#define FLOOR { 0,1104,48,48 }
#define FLOOR2 { 48,1104,48,48 }
#define FLOOR3 { 98,1104,48,48 }
#define FLOOR4 { 144,1104,48,48 }
#define FLOOR5 { 192,1104,48,48 }
#define FLOOR6 { 0,1152,48,48 }
#define FLOOR7 { 48,1152,48,48 }
#define FLOOR8 { 96,1152,48,48 }
#define WALL1 { 0,1056,48,48 }
#define WALL2 { 48,1056,48,48 }
#define WALL3 { 96,1056,48,48 }
#define WALL4 { 144,1056,48,48 }

MapGenerator::MapGenerator()
{
	name = "map";
}

MapGenerator::~MapGenerator() {}

bool MapGenerator::Awake(pugi::xml_node& mapNode)
{
	mapSeed = mapNode.child("seed").attribute("value").as_int();

	for (register pugi::xml_node aux_node = mapNode.child("lvl"); aux_node; aux_node = aux_node.next_sibling("lvl"))
	{
		pugi::xml_node actualNode = aux_node.child("sizeGrid");
		gridSizePerLevel.push_back({ actualNode.attribute("x").as_int(),actualNode.attribute("y").as_int() });
		iterationsPerLevel.push_back(aux_node.child("sizeDungeon").attribute("iterations").as_int());

		actualNode = aux_node.child("archers");
		SDL_Rect archerPerLevel = { actualNode.attribute("quantity").as_int(), actualNode.child("tier1").attribute("percentage").as_int(), actualNode.child("tier2").attribute("percentage").as_int(), actualNode.child("tier3").attribute("percentage").as_int() };

		archers.push_back(archerPerLevel);

		numberOfLevels++;
	}

	return true;
}

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
		randNum = rand() % ((nodes.size()-1) + 1);
	while (nodes[randNum]->layerBelow != -2 || nodes[randNum]->pos == nodes[Get(sizeX / 2, sizeY / 2)]->pos || nodes[randNum]->pos.DistanceTo( nodes[Get(sizeX / 2, sizeY / 2)]->pos ) < MINDISTANCE);

	return nodes[randNum]->pos;
}

iPoint MapGenerator::GetRandomValidPointProxy(int distance, int proxyDistance)
{
	int randNum = 0;

	do
		randNum = rand() % ((nodes.size() - 1) + 1);
	while (nodes[randNum]->layerBelow != -2 || nodes[randNum]->pos == nodes[Get(sizeX / 2, sizeY / 2)]->pos || nodes[randNum]->pos.DistanceTo(nodes[Get(sizeX / 2, sizeY / 2)]->pos) > distance || nodes[randNum]->pos.DistanceTo(nodes[Get(sizeX / 2, sizeY / 2)]->pos) < proxyDistance);

	return nodes[randNum]->pos;
}

iPoint MapGenerator::GetRandomBossPoint()
{
	int randNum = 0;

	do
		randNum = rand() % ((nodes.size() - 1) + 1);
	while (nodes[randNum]->layerBelow != -2);

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

SDL_Rect MapGenerator::GetTileRect(int id) const
{
	int relative_id = id - 1;

	SDL_Rect rect;
	rect.w = tileSize;
	rect.h = tileSize;
	rect.x = ((rect.w) * (relative_id % 10));
	rect.y = ((rect.h) * (relative_id / 10));

	return rect;
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

	LOG("Generating map Grid...");

	totalSize = sizeX * sizeY;

	for (uint i = 0u; i < sizeY; ++i)
	{
		for (uint j = 0u; j < sizeX; ++j)
		{
			nodes.push_back(new MapNode({ (int)j,(int)i }, VOID));
		}
	}
	ret = nodes.size() == totalSize;

	//if (ret)
		ret = GenerateChestMap();

//	if (ret)
		ret = ExecuteAlgorithm(AlgorithmStart, data.iterations, data.seed);

	//if (ret)
		ret = GenerateWalls();

	for (uint i = 0u; i < totalSize; ++i)
	{
		if (SDL_RectEquals(&nodes[i]->whatToBlit, &SDL_Rect(VOID)))
			nodes[i]->layerBelow = 1;
	}

	return true;
}

bool MapGenerator::GenerateBossMap()
{
	pugi::xml_document doc_map;
	pugi::xml_node map_child;

	char* buffer;
	uint size = App->fs->Load("maps/guldanMap.tmx", &buffer);
	doc_map.load_buffer(buffer, size);
	RELEASE(buffer);

	map_child = doc_map.child("map");
	
	sizeX = map_child.attribute("width").as_uint();
	sizeY = map_child.attribute("height").as_uint();

	for (int y = 0; y < sizeY; ++y)
	{
		for (int x = 0; x < sizeX; ++x)
			nodes.push_back(new MapNode({ x,y }, { 0,0,0,0 }));
	}

	pugi::xml_node sub_map_node = map_child.child("tileset");
	tileSize = sub_map_node.attribute("tilewidth").as_int();

	int contNodes = 0;

	for (pugi::xml_node layer = map_child.child("layer"); layer; layer = layer.next_sibling("layer"))
	{
		for (pugi::xml_node tile_gid = layer.child("data").child("tile"); tile_gid; tile_gid = tile_gid.next_sibling("tile"))
		{
			int gid = tile_gid.attribute("gid").as_int();

			std::string layerName = layer.attribute("name").as_string();
			
			if (layerName == "Tiles")
			{
				nodes[contNodes]->whatToBlit = GetTileRect(gid);
			}
			else if (layerName == "Colliders")
			{
				if (gid == 32)
				{
					App->colliders->AddCollider(SDL_Rect({ nodes[contNodes]->pos.x * (int)(tileSize - 2), (nodes[contNodes]->pos.y + 1) * (int)(tileSize - 2) - (int)(tileSize - 2), 48,48 }), Collider::ColliderType::WALL);
				}
			}
			else if (layerName == "Layers")
			{
				if (gid == 32)
					nodes[contNodes]->layerBelow = 2;
				else if (gid == 33)
					nodes[contNodes]->layerBelow = 0;
				else
					nodes[contNodes]->layerBelow = -2;
			}
			contNodes++;
		}
		contNodes = 0;
	}

	mapTexture = App->textures->Load("maps/tiles_boss.png");

	return true;
}

bool MapGenerator::ExecuteAlgorithm(MapNode* startNode, uint iterations, int seed)
{
	LOG("Executing map Algorithm...");

	if (seed != 0)
		srand(seed);
	else
		srand(time(NULL));

	startNode->whatToBlit = randomTile(true);
	startNode->layerBelow = -2;
	startNode->cost = -1;
	//visited.push_back(startNode);

	MapNode* auxNode = startNode;

	for (uint i = 0u; i < iterations;)
	{
 		uint randNum = rand() % (10 + 1);

		if ((randNum == 0 || randNum == 1 || randNum == 3) && CheckBoundaries({ auxNode->pos.x + 1, auxNode->pos.y }) && !nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]->InvalidForMap)
		{
			auxNode = nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)];
			if (!auxNode->layerBelow != -2)
			{
				auxNode->whatToBlit = randomTile(true);
				auxNode->cost = -1;
				visited.push_back(auxNode);
				auxNode->layerBelow = -2;
				i++;
			}
		}

		else if ((randNum == 4 || randNum == 5 || randNum == 6) && CheckBoundaries({ auxNode->pos.x - 1, auxNode->pos.y }) && !nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)]->InvalidForMap)
		{
			auxNode = nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)];
			if (!auxNode->layerBelow != -2)
			{
				auxNode->whatToBlit = randomTile(true);
				auxNode->cost = -1;
				visited.push_back(auxNode);
				auxNode->layerBelow = -2;
				i++;
			}
		}

		else if ((randNum == 7 || randNum == 8) && CheckBoundaries({ auxNode->pos.x, auxNode->pos.y + 1 }) && !nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]->InvalidForMap)
		{
			auxNode = nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)];
			if (!auxNode->layerBelow != -2)
			{
				auxNode->whatToBlit = randomTile(true);
				auxNode->cost = -1;
				visited.push_back(auxNode);
				auxNode->layerBelow = -2;
				i++;
			}
		}

		else if ((randNum == 9 || randNum == 10) && CheckBoundaries({ auxNode->pos.x, auxNode->pos.y - 1 }) && !nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->InvalidForMap)
		{
			auxNode = nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)];
			if (auxNode->layerBelow != -2)
			{
				auxNode->whatToBlit = randomTile(true);
				auxNode->cost = -1;
				visited.push_back(auxNode);
				auxNode->layerBelow = -2;
				i++;
			}
		}
	}

	auxNode = nullptr;

	return visited.size() == iterations + 1;
}

bool MapGenerator::GenerateChestMap()
{
	pugi::xml_document doc_map;
	pugi::xml_node map_child;

	char* buffer;
	uint size = App->fs->Load("maps/ChestRoom.tmx", &buffer);
	doc_map.load_buffer(buffer, size);
	RELEASE(buffer);

	map_child = doc_map.child("map");

	int chestSizeX = map_child.attribute("width").as_uint();
	int chestSizeY = map_child.attribute("height").as_uint();

	MapNode* firstTile = nodes[Get(sizeX / 2 - chestSizeX/2, sizeY / 2)];

	pugi::xml_node sub_map_node = map_child.child("tileset");
	tileSize = sub_map_node.attribute("tilewidth").as_int();

	for (pugi::xml_node layer = map_child.child("layer"); layer; layer = layer.next_sibling("layer"))
	{
		pugi::xml_node tile_gid = layer.child("data").child("tile");

		for (int i = 0; i < chestSizeY; ++i)
		{
			for (int j = 0; j < chestSizeX; ++j)
			{
				//tile_gid = tile_gid.next_sibling("tile");

				int gid = tile_gid.attribute("gid").as_int();
				std::string layerName = layer.attribute("name").as_string();

				if (layerName == "Tiles")
				{
					nodes[Get(firstTile->pos.x + j, firstTile->pos.y + i)]->whatToBlit = GetTileRect(gid);

					if (gid != 225)
					{
						visited.push_back(nodes[Get(firstTile->pos.x + j, firstTile->pos.y + i)]);
					}
				}
				else if (layerName == "NoRandom")
				{
					if (gid == 34)
						nodes[Get(firstTile->pos.x + j, firstTile->pos.y + i)]->InvalidForMap = true;
				}
				else if (layerName == "StartNode")
				{
					if (gid == 32)
						begginingNode = nodes[Get(firstTile->pos.x + j, firstTile->pos.y + i)];
				
				}
				else if (layerName == "Layers")
				{
					if (gid == 32)
						nodes[Get(firstTile->pos.x + j, firstTile->pos.y + i)]->layerBelow = 2;
					else if (gid == 33)
						nodes[Get(firstTile->pos.x + j, firstTile->pos.y + i)]->layerBelow = 0;
					else
						nodes[Get(firstTile->pos.x + j, firstTile->pos.y + i)]->layerBelow = -2;
				}
				else if (layerName == "Algorithm")
				{
					if (gid == 32)
						AlgorithmStart = nodes[Get(firstTile->pos.x + j, firstTile->pos.y + i)];
				}
				else if (layerName == "Chest")
				{
					if (gid == 32)
						chestNode = nodes[Get(firstTile->pos.x + j, firstTile->pos.y + i)];
				}
				tile_gid = tile_gid.next_sibling("tile");
			}
		}			
	}

	return true;
}



SDL_Rect MapGenerator::randomTile(bool isFloor)
{
	SDL_Rect toReturn = VOID;

	if (isFloor)
	{
		int randNum = rand() % (100 - 1 + 1) + 1;

	    if (randNum <= 3)
			toReturn = FLOOR8;
		else if (randNum <= 10)
			toReturn = FLOOR7;
		else if (randNum <= 20)
			toReturn = FLOOR6;
		else if (randNum <= 30)
			toReturn = FLOOR5;		
		else if (randNum <= 40)
			toReturn = FLOOR4;
		else if (randNum <= 60)
			toReturn = FLOOR3;
		else if (randNum <= 80)
			toReturn = FLOOR2;
		else if (randNum <= 100)
			toReturn = FLOOR;
		
	}
	else
	{
		int randNum = rand() % (4 - 1 + 1) + 1;

		if (randNum == 1)
			toReturn = WALL1;
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
		if (visited[i]->layerBelow == -2)
		{
			MapNode* auxNode = visited[i];

				if (SDL_RectEquals(&nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->whatToBlit, &SDL_Rect(VOID)))
				{
					nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->whatToBlit = randomTile(false);
									
					if (!nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->colliderInside)
					{
						App->colliders->AddCollider(SDL_Rect({ auxNode->pos.x * (int)(tileSize-2), auxNode->pos.y * (int)(tileSize - 2) - (int)(tileSize - 2), 48,48 }), Collider::ColliderType::WALL);
						nodes[Get(auxNode->pos.x, auxNode->pos.y - 1)]->colliderInside = true;
					}
				}

				if (SDL_RectEquals(&nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]->whatToBlit, &SDL_Rect(VOID)))
				{
					if (!nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]->colliderInside)
					{
						App->colliders->AddCollider(SDL_Rect({ auxNode->pos.x * (int)(tileSize - 2), auxNode->pos.y * (int)(tileSize - 2) + (int)(tileSize - 2), 48,48 }), Collider::ColliderType::WALL);
						nodes[Get(auxNode->pos.x, auxNode->pos.y + 1)]->colliderInside = true;
					}
				}

				if (SDL_RectEquals(&nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]->whatToBlit, &SDL_Rect(VOID)))
				{
					if (!nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]->colliderInside)
					{
						App->colliders->AddCollider(SDL_Rect({ auxNode->pos.x * (int)(tileSize - 2) + (int)(tileSize - 2), auxNode->pos.y * (int)(tileSize - 2), 48,48 }), Collider::ColliderType::WALL);
						nodes[Get(auxNode->pos.x + 1, auxNode->pos.y)]->colliderInside = true;
					}
				}

				if (SDL_RectEquals(&nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)]->whatToBlit, &SDL_Rect(VOID)))
				{
					if (!nodes[Get(auxNode->pos.x - 1, auxNode->pos.y)]->colliderInside)
					{
						App->colliders->AddCollider(SDL_Rect({ auxNode->pos.x * (int)(tileSize - 2) - (int)(tileSize - 2), auxNode->pos.y * (int)(tileSize - 2), 48,48 }), Collider::ColliderType::WALL);
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
	return (int)tileSize;
}

std::vector<MapNode*> MapGenerator::GetMapNodesAndInfo(uint& sizeX, uint& sizeY, uint& tileSize)
{
	sizeX = this->sizeX;
	sizeY = this->sizeY;
	tileSize = this->tileSize;
	return nodes;
}

int MapGenerator::UseYourPowerToGenerateMeThisNewMap(int lvlIndex)
{
	if (lvlIndex >= numberOfLevels)
	{
		if (!App->map->GenerateBossMap())
			return -1;
		return 0;
	}

	MapData mapInfo;

	std::list<iPoint>::iterator it = gridSizePerLevel.begin();
	std::advance(it, lvlIndex);
	mapInfo.sizeX = (*it).x;
	mapInfo.sizeY = (*it).y;

	std::list<int>::iterator it_2 = iterationsPerLevel.begin();
	std::advance(it_2, lvlIndex);
	mapInfo.iterations = (*it_2);

	mapInfo.tilesetPath = "maps/tiles_boss.png";
	mapInfo.seed = mapSeed;

	if (!App->map->GenerateMap(mapInfo))
		return -1;

	return lvlIndex + 1;
}
