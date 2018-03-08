#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Module.h"
#include "Globals.h"
#include <list>
#include <string>

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

private:

	void PrepareUpdate();
	void FinishUpdate();
	bool PreUpdate();
	bool DoUpdate();
	bool PostUpdate();

public:

	Window*					window = nullptr;
	Input*					input = nullptr;
	Render*					render = nullptr;
	Textures*				textures = nullptr;
	Map*					map = nullptr;
	Fonts*					fonts = nullptr;
	EntitySystem*			entities = nullptr;
	FileSystem*				fs = nullptr;
	ModuleGUI*				gui = nullptr;
	Audio*					audio = nullptr;
	Scene*					scene = nullptr;
	Console*				console = nullptr;

public:

	float dt = 0.0f;

private:

	std::list<Module*> modules;
	int					argc;
	char**				args;

	std::string			title;
	std::string			organization;

};

extern Application* App;

#endif