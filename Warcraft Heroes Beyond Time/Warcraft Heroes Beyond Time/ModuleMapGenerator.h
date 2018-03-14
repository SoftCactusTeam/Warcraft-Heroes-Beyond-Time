#ifndef __MODULEMAPGENERATOR_H__
#define __MODULEMAPGENERATOR_H__

#include "Module.h"
#include "SDL\include\SDL_rect.h"
#include "p2Point.h"
#include <vector>
#include <list>

class Module;

struct MapNode
{
	MapNode(iPoint pos, SDL_Rect whatToBlit) : pos(pos), whatToBlit(whatToBlit) {};
	~MapNode() {};

	iPoint pos = { 0,0 };
	SDL_Rect whatToBlit = { 0,0,0,0 };
};

class MapGenerator : public Module
{
public:
	enum class nodeType
	{
		noType = -1,
		typeLeft,
		typeRight,
		typeUp,
		typeDown,
		typeUpLeft,
		typeUpRight,
		typeLeftRight,
		typeUpDown,
		typeUpLeftRight,
		typeDownLeft,
		typeDownRight,
		typeDownLeftRight,
		typeUpDownLeft,
		typeUpDownRight,
		typeFully

	};

public:
	MapGenerator();
	~MapGenerator();
	inline int Get(int x, int y) const;
	bool CheckBoundaries(const iPoint& pos) const;
	bool GenerateGrid(int sizeX, int sizeY);
	bool ExecuteAlgorithm(MapNode* startNode, uint iterations);
	int CheckTypeOfNode(MapNode* nodetocheck);
	void UpdateNode(MapNode* nodetocheck, int type);
	bool GenerateWalls();
	bool Update(float dt);
	bool CleanMap();
	bool CleanUp();

private:

	uint totalSize = 0u;
	uint sizeX = 0u;
	uint sizeY = 0u;
	std::vector<MapNode*> nodes;
	std::vector<MapNode*> visited;
	SDL_Texture* mapTexture = nullptr;

};

#endif
