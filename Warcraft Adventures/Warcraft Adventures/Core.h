#ifndef __CORE_H__
#define __CORE_H__

#include "Module.h"

#include <list>
#include <string>

// Modules
class Window;
class Input;
class Render;
class Textures;
class Map;

class Core
{
public:

	// Constructor
	Core(int argc, char* args[]);

	// Destructor
	virtual ~Core();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

private:

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

public:

	// Modules
	Window*					window = nullptr;
	Input*					input = nullptr;
	Render*					render = nullptr;
	Textures*				textures = nullptr;
	Map*					map = nullptr;

private:

	std::list<Module*> modules;
	int					argc;
	char**				args;

	std::string			title;
	std::string			organization;

};

extern Core* core; // No student is asking me about that ... odd :-S

#endif