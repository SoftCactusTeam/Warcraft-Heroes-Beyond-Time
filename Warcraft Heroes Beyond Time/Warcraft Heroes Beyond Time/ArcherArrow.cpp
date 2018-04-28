#include "ArcherArrow.h"
#include "Application.h"
#include "ModulePrinter.h"

#include "Scene.h"
#include "ModuleInput.h"

ArcherArrow::ArcherArrow(const ArcherArrowInfo* info, Projectile_type type) : Projectile(info, type)
{
	arrowAnims[(uint)ArrowAnimations::moving_anim].PushBack({ 21,33,14,14 });
	arrowAnims[(uint)ArrowAnimations::moving_anim].PushBack({ 36,33,14,14 });
	arrowAnims[(uint)ArrowAnimations::moving_anim].speedFactor = 4.5f;

	actualAnim = &arrowAnims[(uint)ArrowAnimations::moving_anim];

	toData = (ArcherArrowInfo*)info;
}

ArcherArrow::~ArcherArrow()
{
	RELEASE(data);
}

bool ArcherArrow::Update(float dt)
{
	bool ret = true;

	actualAnim->speed = actualAnim->speedFactor * dt;

	return ret;
}

bool ArcherArrow::Draw() const
{
	bool ret = true;

	ret = App->printer->PrintSprite({ (int)data->pos.x - 7,(int)data->pos.y - 7 }, (SDL_Texture*)App->projectiles->GetProjectileAtlas(), actualAnim->GetCurrentFrame(), data->layer, ModulePrinter::Pivots::UPPER_LEFT);

	return ret;
}

void ArcherArrow::OnCollision(Collider* yours, Collider* collideWith)
{
}

void ArcherArrow::OnCollisionContinue(Collider* yours, Collider* collideWith)
{
}

void ArcherArrow::OnCollisionLeave(Collider* yours, Collider* collideWith)
{
}
