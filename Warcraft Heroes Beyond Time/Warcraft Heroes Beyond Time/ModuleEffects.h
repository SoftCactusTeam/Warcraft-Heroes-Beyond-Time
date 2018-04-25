#ifndef __MODULE_EFFECTS_H__
#define __MODULE_EFFECTS_H__

#include "Module.h"
#include "EffectsElem.h"

#include "p2Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"
#include <list>

class EffectsElem;

class ModuleEffects : public Module
{
public:

	ModuleEffects();
	virtual ~ModuleEffects();

	void Init()
	{
		active = false;
	}

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:
	EffectsElem * CreateEffect(fPoint pos, float life, TimeBasedAnimation& effectAnim);
	bool DestroyEffect(EffectsElem* elem);
	SDL_Texture* ModuleEffects::GetAtlas() const;

public:

	std::list<EffectsElem*> effectsList;
	std::list<EffectsElem*> effectsToKill;

	//Here we have all animations charged
	TimeBasedAnimation playerDustAnim;

private:
	SDL_Texture* effects_atlas = nullptr;
};

#endif // __MODULE_EFFECTS_H__