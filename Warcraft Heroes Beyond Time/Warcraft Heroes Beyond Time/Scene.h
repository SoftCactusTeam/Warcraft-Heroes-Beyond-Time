#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "p2Point.h"
#include "ModuleEntitySystem.h"
#include "Emitter.h"

#include "Animation.h"


class GUIWindow;
class PlayerEntity;
class ChestEntity;
class PortalEntity;
class Guldan;
class Item;
class Label;


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

	void Save(pugi::xml_node&);
	void Load(const pugi::xml_node&);

	void AddCommands();
	void GeneratePortal();

public:

	virtual bool OnUIEvent(GUIElem* UIelem, UIEvents _event);
	void CreateMainMenuScreen();
	void CreateSettingsScreen();
	void CreatePauseMenu();
	void CreateGratitudeScreen();
	void CreateItemSelectionScreen(Item*, Item*, Item*);
	bool gratitudeON = false;
	void GoMainMenu();
	void Restart()
	{
		restart = true;
	}

	void GoNextLevel();

public:
	enum class Stages
	{
		NO_STAGE,
		INTRO_VIDEO,
		MAIN_MENU,
		SETTINGS,
		INGAME,
		LOAD_INGAME,

	} actual_scene = Stages::INTRO_VIDEO, next_scene = Stages::INTRO_VIDEO;

	void SetScene(const Stages sceneTOSet)
	{
		actual_scene = sceneTOSet;
	}

	PlayerEntity* player = nullptr;
	EntitySystem::PlayerStats playerStats;

	ChestEntity* lvlChest = nullptr;
	Guldan* guldan = nullptr;
	bool paused = false;

	GUIElem* player_HP_Bar = nullptr;
	PortalEntity* portal = nullptr;

	GUIWindow* ItemSelection = nullptr;
	GUIWindow* PauseMenu = nullptr;

	GUIWindow* blood = nullptr;

	Label* attackBinding = nullptr;
	Label* skillBinding = nullptr;
	Label* dashBinding = nullptr;

private:
	// BG stuff
	SDL_Texture * textureBG = nullptr;
	SDL_Rect rectBG = { 0, 0, 640, 360 };
	Animation thrallShadowAnim;
	Animation boltAnim;
	Animation titleAnim;

	Emitter* testEmitter = nullptr;
	Emitter* bgEmitter1 = nullptr;
	Emitter* bgEmitter2 = nullptr;

	uint currentPercentAudio = 0u;

	bool restart = false;
	bool printButtons = false;
	int lvlIndex = 0;
};

#endif