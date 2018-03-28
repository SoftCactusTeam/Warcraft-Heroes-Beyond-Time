#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "PlayerEntity.h"
#include "ChestEntity.h"
#include "StaticEntity.h"

class Scene : public Module
{
public:

	Scene();
	virtual ~Scene();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:

	virtual bool OnUIEvent(GUIElem* UIelem, UIEvents _event);
	

public:
	enum class Stages
	{
		NO_STAGE,
		MAIN_MENU, 
		SETTINGS,
		INGAME

	} actual_scene = Stages::MAIN_MENU;


private:
	bool restart = false;
	int lvlIndex = 0;

	ChestEntity* lvlChest = nullptr;
	StaticEntity* portal = nullptr;

public:
	PlayerEntity* player = nullptr;
};


#endif