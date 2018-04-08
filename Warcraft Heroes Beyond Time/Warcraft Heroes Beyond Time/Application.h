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

	void AddCommands();

private:

	bool LoadConfig(pugi::xml_document&);

	bool SaveNow() const;
	bool LoadNow();

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

public:

	float dt = 0.0f;

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
};

extern Application* App;

#endif