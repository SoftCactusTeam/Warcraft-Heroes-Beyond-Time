#include "ModuleEffects.h"
#include "Application.h"
#include "ModuleTextures.h"

#include "EffectsElem.h"

#include "Brofiler\Brofiler.h"

ModuleEffects::ModuleEffects() : Module()
{
	name = "Effects";

	playerDustAnim.PushBack({ 0, 0, 24, 19 });
	playerDustAnim.PushBack({ 24, 0, 24, 19 });
	playerDustAnim.PushBack({ 0, 19, 24, 19 });
	playerDustAnim.PushBack({ 24, 19, 24, 19 });
	playerDustAnim.speedFactor = 5.0f;
	playerDustAnim.loop = false;
}

ModuleEffects::~ModuleEffects() {}

bool ModuleEffects::Awake(pugi::xml_node& node)
{
	return true;
}

bool ModuleEffects::Start()
{
	effects_atlas = App->textures->Load("sprites/effects.png");
	return true;
}

bool ModuleEffects::PreUpdate()
{
	return true;
}

bool ModuleEffects::Update(float dt)
{
	bool res = true;

	std::list<EffectsElem*>::iterator it;
	for (it = effectsList.begin(); it != effectsList.end() && res; ++it)
	{
		res = (*it)->UpdateEffects(dt);
	}

	playerDustAnim.speed = playerDustAnim.speedFactor * dt;

	return res;
}

bool ModuleEffects::PostUpdate()
{
	bool ret = true;

	if (effectsToKill.size() > 0)
	{
		std::list<EffectsElem*>::iterator it;
		for (it = effectsToKill.begin(); it != effectsToKill.end(); ++it)
		{
			std::list<EffectsElem*>::iterator it2;
			for (it2 = effectsList.begin(); it2 != effectsList.end(); ++it2)
			{
				if (*it2 == *it)
				{
					delete *it;
					effectsList.erase(it2);
					break;
				}
			}
		}
		effectsToKill.clear();
	}

	ret = effectsToKill.size() <= 0;

	if (ret)
	{
		std::list<EffectsElem*>::iterator it;
		for (it = effectsList.begin(); it != effectsList.end() && ret; ++it)
		{
			ret = (*it)->DrawEffects();
		}
	}

	return ret;
}


bool ModuleEffects::CleanUp()
{
	BROFILER_CATEGORY("ClearEffects", Profiler::Color::BlanchedAlmond);

	std::list<EffectsElem*>::reverse_iterator it;
	for (it = effectsList.rbegin(); it != effectsList.rend(); ++it)
	{
		delete (*it);
	}
	effectsList.clear();
	effectsToKill.clear();

	App->textures->UnLoad(effects_atlas);

	return effectsList.size() <= 0;
}

//----------------------------------------------------------------------------------------------------//

EffectsElem * ModuleEffects::CreateEffect(fPoint pos, float life, Animation& effectAnim)
{
	Animation anim = effectAnim;
	anim.speedFactor = anim.getFrames() / life;
	anim.speed = anim.getFrames() / life;

	EffectsElem* tmpEffect = new EffectsElem(pos, anim);
	
	effectsList.push_back(tmpEffect);

	return tmpEffect;
}

bool ModuleEffects::DestroyEffect(EffectsElem * elem)
{
	if (elem != nullptr)
	{
		effectsToKill.push_back(elem);
		return true;
	}
	return false;
}


SDL_Texture* ModuleEffects::GetAtlas() const
{
	return effects_atlas;
}