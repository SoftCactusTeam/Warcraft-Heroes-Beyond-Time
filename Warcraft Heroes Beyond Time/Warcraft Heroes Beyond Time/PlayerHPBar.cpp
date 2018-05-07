#include "PlayerHPBar.h"
#include "Log.h"
#include "PlayerEntity.h"
#include "ModuleRender.h"
#include "Application.h"

PlayerHPBar::PlayerHPBar(PlayerEntity* player, fPoint localPos, GUIElem* parent) : GUIElem(localPos, nullptr, BAR, GUIElemType::PLAYER_HP_BAR, parent), player(player) 
{}

PlayerHPBar::~PlayerHPBar()
{
	DestroyChilds();
}

bool PlayerHPBar::Update(float dt)
{
	SDL_Rect hp_full = HP_COLOR;
	SDL_Rect energy_full = ENERGY_COLOR;

	uint percent_hp = (player->numStats.hp * 100) / player->numStats.maxhp;
	hp.w = (percent_hp * hp_full.w) / 100;
	
	uint percent_energy = player->numStats.energy;
	if (percent_energy == 100)
		energy = FULL_ENERGY_COLOR;
	else
	{
		energy = energy_full;
		energy.w = (percent_energy * energy_full.w) / 100;
	}

	return true;
}

bool PlayerHPBar::Draw()
{
	//Blit back
	bool ret = true;

	if(ret)
		ret = App->render->Blit(App->gui->getAtlas(), hpPos.x + localPos.x, hpPos.y + localPos.y, &SDL_Rect(BAR_BACK), 1, 0);
	if(ret)
		ret = App->render->Blit(App->gui->getAtlas(), energyPos.x + localPos.x, energyPos.y + localPos.y, &SDL_Rect(BAR_BACK), 1, 0);

	//Blit HP
	if (ret)
		ret = App->render->Blit(App->gui->getAtlas(), hpPos.x + localPos.x, hpPos.y + localPos.y, &hp, 1, 0);
	//Blit Energy
	if (ret)
		ret = App->render->Blit(App->gui->getAtlas(), energyPos.x + localPos.x, energyPos.y + localPos.y, &energy, 1, 0);
	
	//Blit Bar
	if (ret)
		ret = App->render->Blit(App->gui->getAtlas(), localPos.x, localPos.y, &SDL_Rect(BAR), 1, 0);
	
	//Blit Portrait

	//Blit Consumable


	return ret;

}