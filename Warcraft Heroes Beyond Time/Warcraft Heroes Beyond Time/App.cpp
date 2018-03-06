#include <iostream> 

#include "p2Defs.h"
#include "Log.h"
#include "App.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleEntitySystem.h"
#include "FileSystem.h"
#include "ModuleAudio.h"
#include "Scene.h"

#include "Fonts.h"

App::App(int argc, char* args[]) : argc(argc), args(args)
{
	window = new Window();
	render = new Render();
	input = new Input();
	textures = new Textures();
	entities = new EntitySystem();
	fonts = new Fonts();
	fs = new FileSystem();
	audio = new Audio();
	scene = new Scene();

	//map = new Map();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp

	AddModule(window);
	AddModule(input);
	AddModule(textures);
	AddModule(entities);
	AddModule(audio);
	AddModule(scene);
	AddModule(fonts);
	AddModule(fs);

	//AddModule(map);

	// render last to swap buffer
	AddModule(render);
}

App::~App()
{
	std::list<Module*>::reverse_iterator it;
	for (it = modules.rbegin(); it != modules.rend(); ++it)
	{
		delete (*it);
	}

	modules.clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.push_back(module);
}

bool App::Awake()
{
	bool ret = true;

	std::list<Module*>::const_iterator item;

	for (item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		ret = (*item)->Awake();
	}

	return ret;
}

bool App::Start()
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

bool App::Update()
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

void App::PrepareUpdate() {}

void App::FinishUpdate() {}

bool App::PreUpdate()
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

bool App::DoUpdate()
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

		ret = (*item)->Update(dt);
	}

	return ret;
}

bool App::PostUpdate()
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

bool App::CleanUp()
{
	bool ret = true;

	std::list<Module*>::reverse_iterator it;

	for (it = modules.rbegin(); it != modules.rend(); ++it)
	{
		ret = (*it)->CleanUp();
	}

	return ret;
}

int App::GetArgc() const
{
	return argc;
}

const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

const char* App::GetTitle() const
{
	return title.data();
}

const char* App::GetOrganization() const
{
	return organization.data();
}