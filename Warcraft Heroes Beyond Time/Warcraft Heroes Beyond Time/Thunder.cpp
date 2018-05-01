#include "Thunder.h"

#include "Application.h"
#include "ModulePrinter.h"

Thunder::Thunder(const ThunderInfo* info, Projectile_type type) : Projectile(info, type)
{

	toData = (ThunderInfo*)info;
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 5,236,37,131 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 37,236,37,131 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 69,236,37,131 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 104,236,37,131 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 143,236,37,131 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 172,236,37,131 });
	thunderAnims[(uint)ThunderAnimations::cast].PushBack({ 209,236,37,131 });
	thunderAnims[(uint)ThunderAnimations::cast].speedFactor = 4.5f;
	thunderAnims[(uint)ThunderAnimations::cast].loop = false;

	actualAnim = &thunderAnims[(uint)ThunderAnimations::cast];
}
Thunder::~Thunder()
{
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
