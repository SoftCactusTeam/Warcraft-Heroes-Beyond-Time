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
	bool Update(float dt);
	bool PostUpdate();
	bool DrawPrePlayerMap();
	bool DrawPostPlayerMap();
	inline int Get(int x, int y) const;
	bool CheckBoundaries(const iPoint& pos) const;
	bool GenerateMap(MapData data);
	bool ExecuteAlgorithm(MapNode* startNode, uint iterations, int seed);
	int CheckTypeOfNode(MapNode* nodetocheck);
	void UpdateNode(MapNode* nodetocheck, int type);
	bool GenerateWalls();
	bool CleanUp();

private:

	uint totalSize = 0u;
	uint sizeX = 0u;
	uint sizeY = 0u;
	uint tileSize = 32;
	std::vector<MapNode*> nodes;
	std::vector<MapNode*> visited;
	SDL_Texture* mapTexture = nullptr;

};

#endif
