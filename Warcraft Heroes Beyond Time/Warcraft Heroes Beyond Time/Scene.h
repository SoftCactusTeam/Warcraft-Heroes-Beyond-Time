#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "p2Point.h"

class GUIWindow;
class PlayerEntity;
class ChestEntity;
class PortalEntity;

class Scene : public Module
{
public:

	Scene();
	virtual ~Scene();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void AddCommands();

public:

	virtual bool OnUIEvent(GUIElem* UIelem, UIEvents _event);
	void CreateMainMenuScreen();
	void CreateSettingsScreen();
	void CreatePauseMenu();

public:
	enum class Stages
	{
		NO_STAGE,
		MAIN_MENU,
		SETTINGS,
		INGAME

	} actual_scene = Stages::MAIN_MENU;

	PlayerEntity* player = nullptr;
	bool paused = false;


private:

	GUIWindow* PauseMenu = nullptr;
	ChestEntity* lvlChest = nullptr;
	PortalEntity* portal = nullptr;

	bool restart = false;
	int lvlIndex = 0;

private:
	fPoint getPosByResolution(fPoint pos) const;

};


#endif
