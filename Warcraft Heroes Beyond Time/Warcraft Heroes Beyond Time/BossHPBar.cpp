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
	//Blit Bar
	App->render->Blit(App->gui->getAtlas(), localPos.x, localPos.y, &atlasRect, 1, 0);

	//Blit red part
	App->render->Blit(App->gui->getAtlas(), localPos.x + 1, localPos.y + 1, &actualrect, 1, 0);
}
