#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "Globals.h"
#include <list>
#include <string>

class Window;
class Input;
class Render;
class Textures;
class Map;
class FileSystem;
class Audio;
class Scene;
class Fonts;



class App
{
public:

	App(int argc, char* args[]);
	virtual ~App();

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
	FileSystem*				fs = nullptr;
	Audio*					audio = nullptr;
	Scene*					scene = nullptr;

public:

	float dt = 0.0f;

private:

	std::list<Module*> modules;
	int					argc;
	char**				args;

	std::string			title;
	std::string			organization;

};

extern App* Application;

#endif