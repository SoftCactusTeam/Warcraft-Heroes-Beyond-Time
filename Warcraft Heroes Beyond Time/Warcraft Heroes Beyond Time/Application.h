#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Module.h"
#include "Globals.h"
#include <list>
#include <string>
#include "PugiXml/src/pugixml.hpp"
#include "Timer.h"


class Window;
class Input;
class Render;
class Textures;
class Map;
class EntitySystem;
class FileSystem;
class Fonts;
class ModuleGUI;
class Audio;
class Scene;
class Fonts;
class Console;
class MapGenerator;
class ModuleColliders;
class Pathfinding;
class ModulePrinter;
class ModuleItems;
class ModuleProjectiles;
class ModuleEffects;
class ModuleTransitions;
class ModuleVideo; 
class ParticleSystem;

class Application
{
public:

	Application(int argc, char* args[]);
	virtual ~Application();

	bool Awake();
	bool Start();
	bool Update();	
	bool CleanUp();

	void AddModule(Module* module);

	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void Save();
	void Load();
	void SaveInput();
	void LoadInput();

	void AddCommands();
	pugi::xml_node LoadEmitters(pugi::xml_document& psystem_file) const;

private:

	bool LoadConfig(pugi::xml_document&);
	bool SaveNow() const;
	bool SaveInputNow()const;
public:
	bool LoadNow();
	bool LoadInputNow();

private:

	bool PrepareUpdate();
	bool FinishUpdate();
	bool PreUpdate();
	bool DoUpdate();
	bool PostUpdate();

public:

	Window*					window = nullptr;
	Input*					input = nullptr;
	Render*					render = nullptr;
	Textures*				textures = nullptr;
	Fonts*					fonts = nullptr;
	EntitySystem*			entities = nullptr;
	FileSystem*				fs = nullptr;
	ModuleGUI*				gui = nullptr;
	Audio*					audio = nullptr;
	Scene*					scene = nullptr;
	Console*				console = nullptr;
	MapGenerator*			map = nullptr;
	ModuleColliders*		colliders = nullptr;
	Pathfinding*			path = nullptr;
	ModulePrinter*			printer = nullptr;
	ModuleItems*			items = nullptr;
	ModuleProjectiles*		projectiles = nullptr;
	ModuleEffects*			effects = nullptr;
	ModuleTransitions*		transitions = nullptr;
	ModuleVideo*			video = nullptr;
	ParticleSystem*			psystem = nullptr;

public:

	float dt = 0.0f;
	uint winScale = 0u;

private:

	std::list<Module*>	modules;
	int					argc;
	char**				args;

	std::string			title;
	std::string			organization;

	Timer				frame_time;
	Timer				last_sec_frame_time;
	Timer				startup_time;

	uint				prev_last_sec_frame_count = 0;
	uint				last_sec_frame_count = 0;
	uint				frame_count = 0;
	float				capped_ms = 1/60;

	mutable bool savegame = false;
	bool loadgame = false;

	mutable bool saveinput = false;
	bool loadinput = false;
};

extern Application* App;

#endif