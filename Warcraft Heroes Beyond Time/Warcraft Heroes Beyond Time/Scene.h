#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "p2Point.h"

#include "Animation.h"

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
	void GeneratePortal();

public:

	virtual bool OnUIEvent(GUIElem* UIelem, UIEvents _event);
	void CreateMainMenuScreen();
	void CreateSettingsScreen();
	void CreatePauseMenu();
	void CreateGratitudeScreen();
	void CreateItemSelectionScreen();
	bool gratitudeON = false;
	void GoMainMenu();
	void GoBossRoom();

public:
	enum class Stages
	{
		NO_STAGE,
		MAIN_MENU,
		SETTINGS,
		INGAME,
		BOSS_ROOM

	} actual_scene = Stages::MAIN_MENU;

	PlayerEntity* player = nullptr;
	ChestEntity* lvlChest = nullptr;
	bool paused = false;

	GUIElem* player_HP_Bar = nullptr;
	PortalEntity* portal = nullptr;
	GUIWindow* ItemSelection = nullptr;

private:

	GUIWindow* PauseMenu = nullptr;

	uint currentPercentAudio = 0u;
	

	bool restart = false;
	int lvlIndex = 0;

	int seed = NULL;
};


#endif