#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

class GUIWindow;
class PlayerEntity;
class ChestEntity;
class PortalEntity;

class Scene : public Module
{
public:

	Scene();
	virtual ~Scene();

	bool Awake(pugi::xml_node& consoleNode);
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

	GUIWindow* PauseMenu = nullptr;

	bool restart = false;

	int lvlIndex = 0;

	ChestEntity* lvlChest = nullptr;
	PortalEntity* portal = nullptr;

public:
	PlayerEntity* player = nullptr;

	bool paused = false;

};


#endif