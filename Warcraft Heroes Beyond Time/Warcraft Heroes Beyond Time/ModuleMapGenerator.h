#ifndef __MODULEMAPGENERATOR_H__
#define __MODULEMAPGENERATOR_H__

#include "Module.h"
#include "SDL\include\SDL_rect.h"
#include "p2Point.h"
#include <vector>
#include <list>

class Module;

struct MapData
{
	char* tilesetPath = NULL;
	int sizeX = 0;
	int sizeY = 0;
	uint iterations = 0;
	int randomize = 0;
	int seed = 0;
};

struct MapNode
{
	MapNode(iPoint pos, SDL_Rect whatToBlit) : pos(pos), whatToBlit(whatToBlit) {};
	~MapNode() {};

	iPoint pos = { 0,0 };
	int cost = 0;
	SDL_Rect whatToBlit = { 0,0,0,0 };
	int layerBelow = 0;
	bool colliderInside = false;
	bool InvalidForMap = false;
};

class MapGenerator : public Module
{
public:
	enum class nodeType
	{
		noType = -1,
		typeTiledWall,
		typeWall,
		typeFully
	};

public:
	MapGenerator();
	~MapGenerator();

	bool Awake(pugi::xml_node& node);

	void Init()
	{
		active = false;
	}
	bool PostUpdate();
	bool DrawMap() const;
	inline int Get(int x, int y) const;
	SDL_Rect MapGenerator::GetTileRect(int id) const;
	bool CheckBoundaries(const iPoint& pos) const;
	bool GenerateMap(MapData data);
	bool GenerateBossMap(); 
	bool ExecuteAlgorithm(MapNode* startNode, uint iterations, int seed);
	bool GenerateChestMap();
	SDL_Rect randomTile(bool isFloor);
	bool GenerateWalls();
	bool CleanUp();
	iPoint GetRandomValidPoint();
	iPoint GetRandomValidPointProxy(int distance, int proxyDistance);
	iPoint GetRandomValidPointProxyForThisPos(int distance, int proxyDistance, iPoint pos);
	iPoint GetRandomBossPoint();


	void getSize(uint& w, uint& h);
	int getTileSize();


	void Save(pugi::xml_node& sceneNode);
	void Load(const pugi::xml_node& sceneNode);

	std::vector<MapNode*> GetMapNodesAndInfo(uint& sizeX, uint& sizeY, uint& tileSize);

	int UseYourPowerToGenerateMeThisNewMap(int lvlIndex);
public:
	int numberOfLevels = 0;
	std::list<SDL_Rect> archers;

	MapNode* begginingNode = nullptr;
	MapNode* chestNode = nullptr;
	uint sizeX = 0u;
	uint sizeY = 0u;

public:
	int getSeed() const
	{
		return mapSeed;
	}

private:

	uint totalSize = 0u;
	int tileSize = 48;
	std::vector<MapNode*> nodes;
	std::vector<MapNode*> visited;
	SDL_Texture* mapTexture = nullptr;	
	MapNode* AlgorithmStart = nullptr;
	std::list<iPoint> gridSizePerLevel;
	std::list<int> iterationsPerLevel;
	int mapSeed = 0;
};

#endif
