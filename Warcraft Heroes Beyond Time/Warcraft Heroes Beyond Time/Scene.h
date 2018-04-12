#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "p2Point.h"

class GUIWindow;
class PlayerEntity;
class ChestEntity;
class PortalEntity;
class Item;
class WCItem;
struct SDL_rect;

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
	void GoMainMenu();

public:
	enum class Stages
	{
		NO_STAGE,
		MAIN_MENU,
		SETTINGS,
		INGAME

	} actual_scene = Stages::MAIN_MENU;

	PlayerEntity* player = nullptr;
	ChestEntity* lvlChest = nullptr;
	bool paused = false;

//item
	WCItem* paper = nullptr;
	WCItem* paper_fake = nullptr;
	SDL_Texture* venom = nullptr;
	SDL_Texture* texture = nullptr;

private:

<<<<<<< HEAD
	GUIWindow * PauseMenu = nullptr;
	ChestEntity* lvlChest = nullptr;
=======
	GUIWindow* PauseMenu = nullptr;
>>>>>>> master
	PortalEntity* portal = nullptr;

	bool restart = false;
	int lvlIndex = 0;

private:
	fPoint getPosByResolution(fPoint pos) const;

};


#endif