#include "Thunder.h"

#include "Application.h"
#include "ModulePrinter.h"
#include "ModuleColliders.h"

Thunder::Thunder(const ThunderInfo* info, Projectile_type type) : Projectile(info, type)
{

	toData = (ThunderInfo*)info;
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 0,237,34,130 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 34, 237,34,130 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 69, 237,34,130 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 104,237,34,130 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 139,237,34,130 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 174,237,34,130 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 209,237,34,130 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 244,237,34,130 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 279,237,34,130 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 314,237,34,130 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 349,237,34,130 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 384,237,34,130 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 419,237,34,130 });
	thunderAnims[(uint)ThunderAnimations::cast].speedFactor = 9.0f;
	thunderAnims[(uint)ThunderAnimations::cast].loop = false;

	actualAnim = &thunderAnims[(uint)ThunderAnimations::cast];

	int minusX = info->pos.x / 48.0f;
	int minusY = info->pos.y / 48.0f;
	
	projCollider = *App->colliders->AddEnemyAttackCollider({ (int)info->pos.x - (minusX * 2), (int)info->pos.y - (minusY * 2), 48, 48 }, nullptr, 50, EnemyAttack::E_Attack_Type::GULDAN_BALL).lock();

}

Thunder::~Thunder()
{
	App->colliders->deleteCollider(projCollider);
	RELEASE(data);
}

bool Thunder::Update(float dt)
{
	bool ret = true;

	if (actualAnim->Finished())
		App->projectiles->DestroyProjectile(this);

	actualAnim->speed = actualAnim->speedFactor * dt;

	return ret;
}

bool Thunder::Draw() const
{
	bool ret = true;

	ret = App->printer->PrintSprite({ (int)data->pos.x - 30/2,(int)data->pos.y - 131 + 20 }, (SDL_Texture*)App->projectiles->GetProjectileAtlas(), actualAnim->GetCurrentFrame(), data->layer, ModulePrinter::Pivots::UPPER_LEFT);

	return ret;
}

void Thunder::OnCollision(Collider* yours, Collider* collideWith)
{
}

void Thunder::OnCollisionContinue(Collider* yours, Collider* collideWith)
{
}

void Thunder::OnCollisionLeave(Collider* yours, Collider* collideWith)
{
}
