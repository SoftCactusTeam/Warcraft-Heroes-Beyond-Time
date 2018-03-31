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
	int cost = -1;
	SDL_Rect whatToBlit = { 0,0,0,0 };
	bool floor = false;
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

	void Init()
	{
		active = false;
	}
	bool Update(float dt);
	bool DrawMap() const;
	inline int Get(int x, int y) const;
	bool CheckBoundaries(const iPoint& pos) const;
	bool GenerateMap(MapData data);
	bool ExecuteAlgorithm(MapNode* startNode, uint iterations, int seed);
	SDL_Rect randomTile(bool isFloor);
	bool GenerateWalls();
	bool CleanUp();
	iPoint GetRandomValidPoint();

	void getSize(uint& w, uint& h);
	int getTileSize();

	std::vector<MapNode*> GetMapNodesAndInfo(uint& sizeX, uint& sizeY, uint& tileSize);

private:

	uint totalSize = 0u;
	uint sizeX = 0u;
	uint sizeY = 0u;
	uint tileSize = 48;
	std::vector<MapNode*> nodes;
	std::vector<MapNode*> visited;
	SDL_Texture* mapTexture = nullptr;

};

#endif
