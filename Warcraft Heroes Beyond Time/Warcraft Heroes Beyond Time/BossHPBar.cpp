#include "BossHPBar.h"
#include "BossEntity.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGUI.h"

BossHPBar::BossHPBar(BossEntity* boss, fPoint localPos, GUIElem* parent) : GUIElem(localPos, nullptr, {300, 829, 312, 10}, GUIElemType::BOSS_HP_BAR, parent), boss(boss)
{}

bool BossHPBar::Update(float dt)
{
	SDL_Rect full = {301, 841, 310, 8};
	uint percent_hp = 0;

	if(boss != nullptr)
		 percent_hp = (boss->numStats.hp * 100) / boss->numStats.maxhp;

	actualrect.w = (percent_hp * full.w) / 100;

	return true;
}

bool BossHPBar::Draw()
{
	bool ret = true;

	//Blit red part
	 ret = App->render->Blit(App->gui->getAtlas(), localPos.x + 1 - App->render->camera.x, localPos.y + 1 - App->render->camera.y, &actualrect);

	//Blit Bar
	 if(ret)
		ret = App->render->Blit(App->gui->getAtlas(), localPos.x - App->render->camera.x, localPos.y - App->render->camera.y, &atlasRect);

	return ret;
}
