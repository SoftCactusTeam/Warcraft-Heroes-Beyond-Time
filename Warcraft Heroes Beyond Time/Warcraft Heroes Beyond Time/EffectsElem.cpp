#include "EffectsElem.h"
#include "Application.h"
#include "ModulePrinter.h"

EffectsElem::EffectsElem(fPoint pos, float life, Animation effectAnim) : pos(pos), life(life), effectAnim(effectAnim)
{
}

bool EffectsElem::DrawEffects()
{
	bool ret = true;
	ret = App->printer->PrintSprite({ (int)pos.x, (int)pos.y }, App->effects->GetAtlas(), effectAnim.GetCurrentFrame(), -1);

	return ret;
}

bool EffectsElem::UpdateEffects(float dt)
{
	if (timer >= life)
		App->effects->DestroyEffect(this);
	else
		timer += dt;

	return true;
}

void EffectsElem::MoveEffect(fPoint pos)
{
	this->pos = pos;
}
