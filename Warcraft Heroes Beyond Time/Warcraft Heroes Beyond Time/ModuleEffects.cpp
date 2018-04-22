#include "ModuleEffects.h"
#include "Application.h"
#include "ModuleTextures.h"

#include "Brofiler\Brofiler.h"

ModuleEffects::ModuleEffects() : Module()
{
	name = "Effects";
}

ModuleEffects::~ModuleEffects() {}

bool ModuleEffects::Awake(pugi::xml_node& guiNode)
{
	
	return true;
}

bool ModuleEffects::Start()
{
	
	return true;
}

bool ModuleEffects::PreUpdate()
{
	return true;
}

bool ModuleEffects::Update(float dt)
{
	return true;
}

bool ModuleEffects::PostUpdate()
{
	bool ret = true;

	
	return ret;
}


bool ModuleEffects::CleanUp()
{
	return true;
}

//----------------------------------------------------------------------------------------------------//



SDL_Texture* ModuleEffects::GetAtlas() const
{
	return atlas;
}
