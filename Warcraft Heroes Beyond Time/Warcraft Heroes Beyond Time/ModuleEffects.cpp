#include "ModuleEffects.h"
#include "Application.h"
#include "ModuleTextures.h"

#include "EffectsElem.h"

#include "Brofiler\Brofiler.h"

ModuleEffects::ModuleEffects() : Module()
{
	name = "effects";

	playerDustAnim.PushBack({ 0, 0, 24, 19 });
	playerDustAnim.PushBack({ 24, 0, 24, 19 });
	playerDustAnim.PushBack({ 0, 19, 24, 19 });
	playerDustAnim.PushBack({ 24, 19, 24, 19 });
	playerDustAnim.loop = false;

	playerReliveAnim.PushBack({17,57,58,143});
	playerReliveAnim.PushBack({78,57,58,143});
	playerReliveAnim.PushBack({141,57,58,143});
	playerReliveAnim.PushBack({204,57,58,143});
	playerReliveAnim.PushBack({266,57,58,143});
	playerReliveAnim.PushBack({329,57,58,143});
	playerReliveAnim.PushBack({392,57,58,143});
	playerReliveAnim.loop = false;
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
			effectsList.remove(*it);
			delete *it;
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

EffectsElem * ModuleEffects::CreateEffect(fPoint pos, float life, TimeBasedAnimation& effectAnim)
{
	TimeBasedAnimation anim = effectAnim;
	anim.life = life;

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