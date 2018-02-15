#include <iostream> 

#include "p2Defs.h"
#include "p2Log.h"

#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "Core.h"

// Constructor
Core::Core(int argc, char* args[]) : argc(argc), args(args)
{
	input = new Input();
	window = new Window();
	render = new Render();
	textures = new Textures();
	//map = new Map();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(window);
	AddModule(textures);
	//AddModule(map);

	// render last to swap buffer
	AddModule(render);
}

// Destructor
Core::~Core()
{
	// release modules
	std::list<Module*>::const_iterator item;

	for (item = modules.end(); item != modules.begin(); --item)
	{
		delete (*item);
	}

	modules.clear();
}

void Core::AddModule(Module* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool Core::Awake()
{
	bool ret = true;

	std::list<Module*>::const_iterator item;

	for (item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		ret = (*item)->Awake();
	}


	return ret;
}

// Called before the first frame
bool Core::Start()
{
	bool ret = true;
	std::list<Module*>::const_iterator item;

	for (item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		if ((*item)->active)
			ret = (*item)->Start();
	}

	return ret;
}

// Called each loop iteration
bool Core::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT))
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
void Core::PrepareUpdate()
{
}

// ---------------------------------------------
void Core::FinishUpdate()
{

}

// Call modules before each loop iteration
bool Core::PreUpdate()
{
	bool ret = true;

	std::list<Module*>::const_iterator item;
	Module* pModule = nullptr;

	for (item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		pModule = (*item);

		if (pModule->active == false) {
			continue;
		}

		ret = (*item)->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool Core::DoUpdate()
{
	bool ret = true;

	std::list<Module*>::const_iterator item;
	Module* pModule = nullptr;

	for (item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		pModule = (*item);

		if (pModule->active == false) {
			continue;
		}

		ret = (*item)->Update();
	}

	return ret;
}

// Call modules after each loop iteration
bool Core::PostUpdate()
{
	bool ret = true;

	std::list<Module*>::const_iterator item;
	Module* pModule = nullptr;

	for (item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		pModule = (*item);

		if (pModule->active == false) {
			continue;
		}

		ret = (*item)->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool Core::CleanUp()
{
	bool ret = true;

	std::list<Module*>::const_iterator item;

	for (item = modules.end(); item != modules.begin() && ret == true; --item)
	{
		ret = (*item)->CleanUp();
		
	}

	return ret;
}

// ---------------------------------------
int Core::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* Core::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* Core::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* Core::GetOrganization() const
{
	return organization.GetString();
}