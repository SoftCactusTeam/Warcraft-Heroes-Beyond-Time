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
	AddModule(console);
	AddModule(gui);
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

	pugi::xml_document doc;
	ret = LoadConfig(doc);

	pugi::xml_node gameConfig = doc.first_child();

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

	startup_time.Start();

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

void Application::PrepareUpdate() 
{
	frame_count++;
	last_sec_frame_count++;

	dt = frame_time.ReadSec();
	frame_time.Start();
}

void Application::FinishUpdate() 
{
	// Framerate calculations --

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	static char title[256];
	sprintf_s(title, 256, "Warcraft: Heroes Beyond Time   Av.FPS: %.2f"/*Last Frame Ms: %u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %lu */,
		avg_fps, last_frame_ms, frames_on_last_update, dt, seconds_since_startup, frame_count);
	App->window->SetTitle(title);
	

	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		SDL_Delay(capped_ms - last_frame_ms);
	}
}

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

bool Application::LoadConfig(pugi::xml_document& doc)
{
	bool result = false;
	
	char* buffer;
	uint size = App->fs->Load("config.xml", &buffer);
	result = doc.load_buffer(buffer, size);
	delete[] buffer;

	return result;
}
