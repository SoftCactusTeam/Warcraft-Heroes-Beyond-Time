#include <iostream> 

#include "p2Defs.h"
#include "Log.h"
#include "Application.h"

#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleEntitySystem.h"
#include "FileSystem.h"

#include "ModuleAudio.h"
#include "Scene.h"
#include "Console.h"

#include "Fonts.h"
#include "ModuleGUI.h"

Application::Application(int argc, char* args[]) : argc(argc), args(args)
{
	window = new Window();
	render = new Render();
	input = new Input();
	textures = new Textures();
	entities = new EntitySystem();
	fonts = new Fonts();
	fs = new FileSystem();
	gui = new ModuleGUI();
	audio = new Audio();
	scene = new Scene();
	console = new Console();

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
	AddModule(gui);
	AddModule(console);
	//AddModule(map);

	// render last to swap buffer
	AddModule(render);

	organization = "SoftCactus";
	title = "Warcraft: Heroes Beyond Time";
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator it;
	for (it = modules.rbegin(); it != modules.rend(); ++it)
	{
		delete (*it);
	}

	modules.clear();
}

void Application::AddModule(Module* module)
{
	module->Init();
	modules.push_back(module);
}

bool Application::Awake()
{
	bool ret = true;

	ret = LoadConfig();

	//Here goes App configuration

	std::list<Module*>::const_iterator item;

	for (item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		ret = (*item)->Awake(gameConfig.child((*item)->name.data()));
	}

	return ret;
}

bool Application::Start()
{
	bool ret = true;
	std::list<Module*>::const_iterator item;

	for (item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		if ((*item)->isActive())
			ret = (*item)->Start();
	}

	return ret;
}

bool Application::Update()
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

void Application::PrepareUpdate() {}

void Application::FinishUpdate() {}

bool Application::PreUpdate()
{
	bool ret = true;

	std::list<Module*>::const_iterator item;
	Module* pModule = nullptr;

	for (item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		pModule = (*item);

		if (pModule->isActive() == false) 
		{
			continue;
		}

		ret = (*item)->PreUpdate();
	}

	return ret;
}

bool Application::DoUpdate()
{
	bool ret = true;

	std::list<Module*>::const_iterator item;
	Module* pModule = nullptr;

	for (item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		pModule = (*item);

		if (pModule->isActive() == false) 
		{
			continue;
		}

		ret = (*item)->Update(dt);
	}

	return ret;
}

bool Application::PostUpdate()
{
	bool ret = true;

	std::list<Module*>::const_iterator item;
	Module* pModule = nullptr;

	for (item = modules.begin(); item != modules.end() && ret == true; ++item)
	{
		pModule = (*item);

		if (pModule->isActive() == false) 
		{
			continue;
		}

		ret = (*item)->PostUpdate();
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	std::list<Module*>::reverse_iterator it;

	for (it = modules.rbegin(); it != modules.rend(); ++it)
	{
		ret = (*it)->CleanUp();
	}

	return ret;
}

int Application::GetArgc() const
{
	return argc;
}

const char* Application::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

const char* Application::GetTitle() const
{
	return title.data();
}

const char* Application::GetOrganization() const
{
	return organization.data();
}

bool Application::LoadConfig()
{
	bool result = false;

	pugi::xml_document doc;
	SDL_RWops* config = App->fs->Load("config.xml");
	result = doc.load_buffer(config, sizeof(config));
	SDL_RWclose(config);
	
	gameConfig = doc.child("Game");

	return result;
}
