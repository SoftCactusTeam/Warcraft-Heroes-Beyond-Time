#include "EffectsElem.h"
#include "Application.h"
#include "ModulePrinter.h"

EffectsElem::EffectsElem(fPoint pos, Animation effectAnim) : pos(pos), effectAnim(effectAnim)
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
	if(effectAnim.Finished())
		App->effects->DestroyEffect(this);

	return true;
}

void EffectsElem::MoveEffect(fPoint pos)
{
	this->pos = pos;
}
