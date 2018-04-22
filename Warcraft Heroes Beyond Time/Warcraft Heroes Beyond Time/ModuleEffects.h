#ifndef __MODULE_EFFECTS_H__
#define __MODULE_EFFECTS_H__

#include "Module.h"

#include "p2Point.h"
#include "SDL/include/SDL.h"
#include <list>

class ModuleEffects : public Module
{
public:

	ModuleEffects();
	virtual ~ModuleEffects();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();



public:
	SDL_Texture* ModuleEffects::GetAtlas() const;


public:




private:
	SDL_Texture* atlas = nullptr;
};

#endif