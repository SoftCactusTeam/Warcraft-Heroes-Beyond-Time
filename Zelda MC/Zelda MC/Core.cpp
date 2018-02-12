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
	p2List_item<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void Core::AddModule(Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool Core::Awake()
{
	bool ret = true;

	p2List_item<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Awake();
		item = item->next;
	}

	return ret;
}

// Called before the first frame
bool Core::Start()
{
	bool ret = true;
	p2List_item<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		if (item->data->active)
			ret = item->data->Start();

		item = item->next;
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
	p2List_item<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool Core::DoUpdate()
{
	bool ret = true;
	p2List_item<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->Update();
	}

	return ret;
}

// Call modules after each loop iteration
bool Core::PostUpdate()
{
	bool ret = true;
	p2List_item<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool Core::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
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